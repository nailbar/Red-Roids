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
  
  //Variables for game loop
  SDL_Event event;
  bool inloop=true;
  Uint8* keys; //Keyboard input
  keys=SDL_GetKeyState(NULL);
  int mpos[2]={320,200}; //Mouse coordinates
  Uint8 mkeys; //Mouse keys
  
  //Game loop
  while(inloop){
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_QUIT: inloop=false; break;
        
        //Mouse moved
        case SDL_MOUSEMOTION:
          mpos[0]=event.motion.x;
          mpos[1]=event.motion.y;
          break;
      }
    }
    
    //Mouse keys
    mkeys=SDL_GetMouseState(NULL,NULL);
    
    //If LMB is pressed, draw something at cursor
    if(mkeys & SDL_BUTTON(1)){
        lineRGBA(
          win,
          mpos[0]+rand()%9-4,
          mpos[1]+rand()%9-4,
          mpos[0]+rand()%9-4,
          mpos[1]+rand()%9-4,
          rand()%256,
          rand()%256,
          rand()%256,
          255
        );
    }
    
    //Swap double buffer
    SDL_Flip(win);
    
    //Close on Escape
    if(keys[SDLK_ESCAPE]) inloop=false;
  }
  
  
  //Clean up
  SDL_Quit();
  
  //Done
  return 0;
}
