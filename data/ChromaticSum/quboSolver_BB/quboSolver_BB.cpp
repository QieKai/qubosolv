//  quboSolver_B&B.cpp
//
//  Created by Kai Liu on 12/10/17.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <list>
#include <ctime>
#include <sys/time.h>
#include "two_opt_localSearch.h"
//#include <omp.h>
//#define LOG_PRINT
using namespace std;

struct branch_Node{
    int level;
    double* lb_pds; // partial derivative lower bound
    double* ub_pds; //partial derivative upper bound
    double lowerBound;
};

void optimizer(double **Q, const int qSize, int X[], double &currentOPT);
double updateLowerBound(const double initBound, int level, int p[], double** Q, int incumX[], const int qSize);
void initRangeOfGradient(double **Q, const int qSize, double* ub_pds,double* lb_pds);
void updateMinRangeOfGradient(double **Q, const int qSize, int incumX[],int level, int p[], double ub_pds[],double lb_pds[]);
int checkFixedSignVar(const int qSize, int level, int p[], double* ub_pds,double* lb_pds);
double matrixMulti(double **Q,int x[], int qSize);

int main(int argc, char *argv[])
{
    int n=0;
    double **Q;
    int* X;
    double currentOPT=0;
    if (argc != 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else
    {
        ifstream infile( argv[1] );
        if (infile.is_open()==0)
            cout << "Could not open file" << endl;
        else
        {
            infile >> n;
            // Start of: initializing Q
            Q = new double*[n];
            X = new int[n];
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
            
            currentOPT = heuristicSearch(n,Q);

            struct timeval start, end;
            gettimeofday(&start, NULL);
            optimizer(Q,n,X,currentOPT);
            gettimeofday(&end, NULL);

            cout<<"Find optimal solution -> "<<currentOPT<<endl;
            for(int i=0;i<n;i++)cout<<" "<<X[i];
            cout<<endl;
//#ifdef LOG_PRINT
            cout <<"Time used:" << end.tv_sec-start.tv_sec <<" seconds"<<endl;
//#endif
        }
        infile.close();
    }
    for (int i=0; i<n; i++)
        delete [] Q[i];
    delete [] Q;
    delete [] X;
    return 0;
}

double updateLowerBound(const double initBound, int level, int p[], double** Q, int incumX[], const int qSize)
{
#ifdef LOG_PRINT
    cout<<"update lower bound"<<endl;
#endif
    double lowerbound  = initBound;

    for(int i=0;i<level;i++)
    {
        #pragma omp parallel for
        for(int j=i+1;j<qSize;j++)
        {
            if(incumX[p[i]] ==0 && Q[p[i]][p[j]]<0)
                lowerbound = lowerbound - Q[p[i]][p[j]]*2;
        }
    }
    for(int i=0;i<level;i++)
    {
        if(incumX[p[i]] ==0 && Q[p[i]][p[i]]<0)
            lowerbound = lowerbound - Q[p[i]][p[i]];
    }
    for(int i=0;i<level;i++)
    {
        for(int j=0;j<level;j++)
            if(incumX[p[i]] ==1 && incumX[p[j]] ==1 && Q[p[i]][p[j]]>0)
                lowerbound = lowerbound + Q[p[i]][p[j]];
    }
#ifdef LOG_PRINT
    cout<<"     current lower bound -> "<<lowerbound<<endl;
#endif
    return lowerbound;
}

void initRangeOfGradient(double **Q, const int qSize, double* ub_pds,double* lb_pds)
{
#ifdef LOG_PRINT
    cout<<endl<<"initialize ranges of gradient"<<endl;
    cout<<"    lower bound    upper bound"<<endl;
#endif
    double up=0.0,lb=0.0;
    for(int i=0;i<qSize;i++)
    {
        for(int j=0;j<qSize;j++)
        {
            if(i!=j)
            {
                if(Q[i][j]<0)lb+=Q[i][j];
                else up+=Q[i][j];
            }
            else
            {
                lb=lb+Q[i][j]/2;
                up=up+Q[i][j]/2;
            }
        }
        ub_pds[i] = up;
        lb_pds[i] = lb;
#ifdef LOG_PRINT
        cout<<"    "<<lb_pds[i]<<"              "<<ub_pds[i]<<endl;
#endif
        up=0.0;
        lb=0.0;
    }
}

void updateMinRangeOfGradient(double **Q, const int qSize, int incumX[],int level, int p[], double ub_pds[],double lb_pds[])
{
#ifdef LOG_PRINT
    cout<<"update minimum range of the gradient "<<endl;
    cout<<"    lower bound    upper bound"<<endl;
#endif
    if(incumX[p[level-1]] == 1)
    {
        for(int i=level;i<qSize;i++)
        {
            if(Q[p[i]][p[level-1]] > 0) lb_pds[p[i]] = lb_pds[p[i]] + Q[p[i]][p[level-1]];
            if(Q[p[i]][p[level-1]] < 0) ub_pds[p[i]] = ub_pds[p[i]] + Q[p[i]][p[level-1]];
        }
    }
    else
    {
        for(int i=level;i<qSize;i++)
        {
            if(Q[p[i]][p[level-1]] < 0) lb_pds[p[i]] = lb_pds[p[i]] - Q[p[i]][p[level-1]];
            if(Q[p[i]][p[level-1]] > 0) ub_pds[p[i]] = ub_pds[p[i]] - Q[p[i]][p[level-1]];
        }
    }
#ifdef LOG_PRINT
    for(int i=level;i<qSize;i++)
        cout<<"    "<<lb_pds[p[i]]<<"              "<<ub_pds[p[i]]<<endl;
#endif
}

int checkFixedSignVar(const int qSize, int level, int p[], double* ub_pds,double* lb_pds)
{
#pragma omp parallel for
    for(int i=level;i<qSize;i++)
    {
        if(ub_pds[p[i]]<=0 || lb_pds[p[i]]>=0 ) return i;
    }
    return -1;
}

double matrixMulti(double **Q,int x[], int qSize)
{
    double sum=0;
    
    for (int i=0; i<qSize; i++)
        for (int j=0; j<qSize; j++)
            sum += x[i]*x[j]*Q[i][j];
    return sum;
}

void optimizer(double **Q, const int qSize, int X[], double &currentOPT)
{
    stack<branch_Node> bb_nodes;
    struct branch_Node vertice = {-1,NULL,NULL,0};
    bb_nodes.push(vertice);
    double lowerbound;
    double *ub_pds,*lb_pds;//the lower bound and uper bound of X's partial deratives
    int level=0;    //the current level of the branch and bound tree
    int NSUBP=0;    //the number of subproblems solved
    int incumX[qSize];   //the incumbent minimizer
    int p[qSize];   //the permutation vector
    
    //initialize permutaion vector with order 0,1,2...
    for(int i=0;i<qSize;i++) p[i] = i;
    
    //initialize ranges of gradient
    ub_pds = new double[qSize];
    lb_pds = new double[qSize];
    initRangeOfGradient(Q, qSize, ub_pds, lb_pds);

#ifdef LOG_PRINT
    //initialize lower bound
    cout<<"initialize lower bound"<<endl;
#endif
    double initLB=0.0;
    for(int i=0;i<qSize;i++)
    {
        for(int j=0;j<qSize;j++)
            if(Q[i][j]<0)initLB+=Q[i][j];
    }
#ifdef LOG_PRINT
    cout<<"     current lower bound -> "<<initLB<<endl;
#endif
    //start a branch and bound tree
    while(level!=-1)
    {
#ifdef LOG_PRINT
        cout<<endl<<"NSUBP -> "<<NSUBP<<endl;
        cout<<"Current level: "<<level<<endl;
#endif
        if(level==0)
            lowerbound =initLB;
        else
            lowerbound = updateLowerBound(initLB, level, p, Q, incumX, qSize);
        
        //check if pruning is required
        if(lowerbound>currentOPT || level == qSize)
        {
            if(lowerbound<=currentOPT)
            {
                double newOPT = matrixMulti(Q,incumX,qSize);
#ifdef LOG_PRINT
                cout<<"Find new solution -> "<<newOPT<<endl;
#endif
                if(newOPT<=currentOPT)
                {
                    
                    currentOPT = newOPT;
#ifdef LOG_PRINT
                    cout<<"optimal solution updated to -> "<<currentOPT<<endl;
#endif
#pragma omp parallel for
                    for(int i=0;i<qSize;i++)
                    {
                        X[i]=incumX[i];
#ifdef LOG_PRINT
                        cout<<" "<<X[i];
#endif
                    }
#ifdef LOG_PRINT
                    cout<<endl;
#endif

                }
            }
            
            //ub_pds[level] = bb_nodes.top().level;
            struct branch_Node nodePopped = bb_nodes.top();
            level = nodePopped.level;
            if(level>0)
            {
                lowerbound = nodePopped.lowerBound;
                #pragma omp parallel for
                for(int i=0;i<qSize;i++)
                {
                    ub_pds[i] = nodePopped.ub_pds[i];
                    lb_pds[i] = nodePopped.lb_pds[i];
                }
                incumX[p[level-1]] = 1-incumX[p[level-1]];
#ifdef LOG_PRINT
                cout<<"pop node:"<<endl;
                cout<<"     level -> "<<nodePopped.level;
                cout<<"     lower bound -> "<<nodePopped.lowerBound<<endl;
                cout<<"     lower bound    upper bound"<<endl;
                #pragma omp parallel for
                for(int i=0;i<qSize;i++)
                    cout<<"     "<<nodePopped.lb_pds[i]<<"              "<<nodePopped.ub_pds[i]<<endl;
#endif
                delete[] nodePopped.ub_pds;
                delete[] nodePopped.lb_pds;
                bb_nodes.pop();
            }
            NSUBP++;
        }
        else //depth-first branch
        {
            if(level!=0)
            {
                updateMinRangeOfGradient(Q, qSize, incumX, level, p, ub_pds,lb_pds);
            }
            //if any free varaible can be forced to 0 or 1, return the index of permutation vector, otherwise return -1

            int pIdx = checkFixedSignVar(qSize,level,p,ub_pds,lb_pds);
            if(pIdx<0)
            {
#ifdef LOG_PRINT
                cout<<"Cannot find any free variable can be forced"<<endl;
#endif
                int idx = level;
                double max=ub_pds[p[level]] < -lb_pds[p[level]] ?ub_pds[p[level]]:-lb_pds[p[level]];
                for(int i = level;i<qSize;i++)
                {
                    double min = ub_pds[p[i]] < -lb_pds[p[i]] ?ub_pds[p[i]]:-lb_pds[p[i]];
                    if(max < min)
                    {
                        max = min;
                        idx = i;
                    }
                }
#ifdef LOG_PRINT
                cout<<"Choose variable X["<<idx+1<<"] to branch to next"<<endl;
#endif
                int temp = p[level];
                p[level] = p[idx];
                p[idx] = temp;
                
                double lLB,rLB;
                incumX[p[level]] = 0;
                lLB = updateLowerBound(initLB, level+1, p, Q, incumX, qSize);
                incumX[p[level]] = 1;
                rLB = updateLowerBound(initLB, level+1, p, Q, incumX, qSize);
                if(lLB < rLB)
                    incumX[p[level]] = 0;
                level++;
                
                struct branch_Node node;
                node.ub_pds = new double[qSize];
                node.lb_pds = new double[qSize];
                #pragma omp parallel for
                for(int i=0;i<qSize;i++)
                {
                    node.ub_pds[i] = ub_pds[i];
                    node.lb_pds[i] = lb_pds[i];
                }
                node.level = level;
                if(lLB < rLB) node.lowerBound = rLB;
                else node.lowerBound = lLB;
                bb_nodes.push(node);
#ifdef LOG_PRINT
                cout<<"Push node to Stack:"<<endl;
                cout<<"     level -> "<<node.level;
                cout<<"     lower bound -> "<<node.lowerBound<<endl;
                cout<<"     lower bound    upper bound"<<endl;
                #pragma omp parallel for
                for(int i=0;i<qSize;i++)
                    cout<<"     "<<node.lb_pds[i]<<"              "<<node.ub_pds[i]<<endl;
#endif
            }
            else
            {
                
                if(ub_pds[p[pIdx]] <=0) incumX[p[pIdx]] = 1;
                else incumX[p[pIdx]] = 0;
#ifdef LOG_PRINT
                cout<<"Find free variable x["<<p[pIdx]+1<<"] can be forced to "<<incumX[p[pIdx]]<<endl;
#endif
                int temp = p[level];
                p[level] = p[pIdx];
                p[pIdx] = temp;
                level++;
            }
#ifdef LOG_PRINT
            cout<<"updated permutation arrary is:"<<endl;
#pragma omp parallel for
            for(int i=0;i<qSize;i++) cout<<" "<<p[i]+1;
            cout<<endl;
            cout<<"Fixed variables:"<<endl;
            #pragma omp parallel for
            for(int i=0;i<level;i++)
            {
                cout<<"  "<<"X["<<p[i]+1<<"]->"<<incumX[p[i]]<<"  ";
            }
            cout<<endl;
#endif
        }
    }
#ifdef LOG_PRINT
    cout<<NSUBP<<" nodes have been explored"<<endl;
#endif
    delete []ub_pds;
    delete []lb_pds;
}




