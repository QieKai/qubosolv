// QUBO solver takes a matrix with size as input and generates a optimal solution
// ### Usage:
// $./qubo < matrix.txt
// ### To list all solution found in solution pool:
// $./qubo -a < matrix.txt
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
ILOSTLBEGIN

bool find_all_sol=false;
void read_qubo(int &n, double **&Q);

int main (int argc, char **argv)
{
    int n=0;
    double **Q;
    read_qubo(n,Q);

    if(argc > 1)
    {
        if(std::string(argv[1]) == "-a") find_all_sol = true;
    }

    IloEnv env;
    try {
        IloModel model(env);

        //construct objective function
        IloNumVarArray var(env);
        for (int i = 0; i < n; ++i)
        {
            char buffer [10];
            sprintf (buffer, "x%d", i+1);
            var.add(IloNumVar(env,0,1,ILOBOOL,buffer));
        }
        IloExpr expr(env);
        for (int i=0; i<n; i++)
            for (int j=0; j< n; j++)
                expr += var[i]*var[j]*Q[i][j];

        IloObjective obj = IloMinimize(env,expr);

        //add constraints
        for (int i = 0; i < n; ++i)
        {
            bool reduced=true;
            for(int j=0;j<n;j++)
            {
                if(Q[i][j]!=0 || Q[j][i]!=0)
                {
                    reduced=false;
                    break;
                }
            }
            if(reduced) {
                char buffer [10];
                sprintf (buffer, "c%d", i+1);
                model.add(IloRange(env, 0,var[i], 0,buffer));
            }
        }
        model.add(obj);
        IloCplex cplex(model);
        cplex.exportModel("qubo.lp");

        //do not display any cplex output on the screen
        cplex.setOut(env.getNullStream());
        cplex.setWarning(env.getNullStream());
        cplex.setError(env.getNullStream());

        if(find_all_sol){
            cplex.setParam(IloCplex::SolnPoolGap,0);
            cplex.setParam(IloCplex::SolnPoolIntensity,4);
            cplex.setParam(IloCplex::PopulateLim,2100000000);
            cplex.setParam(IloCplex::Param::OptimalityTarget,3);
            if ( !cplex.populate() ) {
                env.error() << "Failed to optimize QUBO" << endl;
                throw(-1);
            }

            IloNumArray vals(env);
            int numsol = cplex.getSolnPoolNsolns();
            env.out() << "Solution status = " << cplex.getStatus() << endl;
            if(numsol<1) return 0;
            int minsol=(int)cplex.getObjValue(0);
            for (int i = 0; i < numsol; i++)
                if(minsol>(int)cplex.getObjValue(i))
                    minsol=cplex.getObjValue(i);
            env.out() << "Solution value = " << minsol << endl;
            for (int i = 0; i < numsol; i++)
            {
                if((int)cplex.getObjValue(i) == minsol){
                    cplex.getValues(vals, var,i);
                    //env.out() << "Values = "<<endl << vals << endl;
                    env.out() <<"["<<i<< "] Variables = ["; // << vals << endl;
                    for (int j = 0; j<n-1; j++) env.out() << vals[j] << ',';
                    env.out() << vals[n-1] << ']' << endl;
                }
            }
        } else {
            // Optimize the problem and obtain solution.
            if ( !cplex.solve() ) {
                env.error() << "Failed to optimize QUBO" << endl;
                throw(-1);
            }
            IloNumArray vals(env);
            env.out() << "Solution status = " << cplex.getStatus() << endl;
            env.out() << "Solution value  = " << cplex.getObjValue() << endl;
            cplex.getValues(vals, var);
            //env.out() << "Variables = " << vals << endl;
            env.out() <<"Variables = ["; // << vals << endl;
            for (int j = 0; j<n-1; j++) env.out() << vals[j] << ',';
            env.out() << vals[n-1] << ']' << endl;
        }

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


void read_qubo(int &n, double **&Q) {
    // Start of: initializing Q
    string line;
    getline(cin,line);
    std::size_t pos = line.find(" ");
    if(pos!=std::string::npos)
    {
        line = line.substr(0,pos);
    }
    n = atoi(line.c_str());
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
            cin >> Q[i][j];
    }
}
