#include "two_opt_localSearch.h"
using namespace std;
void calculateGains(double *g, double** G,double** Q, int* x, const int n)
{
    int i, j;
    for (i=1; i<n+1; i++)
    {
        g[i]= Q[i][i]*(1-2*x[i]);
        //g[i]= Q[i][i]*(x[i]==0?-1:1);
        for (j=1; j<n+1; j++)
            if (i != j)
            {
                g[i] = g[i]+ 2* Q[i][j] * x[j] *(1-2*x[i]);
            }
        
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
//Output: x'Qx where x is the binary-n-vector of a 2-opt local maximum solution.
double solveQ(double **Q, const int n, int *x)
{
    genRandomInitial(x,n);
    double *g = new double[n+1], **G=new double*[n+1], Gain=0;
    int i,j;
    for (i=0; i< n+1; i++)
        G[i] = new double[n+1];
    
    calculateGains(g, G, Q, x, n);
    
    do {
        int a=1, b=1;
        Gain= G[a][b];
        for (i=1; i< n+1; i++)
            for (j=1; j< n+1; j++)
                if ( G[i][j] > Gain)
                {
                    Gain= G[i][j];
                    a=i;
                    b=j;
                }
        if (Gain < 0)
        {
            x[a]= 1- x[a];
            x[b]= 1- x[b];
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
double evaluate(double **Q, int *x, const int n)
{
    double sum=0;
    for (int i=1; i<n+1; i++)
        for (int j=1; j< n+1; j++)
            sum += x[i]*x[j]*Q[i][j];
    return sum;
}

//Input: n-vector x
//Comments: x becomes a binary-n-vector, with x[i] having equal probability of 0 or 1
void genRandomInitial(int *x, const int n)
{
    for (int i =1; i< n+1; i++)
        x[i] = rand()%2;
}

void printMatrix(double **Q, const int n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }
}

//Comments: prints out a n-vector
void printVector(int *x, const int n)
{
    for (int i=1; i<n+1; i++)
        cout << x[i] << " ";
    cout << endl;
}

double heuristicSearch(const int n, double** origQ)
{
    int x[n+1];
    double **Q= new double*[n+1];
    for (int i=0; i<n+1; i++)
    {
        Q[i] = new double[n+1];
        for (int j=0; j<n+1; j++)
            Q[i][j] = 0;
    }
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            Q[i+1][j+1] = origQ[i][j];
    
    double minimum=0,ret;
    for (int run=0; run< RUNS; run++)
    {
        ret = solveQ(Q,n,x);
        if(minimum>ret)minimum=ret;
    }
    
    //printVector(x,n);
    //cout<<"best known minimum and minimizer from heuristic is: "<<minimum<<endl;
    for (int i=0; i<n; i++)
        delete [] Q[i];
    delete [] Q;
    return minimum;
}
