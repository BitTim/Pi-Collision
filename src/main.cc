//PI Collision
//Approximating PI with collisions
//By BitTim

#include <iostream>
#include <cmath>
#include <string>
#include <SDL2/SDL.h>

#include "../lib/datatypes.hh"
#include "../lib/block.hh"
#include "../lib/var.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

Block smallMass;
Block bigMass;

int collisions = 0;
float tmp_pos_x = 0.0f;

// m/s = 100px / 100u

//================================
// Utility functions
//================================

void init(long int arg)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("PI Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES.x, _SCREENRES.y, _FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0x36, 0x36, 0x36, 0xFF);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  smallMass = Block(1, Vec2f(_SCREENRES.x / 3, _SCREENRES.y - 96), Vec2(96, 96));
  bigMass = Block(arg, Vec2f(_SCREENRES.x - _SCREENRES.x / 3, _SCREENRES.y - 128), Vec2(128, 128));
  bigMass.speed = -1.0f; //Speed m/s
  tmp_pos_x = bigMass.pos.x;
}

void end()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

//================================
// Repeating Functions
//================================

void update()
{
  SDL_PollEvent(&event);
  if(event.type == SDL_QUIT) quit = true;

  SDL_Delay(1);

  if(smallMass.pos.x <= 0)
  {
    collisions++;
    smallMass.speed *= -1;
  }

  if(bigMass.pos.x <= smallMass.pos.x + smallMass.size.x)
  {
    collisions++;

    //Conservation of Momentum (Impulserhaltung) pVor = pNach <=> m1 * v1 + m2 * v2 = m1 * u1 + m2 * u2
    float v1 = smallMass.speed;
    float v2 = bigMass.speed;

    smallMass.speed = (smallMass.mass * v1 + bigMass.mass * (2 * v2 - v1)) / (smallMass.mass + bigMass.mass);
    bigMass.speed = (bigMass.mass * v2 + smallMass.mass * (2 * v1 - v2)) / (smallMass.mass + bigMass.mass);
  }

  smallMass.pos.x += smallMass.speed / 10.0f;
  bigMass.pos.x += bigMass.speed / 10.0f;
  tmp_pos_x += bigMass.speed / 10.0f;

  if(smallMass.pos.x < 0)
  {
    smallMass.pos.x = 0.0f;
    if(tmp_pos_x <= smallMass.size.x) bigMass.pos.x = smallMass.size.x;
  }
}

void draw()
{
  SDL_SetRenderDrawColor(renderer, 0x36, 0x36, 0x36, 0xFF);
  SDL_RenderClear(renderer);

  smallMass.draw(renderer, 128, 128, 255, 255);
  bigMass.draw(renderer, 255, 128, 128, 255);

  printf("Collisions: %d\n", collisions);

  SDL_RenderPresent(renderer);
}

//================================
// Main
//================================

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
   printf("Nutzung: ./bin/pi [MODUS] [ARG]\n\nModi: -pi     ARG wird zur Anzahl der Nachkommastellen\n      -m      ARG wird zur Masse in kg\n");
   return 0;
  }

  long int argument = 0;

  if(argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == 'i') argument = pow(100, std::stoi(argv[2]));
  else if(argv[1][0] == '-' && argv[1][1] == 'm') argument = std::stoi(argv[2]);
  else
  {
    printf("%s ist kein valider Modus\n", argv[1]);
    return 0;
  }

  init(argument);
  
  while(!quit)
  {
    update();
    draw();
  }
  
  end();

  return 0;
}