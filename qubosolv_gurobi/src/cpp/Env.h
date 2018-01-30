// Copyright (C) 2016, Gurobi Optimization, Inc.
// All Rights Reserved
#ifndef _ENV_CPP_H_
#define _ENV_CPP_H_


class GRBEnv
{
  private:

    GRBenv*  env;
    GRBenv** envP;

// only for gurobi_c++.h const GRBEnv& operator=(const GRBEnv &xenv);

    GRBEnv(GRBenv *Cenv);

  public:

    friend class GRBModel;

    GRBEnv();
    GRBEnv(const std::string& logfilename);
    GRBEnv(const std::string& logfilename, const std::string& computeservers, int port,
           const std::string& password, int priority, double timeout);
    GRBEnv(const std::string&, const std::string&, const std::string&, const std::string&);
    GRBEnv(const std::string&, const std::string&, const std::string&, int, const std::string&);
    ~GRBEnv();
    void message(const std::string& msg);
    int get(GRB_IntParam param) const;
    double get(GRB_DoubleParam param) const;
    const std::string get(GRB_StringParam param) const;
    void set(GRB_IntParam param, int newvalue);
    void set(GRB_DoubleParam param, double newvalue);
    void set(GRB_StringParam param, const std::string& newvalue);
    void set(const std::string& paramname, const std::string& newvalue);
    void getParamInfo(GRB_DoubleParam param, double* valP,
                      double* minP, double* maxP, double* defP);
    void getParamInfo(GRB_IntParam param, int* valP, int* minP,
                      int* maxP, int* defP);
    void getParamInfo(GRB_StringParam param, std::string& value,
                      std::string& defvalue);
    void resetParams();
    void writeParams(const std::string& paramfile);
    void readParams(const std::string& paramfile);
    const std::string getErrorMsg() const;
};
#endif
