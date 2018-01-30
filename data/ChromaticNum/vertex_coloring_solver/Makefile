SYSTEM     = x86-64_osx
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /Applications/CPLEX_Studio128/cplex
CONCERTDIR    = /Applications/CPLEX_Studio128/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = clang++ -O0
CC  = clang -O0

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -stdlib=libc++
COPT  = -m64 -fPIC

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libc++
CLNFLAGS  = -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit



run: 
	make lp_chromatic
	./lp_chromatic ./lp_chromatic.alist

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

EXDIR         = $(CPLEXDIR)/examples
EXDATA        = $(EXDIR)/data
EXSRCCPP      = $(EXDIR)/src/cpp

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 
	 
	 
# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~
	/bin/rm -rf lp_chromatic
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#
lp_chromatic: lp_chromatic.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o lp_chromatic lp_chromatic.o $(CCLNFLAGS)
lp_chromatic.o: ./lp_chromatic.cpp
	$(CCC) -c $(CCFLAGS) ./lp_chromatic.cpp -o lp_chromatic.o



# Local Variables:
# mode: makefile
# End:
