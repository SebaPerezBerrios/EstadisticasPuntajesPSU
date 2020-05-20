CXX = g++
CXXFLAGS = -g3 -Wall -lm
MKDIR = mkdir -p

LIBS = -lm

directorios:
	$(MKDIR) build dist

main.o: directorios main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o build/main.o

all: clean main.o
	$(CXX) $(CXXFLAGS) $(LIBS) -o dist/programa build/main.o
	rm -fr build

clean:
	rm -fr *.o a.out core programa dist build

.DEFAULT_GOAL := all
