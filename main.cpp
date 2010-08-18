#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <cmath>

#include "roidmap.h"

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
  
  //Framerate
  Uint32 time1;
  Uint32 time2;
  float fspd;
  time1=SDL_GetTicks();
  time2=0;
  fspd=1.0;
  
  //Roid map
  RR_roidmap rmap;
  
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
    
    //Get framerate
    time1=SDL_GetTicks();
    if(time2){
      fspd=float(time1-time2)/1000.0;
      
      //Limit framerate to fpslimit
      if(1.0/fspd>60) SDL_Delay(int((1.0/60-fspd)*1000.0));
    }
    time2=time1;
    
    //Mouse keys
    mkeys=SDL_GetMouseState(NULL,NULL);
    
    //Clear screen
    boxRGBA(win,-5,-5,805,605,0,0,0,255);
    
    //If LMB is pressed, draw something at cursor
    /*
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
    
    //Moving object
    odis=sqrt((mpos[0]-opos[0])*(mpos[0]-opos[0])+(mpos[1]-opos[1])*(mpos[1]-opos[1]));
    if(odis>0){
      odir[0]=(mpos[0]-opos[0])/odis;
      odir[1]=(mpos[1]-opos[1])/odis;
    }
    opos[0]+=odir[0]*fspd*50.0;
    opos[1]+=odir[1]*fspd*50.0;
    
    //Draw the object trail too
    lineRGBA(
      win,
      opos[0]+rand()%9-4,
      opos[1]+rand()%9-4,
      opos[0]+rand()%9-4,
      opos[1]+rand()%9-4,
      rand()%256,
      0,
      0,
      50
    );*/
    
    //Display the roidmap
    rmap.display(win,mpos);
    
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
