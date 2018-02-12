\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 0 x3 + 0 x4 + [ 2 x3 ^2 + 4 x3 * x4 + 4 x4 ^2 ] / 2
Subject To
 c1: x1  = 0
 c2: x2  = 0
Bounds
 0 <= x1 <= 1
 0 <= x2 <= 1
 0 <= x3 <= 1
 0 <= x4 <= 1
Binaries
 x1  x2  x3  x4 
End
