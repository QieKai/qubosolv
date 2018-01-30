\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: x1 + 2 x2 + 3 x3 + 4 x4 + x5 + 2 x6 + 3 x7 + 4 x8 + x9 + 2 x10 + 3 x11
      + 4 x12 + x13 + 2 x14 + 3 x15 + 4 x16
Subject To
 c1:  x1 + x2 + x3 + x4  = 1
 c2:  x5 + x6 + x7 + x8  = 1
 c3:  x9 + x10 + x11 + x12  = 1
 c4:  x13 + x14 + x15 + x16  = 1
 c5:  x1 + x5 <= 1
 c6:  x2 + x6 <= 1
 c7:  x3 + x7 <= 1
 c8:  x4 + x8 <= 1
 c9:  x1 + x13 <= 1
 c10: x2 + x14 <= 1
 c11: x3 + x15 <= 1
 c12: x4 + x16 <= 1
 c13: x1 + x5 <= 1
 c14: x2 + x6 <= 1
 c15: x3 + x7 <= 1
 c16: x4 + x8 <= 1
 c17: x5 + x9 <= 1
 c18: x6 + x10 <= 1
 c19: x7 + x11 <= 1
 c20: x8 + x12 <= 1
Bounds
 0 <= x1 <= 1
 0 <= x2 <= 1
 0 <= x3 <= 1
 0 <= x4 <= 1
 0 <= x5 <= 1
 0 <= x6 <= 1
 0 <= x7 <= 1
 0 <= x8 <= 1
 0 <= x9 <= 1
 0 <= x10 <= 1
 0 <= x11 <= 1
 0 <= x12 <= 1
 0 <= x13 <= 1
 0 <= x14 <= 1
 0 <= x15 <= 1
 0 <= x16 <= 1
Binaries
 x1  x2  x3  x4  x5  x6  x7  x8  x9  x10  x11  x12  x13  x14  x15  x16 
End
