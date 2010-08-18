#ifndef RR_ROIDMAP_H
#define RR_ROIDMAP_H 1

#ifndef RR_MAXROIDS
#define RR_MAXROIDS 100
#endif

#ifndef RR_MAXLINKS
#define RR_MAXLINKS 6
#endif

#ifndef RR_MINDIST
#define RR_MINDIST 25
#endif

#ifndef RR_MAXTEAMS
#define RR_MAXTEAMS 4
#endif

#include "vector.h"

class RR_roidmap{
  public:
    RR_vector rpos[RR_MAXROIDS];
    int rlink[RR_MAXROIDS][RR_MAXLINKS];
    bool rused[RR_MAXROIDS];
    int rowner[RR_MAXROIDS];
    
    //Constructor
    RR_roidmap(){
      
      //Randomly place roids
      for(int i=0;i<RR_MAXROIDS;i++){
        rpos[i]=RR_vector(rand()%800,rand()%600);
        rused[i]=true;
        rowner[i]=0;
        for(int u=0;u<RR_MAXLINKS;u++) rlink[i][u]=-1;
        
        //Remove roids too close to each other
        for(int u=0;u<i;u++){
          if(sqrt((rpos[i].x-rpos[u].x)*(rpos[i].x-rpos[u].x)+(rpos[i].y-rpos[u].y)*(rpos[i].y-rpos[u].y))<RR_MINDIST){
            rused[i]=false;
            break;
          }
        }
      }
      
      //Calculate links
      RR_vector cnt;
      double dis,tmpdis;
      bool makelink;
      for(int i=0;i<RR_MAXROIDS;i++) if(rused[i]){
        for(int u=0;u<i;u++) if(rused[u]){
          
          //Get center between roids
          cnt.x=(rpos[i].x+rpos[u].x)/2.0;
          cnt.y=(rpos[i].y+rpos[u].y)/2.0;
          
          //Get distance between roids
          dis=sqrt((rpos[i].x-rpos[u].x)*(rpos[i].x-rpos[u].x)+(rpos[i].y-rpos[u].y)*(rpos[i].y-rpos[u].y));
          
          //Make sure no other roids are in the way
          makelink=true;
          for(int j=0;j<RR_MAXROIDS;j++) if(j!=i && j!=u && makelink && rused[j]){
            tmpdis=sqrt((rpos[j].x-cnt.x)*(rpos[j].x-cnt.x)+(rpos[j].y-cnt.y)*(rpos[j].y-cnt.y));
            if(tmpdis<dis*0.5){
              makelink=false;
            }
          }
          
          //Make the link
          if(makelink){
            for(int j=0;j<RR_MAXLINKS;j++) if(rlink[i][j]<0){
              rlink[i][j]=u;
              break;
            }
            for(int j=0;j<RR_MAXLINKS;j++) if(rlink[u][j]<0){
              rlink[u][j]=i;
              break;
            }
          }
        }
      }
      
      //Randomly place home bases
      int tmpi;
      for(int i=1;i<=RR_MAXTEAMS;i++){
        tmpi=rand()%RR_MAXROIDS;
        rowner[tmpi]=i;
      }
    }
    
    //Display roidmap
    void display(SDL_Surface* win, int mpos[]){
      double dis=800;
      double tmpdis;
      int nearest=0;
      for(int i=0;i<RR_MAXROIDS;i++) if(rused[i]){
        
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
        
        //Draw team color
        switch(rowner[i]){
          case 1: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,255,0,0,255); break;
          case 2: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,0,255,0,255); break;
          case 3: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,0,0,255,255); break;
          case 4: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,255,255,0,255); break;
          case 5: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,255,0,255,255); break;
          case 6: ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,0,255,255,255); break;
          default: if(rowner[i]>0) ellipseRGBA(win,rpos[i].x,rpos[i].y,8,8,255,255,255,255);
        }
        
        //Draw links
        for(int u=0;u<RR_MAXLINKS;u++) if(rlink[i][u]>-1 && rlink[i][u]<i) lineRGBA(
          win,
          rpos[i].x,
          rpos[i].y,
          rpos[rlink[i][u]].x,
          rpos[rlink[i][u]].y,
          100,
          100,
          100,
          50
        );
        
        //Check if roid is nearest so far
        tmpdis=sqrt((rpos[i].x-mpos[0])*(rpos[i].x-mpos[0])+(rpos[i].y-mpos[1])*(rpos[i].y-mpos[1]));
        if(tmpdis<dis){
          dis=tmpdis;
          nearest=i;
        }
      
      //Draw removed roid
      }else ellipseRGBA(
        win,
        rpos[i].x,
        rpos[i].y,
        5,
        5,
        255,
        0,
        0,
        50
      );
      
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
      for(int u=0;u<RR_MAXLINKS;u++) if(rlink[nearest][u]>-1) lineRGBA(
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
