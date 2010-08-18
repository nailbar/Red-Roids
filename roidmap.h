#ifndef RR_ROIDMAP_H
#define RR_ROIDMAP_H 1

#ifndef RR_MAXROIDS
#define RR_MAXROIDS 100
#endif

#include "vector.h"

class RR_roidmap{
  public:
    RR_vector rpos[RR_MAXROIDS];
    
    //Constructor
    RR_roidmap(){
      for(int i=0;i<RR_MAXROIDS;i++) rpos[i]=RR_vector(rand()%800,rand()%600);
    }
    
    //Display roidmap
    void display(SDL_Surface* win, int mpos[]){
      double dis=800;
      double tmpdis;
      int nearest=0;
      for(int i=0;i<RR_MAXROIDS;i++){
        
        //Draw roid
        filledEllipseRGBA(
          win,
          rpos[i].x,
          rpos[i].y,
          5,
          5,
          100,
          100,
          100,
          255
        );
        
        //Check if roid is nearest so far
        tmpdis=sqrt((rpos[i].x-mpos[0]) * (rpos[i].x-mpos[0])+ (rpos[i].y-mpos[1]) * (rpos[i].y-mpos[1]));
        if(tmpdis<dis){
          dis=tmpdis;
          nearest=i;
        }
      }
      
      //Mark nearest roid
      filledEllipseRGBA(
        win,
        rpos[nearest].x,
        rpos[nearest].y,
        7,
        7,
        255,
        0,
        0,
        100
      );
    }
};

#endif //RR_ROIDMAP_H
