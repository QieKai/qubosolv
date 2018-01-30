PLATFORM = mac64
INC      = ./include/
CPP      = g++
CARGS    = -m64 -g
RELEASE := $(shell uname -r | cut -f 1 -d .)
CPPLIB   = -L./lib/ -lgurobi_c++ -lgurobi75

run: run_c++
run_c++: run_qubo_c++	

%_c++: ./examples/c++/%_c++.cpp
	$(CPP) $(CARGS) -o $@ $< -I$(INC) $(CPPLIB) -lm

run_qubo_c++: qubo_c++
	./qubo_c++ qubo.txt

clean:
	rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp *.mps *.dSYM
