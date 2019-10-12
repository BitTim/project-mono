mono_linux: src/Linux/mono.cc lib/Linux/*.hh
	g++ -o bin/Linux/mono -lSDL2 $<
