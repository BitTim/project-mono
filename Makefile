tetra_linux: src/Linux/tetra.cc lib/Linux/*.hh
	g++ -o bin/Linux/tetra -lSDL2 $<
