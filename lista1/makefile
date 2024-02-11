.PHONY all:
all: clean fifteen graph run

fifteen: fifteen.o graph.o
	g++ -Wall -Wextra -pedantic -O2 -o fifteen fifteen.o graph.o

fifteen.o: fifteen.cpp
	g++ -c fifteen.cpp

graph.o: graph.cpp
	g++ -c graph.cpp

.PHONY run:
run: fifteen
	./fifteen

.PHONY clean:
clean:
	rm -f *.o *~
