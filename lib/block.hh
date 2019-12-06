#ifndef BLOCK_HH
#define BLOCK_HH

#include <SDL2/SDL.h>

class Block
{
public:
  long int mass;
  float speed = 0.0f;
  Vec2f pos;
  Vec2 size;

  Block() { }
  Block(long int iMass, Vec2f iPos, Vec2 iSize) // Mass in kg
  {
    mass = iMass;
    pos = iPos;
    size = iSize;
  }

  void draw(SDL_Renderer* renderer, byte r, byte g, byte b, byte a)
  {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect block;
    block.x = pos.x;
    block.y = pos.y;
    block.w = size.x;
    block.h = size.y;
    SDL_RenderFillRect(renderer, &block);
  }
};

#endif // BLOCK_HH