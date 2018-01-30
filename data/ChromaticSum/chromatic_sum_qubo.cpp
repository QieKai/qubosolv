// QUBO formulation for chromatic sum problem
// size of k is set to be same as graph size n in this implementation
#include <iostream>
#include <fstream>
#include <utility>
#include <list>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;
void printMatrix(double **Q, const int n);
void loadAdjaList(double **&Q,string filename, int &n, int &k, list <pair<int,int> > &adjacent_list);
void writeToFile(double **Q, string filename, int n, int k);
void makeQuboMatrix(double **Q, int n, int k, list <pair<int,int> > adjacent_list);

int main(int argc, char *argv[])
{
    int n=0,k=0;
    double **Q;
    list <pair<int,int> > adjacent_list;

    if (argc < 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else if(argc > 2)
        k = (int)strtol (argv[2],NULL,10);
    else
        k = -1;
    loadAdjaList(Q,argv[1],n,k,adjacent_list);

    makeQuboMatrix(Q,n,k,adjacent_list);

	writeToFile(Q,argv[1], n, k);

    for (int i=0; i<k*n; i++)
        delete [] Q[i];
    delete [] Q;
    return 0;
}

void loadAdjaList(double **&Q,string filename, int &n, int &k, list <pair<int,int> > &adjacent_list)
{
	ifstream infile( filename );

        //cout<<filename<<endl;
        if (infile.is_open()==0)
            cout << "Could not open file" << endl;
        else
        {
            infile >> n;
            if(k==-1)k=n;
          //  infile >> k;
            // Start of: initializing Q
            const int N = n*k;
            Q = new double*[N];
            for (int i=0; i<N; i++)
            {
                Q[i] = new double[N];
                for (int j=0; j<N; j++)
                    Q[i][j] = 0;
            }

            string line;
            int lineCnt=-1;
			while (std::getline(infile, line))
			{
    			istringstream iss(line);
    			int a;
    			while (iss >> a)
    			{
    				adjacent_list.push_back(make_pair(lineCnt,a));
    			}
    			lineCnt++;
			}
			infile.close();
   //          list<pair<int,int> >::const_iterator iterator;
   //  		for (iterator = adjacent_list.begin(); iterator != adjacent_list.end(); ++iterator)
   //  		{
			// 	cout<<(*iterator).first<<(*iterator).second<<endl;
			// }

        }
}

void makeQuboMatrix(double **Q, int n, int k, list <pair<int,int> > adjacent_list)
{
	const int penalty = n+1;

    //f(x) = sum j*x[i,j]
    for (int i=0; i<n; i++)
    {
		for(int j=0; j<k; j++)
		{
			int idx = i*k+j;
			Q[idx][idx] = Q[idx][idx] + (j+1);
		}
    }
    //    printMatrix(Q,k*n);

    //p1(x) = sum(i) (1- sum(j)x[i,j])^2
    for (int i=0; i<n; i++)
    {
		for(int j=0; j<k; j++)
		{
			for(int l=0; l<k; l++)
			{
				if(j!=l)
				{
					int idx1 = i*k+j;
					int idx2 = i*k+l;
					Q[idx1][idx2] += penalty;
				}
				else
				{
					int idx = i*k+j;
					Q[idx][idx] = Q[idx][idx] - penalty;
				}
			}
		}
    }
    //    printMatrix(Q,k*n);

    //p2(x) = sum(E(G))sum(j) x[u,j]*x[v,j]
    list<pair<int,int> >::const_iterator iterator;

    for (iterator = adjacent_list.begin(); iterator != adjacent_list.end(); ++iterator) {
    	int u = (*iterator).first;
		int v = (*iterator).second;

    	for(int j=0; j<k; j++)
		{
			Q[u*k+j][v*k+j] += penalty;
		}
	}

	//printMatrix(Q,k*n);
}

void writeToFile(double **Q, string filename, int n, int k)
{
  int N = n*k;
	//filename.replace(filename.find(".alist"), 6, ".H");
	//ofstream myfile (filename);
    //if (myfile.is_open())
    {
        cout << N <<" "<< k<<endl;

        for (int i=0; i<N; i++)
        {
            for(int j=0;j<N;j++)
            {
                cout << Q[i][j]<<" ";
            }
            cout <<endl;
        }

        //myfile.close();
    }
}

void printMatrix(double **Q, const int n, const int k)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }
}
