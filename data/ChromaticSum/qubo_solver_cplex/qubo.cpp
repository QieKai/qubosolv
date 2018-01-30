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
ILOSTLBEGIN

//int objective_function(IloNumArray vals, int n,int k);
int
main (int argc, char **argv)
{
    int n=0, k=0;

    double **Q;

    if (argc != 2)
        cout << "Correct usage: " << argv[0] <<" <filename>" << endl;
    else
    {
        ifstream infile( argv[1] );
        if (infile.is_open()==0)
            cout << "Could not open file" << endl;
        else
        {
            infile >> n >>k;
            // Start of: initializing Q
            Q = new double*[n];
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
        }
    }


    IloEnv   env;
    try {
        IloModel model(env);

        IloNumVarArray var(env);

        for (int i = 0; i < n; ++i)
            var.add(IloNumVar(env,0,1,ILOBOOL));

        IloExpr expr(env);
        for (int i=0; i<n; i++)
            for (int j=0; j< n; j++)
                expr += var[i]*var[j]*Q[i][j];

        IloObjective obj = IloMinimize(env,expr);

        model.add(obj);
        IloCplex cplex(model);
        //cplex.exportModel("lpex1.lp");
        cplex.setOut(env.getNullStream());
        // Optimize the problem and obtain solution.
        if ( !cplex.solve() ) {
            env.error() << "Failed to optimize LP" << endl;
            throw(-1);
        }

        IloNumArray vals(env);
        cplex.getValues(vals, var);
        int sl = 0;
        for (int i = 0; i < n/k; i++)
        {
          for (int j = 0; j < k; j++)
          {
              sl = sl + (int)vals[i*k+j] * (j+1);
          }
        }
        // for (int i = 0; i < n; i++)
        //   cout <<(int)vals[i] <<" ";

        env.out() << "Solution status = " << cplex.getStatus() << endl;
        cout << "Solution value  = " << sl << endl;


        env.out() << "Values        = " << vals << endl;
        //env.out() << "Reduced Costs = " << vals << endl;
    }
    catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }

    env.end();
    for (int i=0; i<n; i++)
        delete [] Q[i];
    delete [] Q;
    return 0;
}  // END main
