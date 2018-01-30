#define RUNS 100000
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
void calculateGains(int*, double**, int*, int);
void calculateGains(int *g, double** Q, int* x, int n)
{
    int i, j;
    for (i=1; i<n+1; i++)
    {
        g[i]= Q[i][i]*(1-2*x[i]);
        for (j=1; j<n+1; j++)
            if (i != j)
                g[i] = g[i]+ 2* Q[i][j] * x[j] *(1-2*x[i]);
    }
    
}

//Input: symmetric n-by-n matrix Q, n-vector x
//Output: x'Qx where x is the binary-n-vector of a local maximum solution.
double solveQ(double **Q, int n, int *x)
{
    genRandomInitial(x,n);
    
    int *g = new int[n+1],Gain=0;
    int i,j;
   
    
    calculateGains(g, Q, x, n);
    
    do {
        int k=0;
        i=1;
        Gain= g[i];
        
        
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

            calculateGains(g, Q, x, n);
        }
    } while (Gain < 0);
    
    delete [] g;
 
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
    for (int i=1; i<n+1; i++)
        cout << x[i] << " ";
    cout << endl;
}

int main(int argc, char* argv[])
{
    
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
