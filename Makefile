pi: src/main.cc lib/*.hh
	g++ $< -lSDL2 -lSDL2_ttf -o bin/$@
