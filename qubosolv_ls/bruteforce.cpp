/**********************
 
 This program implements a greedy 2-opt neighbourhood local search heuristic for the unconstrained
 binary quadratic programming problem. Define the 2-opt local neighbourhood of a binary n-vector x,
 as the set of binary n-vector x" where the Hamming distance from x to x" is strictly 2.
 

 
 The number of runs RUNS can be adjusted in the definitions.
 
 The program outputs to ostream. For each test case, the program executes RUNS times. For each time the program is
 executed, the objective value and amount of time taken (in microseconds) is output.
 *********************/

#define RUNS 1
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#define db(x) cout << x << endl

using namespace std;

double solveQ(double **, int, int*);
void generateNext(int*, int);
void printMatrix(double**, int);
void printVector(int*, int);
double evaluate(double**, int *, int);

double solveQ(double **Q, int n, int *y)
{
    int *x = new int[n+1];
    double min=0;
    for(int i=1;i<n+1;i++)x[i]=0;
    while(1){
        
        
        int e = evaluate(Q, x, n);
        if(e<min)
        {
            min = e;
            for(int j=1;j<n+1;j++)y[j] = x[j];
        }
        int sum = 0;
        generateNext(x,n);
        //printVector(x,n);
        for(int k = 1;k<n+1;k++)sum+=x[k];
        if(sum==n)break;
    }
    return min;
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
void generateNext(int *x, int n)
{
    int i=n;
    while(x[i]!=0)i--;
    x[i]=1;
    for(int j=n;j>i;j--)
        x[j]=0;
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
                
                for (int i=1; i<n+1; i++)
                {
                    for(int j=1;j<n+1;j++)
                        infile >> Q[i][j];
                }
                
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
