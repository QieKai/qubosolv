/* Copyright 2017, Gurobi Optimization, Inc. */

/* This example formulates and solves the following simple QP model:

     minimize    x^2 + x*y + y^2 + y*z + z^2 + 2 x
     subject to  x + 2 y + 3 z >= 4
                 x +   y       >= 1

   It solves it once as a continuous model, and once as an integer model.
*/

#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
using namespace std;
void printMatrix(double **Q, int n)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
            cout << Q[i][j] << " ";
        cout << endl;
    }
}



int main(int argc, char *argv[])
{
    GRBVar *x = 0;
    
    int n=0;
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
            infile >> n;
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
    printMatrix(Q,n);
  try {
    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env);
    // Create variables
      x = model.addVars(n,GRB_BINARY);
    // Set objective
    GRBQuadExpr obj = 0;
    for (int i=0; i<n; i++)
        for (int j=0; j< n; j++)
            obj += x[i]*x[j]*Q[i][j];
    model.setObjective(obj);
    
      // Optimize model
    model.optimize();
      for(int i=0;i<n;i++)
      {
          cout<< x[i].get(GRB_DoubleAttr_X) << " ";
      }
      cout<<endl;
    
    
    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

 

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }
    
    for (int i=0; i<n; i++)
        delete [] Q[i];
    delete [] Q;
  return 0;
}
