#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <cmath>

int main(int argc,char* args[]){
  
  //Get it going
  SDL_Init(SDL_INIT_VIDEO);
  
  //This is the windows
  SDL_Surface* win = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
  
  //Set the window title
  SDL_WM_SetCaption("Red Roids",NULL);
  
  //Game loop
  SDL_Event event;
  bool inloop=true;
  while(inloop){
    while(SDL_PollEvent(&event)){
      if(event.type==SDL_QUIT) inloop=false;
    }
    
    lineRGBA(win,rand()%800,rand()%600,rand()%800,rand()%600,rand()%256,rand()%256,rand()%256,255);
    
    //Swap double buffer
    SDL_Flip(win);
  }
  
  
  //Clean up
  SDL_Quit();
  
  //Done
  return 0;
}
