tetral: src/Linux/tetra.cc lib/*.hh
	g++ -o bin/Linux/tetra -lSDL2 $<

tetraw: src/Windows/tetra.cc lib/*.hh
	g++ $< -o bin/Windows/tetra -ID:/Libraries/64-bit/include -LD:/Libraries/64-bit/lib -w -Wl,-subsystem,windows -m64 -lSDL2main -lSDL2

seditl: src/Linux/spriteedit.cc lib/*.hh
	g++ -o bin/Linux/spriteedit -lSDL2 $<

peditl: src/Linux/paletteedit.cc lib/*.hh
	g++ -o bin/Linux/paletteedit -lSDL2 $<

gtestl: src/Linux/guitest.cc lib/*.hh
	g++ -o bin/Linux/guitest -lSDL2 $<
