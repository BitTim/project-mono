tetra_linux: src/Linux/tetra.cc lib/*.hh
	g++ -o bin/Linux/tetra -lSDL2 $<

tetra_win: src/Windows/tetra.cc lib/*.hh
	g++ $< -o bin/Windows/tetra -ID:/Libraries/64-bit/include -LD:/Libraries/64-bit/lib -w -Wl,-subsystem,windows -m64 -lSDL2main -lSDL2

spriteedit_linux: src/Linux/spriteedit.cc lib/*.hh
	g++ -o bin/Linux/spriteedit -lSDL2 $<

spriteview_linux: src/Linux/spriteview.cc lib/*.hh
	g++ -o bin/Linux/spriteview -lSDL2 $<