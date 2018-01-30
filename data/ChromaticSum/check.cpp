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
void loadAdjaList(int &n, list <pair<int,int> > &adjacent_list);
void writeToFile(double **Q, string filename, int n, int k);

int main(int argc, char *argv[])
{
    int n=0,k=0,optmal=0;
    double **Q;
    list <pair<int,int> > adjacent_list;
    cout<<">>>>>>>>>>>>Input graph:"<<endl;
    loadAdjaList(n,adjacent_list);
    //cout<<">>>>>>>>>>>>done"<<endl;
    cout<<">>>>>>>>>>>>Input k:"<<endl;
    cin >> k;

    const int N = n*k;
    int x[N];
    cout<<">>>>>>>>>>>>input optimal value"<<endl;
    cin >> optmal;
    cout<<">>>>>>>>>>>>input optimal value"<<endl;
    loadSolutions(n,k,x);

    return 0;
}

void loadAdjaList(int &n, int &k, list <pair<int,int> > &adjacent_list)
{
  cin >> n;
  string line;
  int lineCnt=-1;
  while (std::getline(cin, line))
  {
    istringstream iss(line);
  int a;
  while (iss >> a)
  {
    adjacent_list.push_back(make_pair(lineCnt,a));
  }
    lineCnt++;
  }
}

void loadSolutions(int n, int k, int x[]){
  const int N = n*k;
  for(int i=0;i<N;i++)
    cin>>x[i];
  for(int i=0;i<N;i++)
    cout << x[i] <<" ";
  cout<<endl;
}
