#ifndef RR_BATTLEFIELD_H
#define RR_BATTLEFIELD_H 1

#ifndef RR_MAX_UNITS
#define RR_MAX_UNITS 50
#endif

#include "vector.h"
#include "unit.h"
//#include "frag.h"
//#include "strategy.h"

class RR_battlefield{
  public:
    RR_unit a[RR_MAX_UNITS];
    
    //Constructor
    RR_battlefield(){
    }
    
    //Display battlefield
    void display(SDL_Surface* win, int mpos[]){
    }
};

#endif //RR_BATTLEFIELD_H
