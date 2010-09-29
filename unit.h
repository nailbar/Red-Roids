#ifndef RR_UNIT_H
#define RR_UNIT_H 1

#ifndef RR_MAX_NEARBY_UNITS
#define RR_MAX_NEARBY_UNITS 5
#endif

#include "vector.h"

class RR_unit{
  public:
    RR_vector pos,spd;
    double rot;
    int team;
    int trg;
    int close[RR_MAX_NEARBY_UNITS];
    bool inuse;
    double energy;
    
    //Constructor
    RR_unit(){
      inuse=false;
    }
    
    //Display unit
    void display(SDL_Surface* win){
    }
};

#endif //RR_UNIT_H
