#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/time.h>
#define fMin -99
#define fMax 99
using namespace std;

//Comments: prints a n-by-n matrix
void printMatrix(double **Q, int n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    
    int n = atoi(argv[1]);
    // Start of: initializing Q
    double **Q= new double*[n];
    for (int i=0; i<n; i++)
        Q[i] = new double[n];
    for (int i=0; i<n; i++)
    {
        for (int j=i; j<n; j++)
        {
            //if(i==j)Q[i][j]=0;
            //else
            {
                double f = (double)rand() / RAND_MAX;
                Q[i][j] = fMin + f * (fMax - fMin);
                Q[j][i] = Q[i][j];
            }
        }
    }
    
    ofstream myfile ("qubo.txt");
    ofstream myfile1 ("..//quboSolver_gurobi/qubo.txt");
    if (myfile.is_open() && myfile1.is_open())
    {
        myfile << n<<endl;
        myfile1 << n<<endl;
        for (int i=0; i<n; i++)
        {
            for(int j=0;j<n;j++)
            {
                myfile << Q[i][j]<<" ";
                myfile1 << Q[i][j]<<" ";
            }
            myfile <<endl;
            myfile1 <<endl;
        }
        
        myfile.close();
        myfile1.close();
    }
    
    else cout << "Unable to open file";
    printMatrix(Q,n);
    for (int i=0; i<n; i++)
        delete [] Q[i];
    delete [] Q;
    
    return 0;
}
