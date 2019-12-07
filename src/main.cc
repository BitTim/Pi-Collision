//PI Collision
//Approximating PI with collisions
//By BitTim

#include <iostream>
#include <cmath>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/datatypes.hh"
#include "../lib/block.hh"
#include "../lib/var.hh"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

TTF_Font* font;
SDL_Surface* tmp_surface;
SDL_Texture* smallMassText;
SDL_Texture* bigMassText;
SDL_Texture* collisionCounter;
SDL_Texture* smallMassSpeedText;
SDL_Texture* bigMassSpeedText;
SDL_Texture* distanceText;
SDL_Color fg;

Block smallMass;
Block bigMass;

int collisions = 0;
float tmp_pos_x = 0.0f;

bool pi_mode;
char tmp_str[64];

// m/s = 100px / 100u

//================================
// Utility functions
//================================

void init(long int arg, float speed)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("PI Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _SCREENRES.x, _SCREENRES.y, _FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  TTF_Init();
  font = TTF_OpenFont("dat/Minimal.ttf", 32);

  SDL_SetRenderDrawColor(renderer, 0x36, 0x36, 0x36, 0xFF);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  smallMass = Block(1, Vec2f(_SCREENRES.x / 3, _SCREENRES.y - 96), Vec2(96, 96));
  bigMass = Block(pi_mode ? pow(100, arg) : arg, Vec2f(_SCREENRES.x - _SCREENRES.x / 3, _SCREENRES.y - 128), Vec2(128, 128));
  bigMass.speed = -speed; //Speed m/s
  tmp_pos_x = bigMass.pos.x;

  fg.r = 0xff;
  fg.g = 0xff;
  fg.b = 0xff;
  fg.a = 0xff;

  sprintf(tmp_str, "%dkg", smallMass.mass);
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, fg);
  smallMassText = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  if(!pi_mode) sprintf(tmp_str, "%dkg", bigMass.mass);
  else sprintf(tmp_str, "100^%dkg", arg);
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, fg);
  bigMassText = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  SDL_FreeSurface(tmp_surface);
}

void end()
{
  TTF_CloseFont(font);
  TTF_Quit();

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

  SDL_DestroyTexture(smallMassSpeedText);
  SDL_DestroyTexture(bigMassSpeedText);
  SDL_DestroyTexture(collisionCounter);
  SDL_DestroyTexture(distanceText);

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

  sprintf(tmp_str, "Kollisionen: %d", collisions);
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, iSDL_Color(128, 255, 128, 255));
  collisionCounter = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  sprintf(tmp_str, "Geschw. Blau: %02f", smallMass.speed);
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, iSDL_Color(128, 128, 255, 255));
  smallMassSpeedText = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  sprintf(tmp_str, "Geschw. Rot: %02f", bigMass.speed);
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, iSDL_Color(255, 128, 128, 255));
  bigMassSpeedText = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  sprintf(tmp_str, "Distanz: %02f", bigMass.pos.x - (smallMass.pos.x + smallMass.size.x));
  tmp_surface = TTF_RenderUTF8_Blended(font, tmp_str, fg);
  distanceText = SDL_CreateTextureFromSurface(renderer, tmp_surface);

  SDL_FreeSurface(tmp_surface);
}

void draw()
{
  SDL_SetRenderDrawColor(renderer, 0x36, 0x36, 0x36, 0xFF);
  SDL_RenderClear(renderer);

  smallMass.draw(renderer, 128, 128, 255, 255);
  bigMass.draw(renderer, 255, 128, 128, 255);

  int tmp_w = 0, tmp_h = 0;
  SDL_Rect tmp_rect;

  SDL_QueryTexture(smallMassText, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(smallMass.pos.x, smallMass.pos.y, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, smallMassText, NULL, &tmp_rect);
  
  SDL_QueryTexture(bigMassText, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(bigMass.pos.x, bigMass.pos.y, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, bigMassText, NULL, &tmp_rect);

  SDL_QueryTexture(collisionCounter, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(_SCREENRES.x / 2, 0, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, collisionCounter, NULL, &tmp_rect);

  SDL_QueryTexture(smallMassSpeedText, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(0, 0, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, smallMassSpeedText, NULL, &tmp_rect);

  SDL_QueryTexture(bigMassSpeedText, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(0, tmp_h, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, bigMassSpeedText, NULL, &tmp_rect);

  SDL_QueryTexture(distanceText, NULL, NULL, &tmp_w, &tmp_h);
  tmp_rect = iSDL_Rect(0, tmp_h * 2, tmp_w, tmp_h);
  SDL_RenderCopy(renderer, distanceText, NULL, &tmp_rect);

  SDL_RenderPresent(renderer);
}

//================================
// Main
//================================

int main(int argc, char* argv[])
{
  if(argc < 4)
  {
   printf("Nutzung: ./bin/pi [MODUS] [ARG] [GESCHW]\n\nModi: -pi     ARG wird zur Anzahl der Nachkommastellen\n      -m      ARG wird zur Masse in kg\nGeschwindigkeit: wird in m/s angegeben\n");
   return 0;
  }

  long int argument = 0;

  if(argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == 'i')
  {
    argument = std::stoi(argv[2]);
    pi_mode = true;
  }
  else if(argv[1][0] == '-' && argv[1][1] == 'm') argument = std::stoi(argv[2]);
  else
  {
    printf("%s ist kein valider Modus\n", argv[1]);
    return 0;
  }

  init(argument, std::atof(argv[3]));
  
  while(!quit)
  {
    update();
    draw();
  }
  
  end();

  return 0;
}