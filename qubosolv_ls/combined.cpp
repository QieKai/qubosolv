/**********************
 
 This program implements a greedy neighbourhood local search heuristic for the unconstrained
 binary quadratic programming problem. The neighbourhood is a combination of the 1-opt neighbour and the 2-opt
 neighbourhood; where the k-opt local neighbourhood of a binary n-vector x,
 as the set of binary n-vector x" where the Hamming distance from x to x" is strictly k.
 
 The file accepts input from a file stream.
 The first input from input file is an integer m, representing number of test cases. For each i={1,2..m}, program
 reads in integers n and k, representing problem size and number of non-zero entries in matrix Q. Immediately
 followed by k lines of 3 numbers (a, b, c), meaning Q[a,b] = Q[b,a]= c.
 
 The number of runs RUNS can be adjusted in the definitions.
 
 The program outputs to ostream. For each test case, the program executes RUNS times. For each time the program is
 executed, the objective value and amount of time taken (in microseconds) is output.
 *********************/

#define RUNS 1000000000
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#define db(x) cout << x << endl

using namespace std;

double solveQ(double **, int, int*);
void genRandomInitial(int*, int);
void printMatrix(double**, int);
void printVector(int*, int);
double evaluate(double**, int *, int);
void calculateGains(double *g, double** G,double** Q, int*, int);

//Input: n-vector g, n-by-n matrix G, symmetric n-by-n matrix Q, n-binary-vector x
//Comments: g[i] and G[j][k] represents the gain in objective function due to flipping bit i and bits j/k
//		respectively.
void calculateGains(double *g, double** G,double** Q, int* x, int n)
{
    int i, j;
    for (i=1; i<n+1; i++)
    {
        g[i]= Q[i][i]*(1-2*x[i]);
        for (j=1; j<n+1; j++)
            if (i != j)
                g[i] = g[i]+ 2* Q[i][j] * x[j] *(1-2*x[i]);
    }
    
    for (i=1; i<n+1; i++)
        for (j=1; j<n+1; j++)
        {
            if (i==j)
                G[i][j]=0;
            else
                G[i][j]= g[i] + g[j] + (1-2*(x[i]-x[j])*(x[i]-x[j]))*2*Q[i][j];
        }
}

//Input: symmetric n-by-n matrix Q, n-vector x
//Output: x'Qx where x is the binary-n-vector of the combined 1/2-opt neighbourhood local maximum solution.
double solveQ(double **Q, int n, int *x)
{
    genRandomInitial(x,n);
    double *g = new double[n+1], **G=new double*[n+1],Gain=0;
    int i,j;
    for (i=0; i< n+1; i++)
        G[i] = new double[n+1];
    
    calculateGains(g, G, Q, x, n);
    
    do {
        int a, b,k=0;
        i=j=1;
        Gain= G[i][j];
        for (i=1; i< n+1; i++)
            for (j=1; j< n+1; j++)
                if ( G[i][j] > Gain)
                {
                    Gain= G[i][j];
                    a=i;
                    b=j;
                }
        
        for (i=1; i<n+1; i++)
            if ( g[i]> Gain)
            {
                Gain = g[i];
                k=i;
            }
        
        if (Gain < 0)
        {
            if (k != 0)
                x[k] = 1- x[k];
            else
            {
                x[a]= 1- x[a];
                x[b]= 1- x[b];
            }
            calculateGains(g, G, Q, x, n);
        }
    } while (Gain < 0);
    
    delete [] g;
    for (i=0; i< n+1; i++)
        delete [] G[i];
    delete [] G;
    return evaluate(Q, x, n);
}

//Input: symmetric n-by-n matrix Q, binary-n-vector x
//Output: x'Qx
double evaluate(double **Q, int *x, int n)
{
    double sum=0;
    for (int i=1; i<n+1; i++)
        for (int j=1; j< n+1; j++)
            sum += x[i]*x[j]*Q[i][j];
    return sum;
}

//Input: n-vector x
//Comments: x becomes a binary-n-vector, with x[i] having equal probability of 0 or 1
void genRandomInitial(int *x, int n)
{
    for (int i =1; i< n+1; i++)
        x[i] = rand()%2;
}

//Comments: prints a n-by-n matrix
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
                //infile >> n >> k;
                infile >> n;
                
                // Start of: initializing Q
                double **Q= new double*[n+1];
                for (int i=0; i<n+1; i++)
                {
                    Q[i] = new double[n+1];
                    for (int j=0; j<n+1; j++)
                        Q[i][j] = 0;
                }
                
                //                for (int i=0; i<k; i++)
                //                {
                //                    int a,b,c;
                //                    infile >> a >> b >> c;
                //                    Q[a][b]=c;
                //                    Q[b][a]=c;
                //                }
                for (int i=1; i<n+1; i++)
                {
                    for(int j=1;j<n+1;j++)
                        infile >> Q[i][j];
                }
                
                //                for(int j=1;j<n+1;j++)
                //                {
                //                    for(int i=1;i<j;i++)
                //                    {
                //                        Q[j][i] = Q[i][j];
                //                    }
                //                }
                //                for(int j=1;j<n+1;j++)
                //                {
                //                    for(int i=1;i<n+1;i++)
                //                    {
                //                        if(i!=j)
                //                            Q[i][j]/=2;
                //                    }
                //                }
                //End of: initializing Q
                printMatrix(Q,n);
                int *x = new int[n+1];
                double minimum=0;
                int *outX = new int[n+1];
                for (run=0; run< RUNS; run++)
                {
                    struct timeval start, end;
                    gettimeofday(&start, NULL);
                    double ret = solveQ(Q,n,x);
                    //cout << fixed << ;
                    gettimeofday(&end, NULL);
                    //                    cout << " " << (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
                    if(minimum>ret)minimum=ret;
                    for(int i=1;i<n+1;i++)
                        outX[i] = x[i];
                    
                    //cout << endl;
                    
                    //cout << endl;
                }
                cout<<minimum<<endl;
                printVector(outX,n);
                //cout << endl;
                
                for (int i=0; i<n; i++)
                    delete [] Q[i];
                delete [] Q;
                delete [] x;
            }
        }
    }
    return 0;
}
