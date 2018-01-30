/**********************
 
 This program implements the simulated annealing-based heuristic for the unconstrained binary quadratic
 programming problem, as in:
 
 Kengo Katayama, Hiroyuki Narihisa. "Performance of simulated annealing-based heuristic for the unconstrained
 binary quadratic programming problem". European Journal of Operational Research 134 (2001) 103-119.
 
 The file accepts input from a file stream.
 The first input from input file is an integer m, representing number of test cases. For each i={1,2..m}, program
 reads in integers n and k, representing problem size and number of non-zero entries in matrix Q. Immediately
 followed by k lines of 3 numbers (a, b, c), meaning Q[a,b] = Q[b,a]= c.
 
 The number of runs RUNS, and parameters for the SA heuristic can be adjusted in the definitions.
 
 The program outputs to ostream. For each test case, the program executes RUNS times. For each time the program is
 executed, the objective value and amount of time taken (in microseconds) is output.
 *********************/

#define RUNS 30
#define tInit (0.3*n)
#define tFactor 0.99
#define termCount 10
#define saCount 2
#define startTFactor 0.8
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <sys/time.h>
#define db(x) cout << x << endl

using namespace std;

double solveQ(int **, int, int*);
void genRandomInitial(int*, int);
void printMatrix(int**, int);
void printVector(int*, int);
double evaluate(int**, int *, int);
void SA(int*, double, int**, int);
void updateGains(int*, int**, int*, int, int);
void fyShuffle(int*, int);

//Input: n-vector x
//Comments: performs a Fisher-Yates random shuffle on all n elements of x
void fyShuffle(int* x, int n)
{
    int i,j,temp;
    for (i=n; i> 1; i--)
    {
        j= rand()%i+1;
        temp= x[j];
        x[j]=x[i];
        x[i]= temp;
    }
}

//Comments: modular function to update the gain functions
void updateGains(int *g, int **Q, int *x, int n, int k)
{
    int i;
    for (i=1; i< n+1; i++)
        if ( i != k)
            g[i] += 2*Q[i][k]* (1-2*x[i])*(2*x[k]-1);
        else
            g[i] = -1*g[i];
}

//Input: symmetric n-dimension matrix Q, n-vector xBest
//Returns: x'Qx where x is the solution vector obtained from simulated annealing
//Comments: saCount is the number of annealings.
double solveQ(int **Q, int n, int *xBest)
{
    genRandomInitial(xBest,n);
    int *x = new int[n+1];
    double T = tInit;
    for (int i=0; i< saCount; i++)
    {
        memcpy(x, xBest, sizeof(int)*(n+1));
        SA(x,T,Q,n);
        memcpy(xBest, x, sizeof(int)*(n+1));
        T = startTFactor*T;
    }
    delete [] x;
    return evaluate(Q, xBest, n);
}

//Input: n-vector x, starting temperature for Simulated-Annealing T, symmetric n-dimension matrix Q
//Comments: runs a single annealing, after which x is the solution vector obtained from the annealing
void SA(int *x, double T, int** Q, int n)
{
    int *x_best= new int[n+1], *RP = new int[n+1];
    int *g= new int[n+1],counter=0,i,j,k;
    memcpy(x_best, x, sizeof(int)*(n+1));
    int f_x_best= evaluate(Q, x_best, n);
    int f_x= f_x_best;
    
    for (i=1; i< n+1; i++)
    {
        g[i] = Q[i][i]*(1-2*x[i]);
        for (j=1; j<n+1; j++)
            if (i != j)
                g[i] += 2*Q[i][j]*x[j]*(1-2*x[i]);
    }
    
    while (termCount > counter)
    {
        counter++;
        for (i=0; i< n+1; i++)
            RP[i] = i;
        random_shuffle(RP+1, RP+n+1);
        for (j=1; j< n+1; j++)
        {
            k= RP[j];
            if (g[k] > 0)
            {
                counter=0;
                x[k] = 1- x[k];
                f_x= f_x + g[k];
                updateGains(g, Q, x, n, k);
                if (f_x > f_x_best)
                {
                    memcpy(x_best, x, sizeof(int)*(n+1));
                    f_x_best= f_x;
                }
            }
            else
            {
                double p = 1.0*rand()/RAND_MAX;
                if (p < exp(1.0*g[k]/T))
                {
                    x[k] = 1- x[k];
                    f_x = f_x+ g[k];
                    updateGains(g, Q, x, n, k);
                }
            }
        }
        T = tFactor*T;
    }
    memcpy(x, x_best, sizeof(int)*(n+1));
    delete [] x_best;
    delete [] g;
    delete [] RP;
}

//Input: symmetric n-dimension matrix Q, n-binary-vector x
//Returns: value of x'Qx
double evaluate(int **Q, int *x, int n)
{
    int sum=0;
    for (int i=1; i<n+1; i++)
        for (int j=1; j< n+1; j++)
            sum += x[i]*x[j]*Q[i][j];
    return sum;
}

//Input: n-vector x
//Comments: x becomes a n-binary-vector, with each bit having an equal probability of being 0 or 1
void genRandomInitial(int *x, int n)
{
    for (int i =1; i< n+1; i++)
        x[i] = rand()%2;
}

//Comments: prints out a n-by-n matrix
void printMatrix(double **Q, int n)
{
    for (int i=1; i<n+1; i++)
    {
        for (int j=1; j<n+1; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }
}

//Comments: prints out a n-vector
void printVector(int *x, int n)
{
    //Prints out n-vector.
    
    for (int i=1; i<n+1; i++)
        cout << x[i] << " ";
    cout << endl;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    if (argc != 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else
    {
        ifstream infile( argv[1] );
        if (infile.is_open()==0)
            cout << "Could not open file" << endl;
        else
        {
            int m, run;
            infile >> m;
            for (; m>0; m--)
            {
                int n, k;
                infile >> n >> k;
                
                //start of: initializing Q
                int **Q= new int*[n+1];
                for (int i=0; i<n+1; i++)
                {
                    Q[i] = new int[n+1];
                    for (int j=0; j<n+1; j++)
                        Q[i][j] = 0;
                }
                
                for (int i=0; i<k; i++)
                {
                    int a,b,c;
                    infile >> a >> b >> c;
                    Q[a][b]=c;
                    Q[b][a]=c;
                }
                //end of: initializing Q
                
                int *x = new int[n+1];
                
                for (run=0; run< RUNS; run++)
                {
                    struct timeval start, end;
                    gettimeofday(&start, NULL);
                    //iter=0;
                    cout << fixed << solveQ(Q,n,x);
                    gettimeofday(&end, NULL);
                    cout << " "; 
                    cout << (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
                    cout << " " << endl;
                }
                cout << endl;
                
                for (int i=0; i<n; i++)
                    delete [] Q[i];
                delete [] Q;
                delete [] x;
            }
        }
    }
    return 0;
}
