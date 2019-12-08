pi_lin: src/Linux/main.cc lib/*.hh
	g++ $< -lSDL2 -lSDL2_ttf -o bin/$@

pi_win: src/Windows/main.cc lib/*.hh
	g++ $< -ID:/Libraries/64-bit/include -LD:/Libraries/64-bit/lib -m64 -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o bin/$@