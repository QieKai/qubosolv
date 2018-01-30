// -------------------------------------------------------------- -*- C++ -*-
// File: ilolpex1.cpp
// Version 12.7.0
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
// 5725-A06 5725-A29 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5655-Y21
// Copyright IBM Corporation 2000, 2016. All Rights Reserved.
//
// US Government Users Restricted Rights - Use, duplication or
// disclosure restricted by GSA ADP Schedule Contract with
// IBM Corp.
// --------------------------------------------------------------------------


#include <ilcplex/ilocplex.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <list>
#include <sstream>
#include <string>

ILOSTLBEGIN

void loadAdjaList(double **&Q,string filename, int &n, int &k, list <pair<int,int> > &adjacent_list);

int main (int argc, char **argv)
{   
    int n=0,k=0;
    double **Q;
    list <pair<int,int> > adjacent_list;
    
    if (argc != 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else
        loadAdjaList(Q,argv[1],n,k,adjacent_list);
    const int N = n*k;
    IloEnv   env;
    try {
        IloModel model(env);
        
        IloNumVarArray var(env);
        IloRangeArray con(env);
        
        for (int i = 0; i < N; ++i)
            var.add(IloNumVar(env,0,1,ILOBOOL));
        
        IloExpr expr(env);
        //f(x) = sum j*x[i,j]
        for (int i=0; i<n; i++)
            for (int j=0; j<k; j++)
            {
                int idx = i*k+j;
                expr += var[idx]*(j+1);
            }
        
        IloObjective obj = IloMinimize(env,expr);
        
        IloExprArray expr_constrs = IloExprArray(env,n);

        for (int i=0; i<n; i++)
        {
            //IloExpr expr_constrs(env);
            expr_constrs[i] = IloExpr(env);
            for (int j=0; j<k; j++)
            {
                int idx = i*k+j;
                expr_constrs[i] += var[idx];
            }
            
            //IloRange r1(env, 1,expr_constrs, 1);
            con.add(expr_constrs[i]==1);
        }
        
        //p2(x) = sum(E(G))sum(j) x[u,j]+x[v,j]<=1
        list<pair<int,int> >::const_iterator iterator;
        
        for (iterator = adjacent_list.begin(); iterator != adjacent_list.end(); ++iterator) {
            int u = (*iterator).first;
            int v = (*iterator).second;
            //IloExpr expr_constrs(env);
            for(int j=0; j<k; j++)
            {
                //expr_constrs = var[u*k+j]+var[v*k+j];
                //IloRange r1(env, 0,expr_constrs, 1);
                //con.add(r1)
                con.add(var[u*k+j]+var[v*k+j]<=1);
            }
        }
        
        model.add(obj);
        model.add(con);
        IloCplex cplex(model);
        cplex.exportModel("lpe.lp");
        cplex.setOut(env.getNullStream());
        // Optimize the problem and obtain solution.
        if ( !cplex.solve() ) {
            env.error() << "Failed to optimize LP" << endl;
            throw(-1);
        }
        
        IloNumArray vals(env);
        env.out() << "Solution status = " << cplex.getStatus() << endl;
        env.out() << "Solution value  = " << cplex.getObjValue() << endl;
        cplex.getValues(vals, var);
        env.out() << "Values = " << vals << endl;
        //env.out() << "Reduced Costs = " << vals << endl;
    }
    catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    
    env.end();

    return 0;
}  // END main

void loadAdjaList(double **&Q,string filename, int &n, int &k, list <pair<int,int> > &adjacent_list)
{
    ifstream infile( filename );
    
    //cout<<filename<<endl;
    if (infile.is_open()==0)
        cout << "Could not open file" << endl;
    else
    {
        infile >> n;
        k = n; //infile >> k;
        
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
    }
}

