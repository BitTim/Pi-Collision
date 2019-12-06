pi: src/main.cc lib/*.hh
	g++ $< -lSDL2 -o bin/$@