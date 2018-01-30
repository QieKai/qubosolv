#include <iostream>
#include <fstream>
using namespace std;

void printMatrix(double**, int);

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
    if (argc != 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else
    {
        ifstream infile( argv[1] );
        if (infile.is_open()==0)
            cout << "Could not open file" << endl;
        else
        {
            int n;
            infile >> n;
            
            // Start of: initializing Q
            double **Q= new double*[n];
            for (int i=0; i<n; i++)
            {
                Q[i] = new double[n];
                for (int j=0; j<n; j++)
                    Q[i][j] = 0;
            }
            
            for (int i=0; i<n; i++)
            {
                for(int j=0;j<n;j++)
                    infile >> Q[i][j];
            }
            infile.close();
            
            for (int i=0; i<n; i++)
            {
                for(int j=i+1;j<n;j++)
                {
                    Q[i][j]=Q[i][j]/2;
                    Q[j][i]=Q[i][j];
                }
            }
            printMatrix(Q,n);
            
            ofstream myfile ("qubo.txt");
            if (myfile.is_open())
            {
                myfile << n<<endl;
                for (int i=0; i<n; i++)
                {
                    for(int j=0;j<n;j++)
                        myfile << Q[i][j]<<" ";
                    myfile <<endl;
                }
                
                myfile.close();
            }
            else cout << "Unable to open file";
            for (int i=0; i<n; i++)
                delete [] Q[i];
            delete [] Q;
        }
    }
    return 0;
}
