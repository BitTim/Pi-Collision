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

#endif // DATATYPES_HH