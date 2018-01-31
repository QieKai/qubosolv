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
void loadSolutions(int n, int k, int x[]);
bool IsLegalColoring(int x[], int n, int k, list <pair<int,int> > adjacent_list);

int main(int argc, char *argv[])
{
    int n=0,k=0,optimal=0;
    double **Q;
    list <pair<int,int> > adjacent_list;
    cout<<">>>>>>>>>>>>Input graph: "<<endl;
    loadAdjaList(n,adjacent_list);
    cin >> k;
    cout<<">>>>>>>>>>>>Input k: "<<k<<endl;

    const int N = n*k;
    int x[N];

    cin >> optimal;
    cout<<">>>>>>>>>>>>Input optimal value: "<<optimal <<endl;
    cout<<">>>>>>>>>>>>Input solution: "<<endl;
    loadSolutions(n,k,x);

    cout<<">>>>>>>>>>>>finish input"<<endl;
    if (IsLegalColoring(x,n,k,adjacent_list))
    {
      int sl = 0;
      for (int i = 0; i < n; i++)
      {
        for (int j = 0; j < k; j++)
        {
            sl = sl + x[i*k+j] * (j+1);
        }
      }
      cout<<">>>>>>>>>>>>Chromatic sum is: "<<sl<<endl;
    }
    else
      cout<<">>>>>>>>>>>>Optimize failed since the coloring is illegal"<<endl;

    return 0;
}

bool IsLegalColoring(int x[], int n, int k, list <pair<int,int> > adjacent_list){
  const int N =n;
  bool ret=true;
  int c[N];
  for (int i=0;i<n;i++)
  {
    int sumj=0;

    for(int j=0;j<k;j++)
    {
      sumj+=x[i*k+j];
      if(x[i*k+j] == 1) c[i] = j+1;
    }
    if (sumj !=1 ) {
      cout << "Illegal coloring found: vertex v[" << i <<"]"<<endl;
      ret=false;
    }
  }

  list<pair<int,int> >::const_iterator iterator;
  for (iterator = adjacent_list.begin(); iterator != adjacent_list.end(); ++iterator) {
    int u = (*iterator).first;
    int v = (*iterator).second;
    if (c[u] == c[v])
    {
      cout <<"Illegal coloring found: vertex [" << u <<"] and vertex [" <<v <<"] are colored by one number "<<c[u]<<endl;
      ret = false;
    }
  }

  return ret;
}

void loadAdjaList(int &n, list <pair<int,int> > &adjacent_list)
{
  cin >> n;
  string line;
  int lineCnt=-1;
  for(int i=0;i<n+1;i++)
  {
    std::getline(cin, line);
    istringstream iss(line);
    int a;
    while (iss >> a)
    {
      adjacent_list.push_back(make_pair(lineCnt,a));
    }
      lineCnt++;
  }

  list<pair<int,int> >::const_iterator iterator;
  for (iterator = adjacent_list.begin(); iterator != adjacent_list.end(); ++iterator) {
    int u = (*iterator).first;
    int v = (*iterator).second;
    cout <<u<<" "<<v<<endl;
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
