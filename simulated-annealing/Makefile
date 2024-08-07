# Based on CS35 Lab 02 Makefile
# Configure compiler settings
CXX=clang++
CXXFLAGS=-gdwarf-4 -std=c++17 -Werror -D_GLIBCXX_DEBUG
# The header files for the program
HFILES = $(shell ls -1 *.h)
# unittest++ keeps its object files in this directory.
UNITTEST_LIB = -lUnitTest++

all: simAnneal sandbox #testMaj

# This target builds your main program.
simAnneal: main.o tree.o node.o randomGenerator.o maj.o treeBuilders.o
	$(CXX) $(CXXFLAGS) -o $@ main.o tree.o node.o randomGenerator.o maj.o treeBuilders.o

# This target builds the test program.
sandbox: sandbox.o randomGenerator.o tree.o node.o maj.o treeBuilders.o
	$(CXX) $(CXXFLAGS) -o $@ sandbox.o randomGenerator.o tree.o node.o maj.o treeBuilders.o

#uncomment when needed
#testMaj: maj.o testMaj.o
#	$(CXX) $(CXXFLAGS) -o $@ testMaj.o maj.o

# This target describes how to compile a .o file from a .cpp file.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# This target deletes the temporary files we have built.
.PHONY: clean all

clean:
	rm -f *.o
	rm -f simAnneal main testMaj sandbox
	rm -f *.aux *.log *.fdb_latexmk *.fls *.synctex.gz
