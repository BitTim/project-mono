tetra_linux: src/Linux/tetra.cc lib/Linux/*.hh
	g++ -o bin/Linux/tetra -lSDL2 $<

spriteedit_linux: src/Linux/spriteedit.cc lib/Linux/*.hh
	g++ -o bin/Linux/spriteedit -lSDL2 $<

spriteview_linux: src/Linux/spriteview.cc lib/Linux/*.hh
	g++ -o bin/Linux/spriteview -lSDL2 $<