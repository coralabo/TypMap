FLAG=-DEXE
export LD_LIBRARY_PATH=/usr/local/lib
g++ -o mcq $FLAG -g  CGRA.cpp  DFG.cpp  main.cpp    TEC.cpp  tool.cpp mcqd.cpp \
  -lm -std=c++11 -lgflags -L/usr/lib/x86_64-linux-gnu/   -lglog -L/usr/local/lib  -lpthread 
  