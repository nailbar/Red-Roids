#ifndef RR_ROIDMAP_H
#define RR_ROIDMAP_H 1

#ifndef RR_MAXROIDS
#define RR_MAXROIDS 100
#endif

#include "vector.h"

class RR_roidmap{
  public:
    RR_vector rpos[RR_MAXROIDS];
    int rlink[RR_MAXROIDS][6];
    
    //Constructor
    RR_roidmap(){
      for(int i=0;i<RR_MAXROIDS;i++){
        rpos[i]=RR_vector(rand()%800,rand()%600);
        for(int u=0;u<6;u++) rlink[i][u]=-1;
      }
      
      //Calculate links
      RR_vector cnt;
      double dis,tmpdis;
      bool makelink;
      for(int i=0;i<RR_MAXROIDS;i++){
        for(int u=0;u<RR_MAXROIDS;u++) if(u!=i){
          
          //Get center between roids
          cnt.x=(rpos[i].x+rpos[u].x)/2.0;
          cnt.y=(rpos[i].y+rpos[u].y)/2.0;
          
          //Get distance between roids
          dis=sqrt((rpos[i].x-rpos[u].x)*(rpos[i].x-rpos[u].x)+(rpos[i].y-rpos[u].y)*(rpos[i].y-rpos[u].y));
          
          //Make sure no other roids are in the way
          makelink=true;
          for(int j=0;j<RR_MAXROIDS;j++) if(j!=i && j!=u && makelink){
            tmpdis=sqrt((rpos[j].x-cnt.x)*(rpos[j].x-cnt.x)+(rpos[j].y-cnt.y)*(rpos[j].y-cnt.y));
            if(tmpdis<dis*0.5){
              makelink=false;
            }
          }
          
          //Make the link
          if(makelink){
            for(int j=0;j<6;j++) if(rlink[i][j]<0){
              rlink[i][j]=u;
              break;
            }
            for(int j=0;j<6;j++) if(rlink[u][j]<0){
              rlink[u][j]=i;
              break;
            }
          }
        }
      }
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
        
        //Draw links
        for(int u=0;u<6;u++) if(rlink[i][u]>-1) lineRGBA(
          win,
          rpos[i].x,
          rpos[i].y,
          rpos[rlink[i][u]].x,
          rpos[rlink[i][u]].y,
          100,
          100,
          100,
          25
        );
        
        //Check if roid is nearest so far
        tmpdis=sqrt((rpos[i].x-mpos[0])*(rpos[i].x-mpos[0])+(rpos[i].y-mpos[1])*(rpos[i].y-mpos[1]));
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
        
      //Draw links
      for(int u=0;u<6;u++) if(rlink[nearest][u]>-1) lineRGBA(
        win,
        rpos[nearest].x,
        rpos[nearest].y,
        rpos[rlink[nearest][u]].x,
        rpos[rlink[nearest][u]].y,
        255,
        0,
        0,
        100
      );
    }
};

#endif //RR_ROIDMAP_H
