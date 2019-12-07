#ifndef DATATYPES_HH
#define DATATYPES_HH

typedef unsigned char byte;

struct Vec2
{
  int x;
  int y;

  Vec2() { }
  Vec2(int iX, int iY)
  {
    x = iX;
    y = iY;
  }
};

struct Vec2f
{
  float x;
  float y;

  Vec2f() { }
  Vec2f(float iX, float iY)
  {
    x = iX;
    y = iY;
  }
};

SDL_Rect iSDL_Rect(int x, int y, int w, int h)
{
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;

  return r;
}

SDL_Color iSDL_Color(int r, int g, int b, int a)
{
  SDL_Color c;
  c.r = r;
  c.g = g;
  c.b = b;
  c.a = a;

  return c;
}

#endif // DATATYPES_HH