#ifndef TWO_OPT_LOCALSEARCH_H
#define TWO_OPT_LOCALSEARCH_H
#include <iostream>
#include <cstdlib>
#define RUNS 1000
double solveQ(double **, const int, int*);
void genRandomInitial(int*, const int);
void printVector(int*, const int);
void printMatrix(double **Q, const int n);
double evaluate(double**, int *, const int);
void calculateGains(double*, double **, double**, int*, const int);
double heuristicSearch(const int n, double**Q);
#endif
