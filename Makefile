tetra_linux: src/Linux/tetra.cc lib/*.hh
	g++ -o bin/Linux/tetra -lSDL2 $<

tetra_win: src/Windows/tetra.cc lib/*.hh
	g++ $< -o bin/Windows/tetra -ID:/Libraries/64-bit/include -LD:/Libraries/64-bit/lib -w -Wl,-subsystem,windows -m64 -lSDL2main -lSDL2

spriteedit_linux: src/Linux/spriteedit.cc lib/*.hh
	g++ -o bin/Linux/spriteedit -lSDL2 $<

paletteedit_linux: src/Linux/paletteedit.cc lib/*.hh
	g++ -o bin/Linux/paletteedit -lSDL2 $<

guitest_linux: src/Linux/guitest.cc lib/*.hh
	g++ -o bin/Linux/guitest -lSDL2 $<
