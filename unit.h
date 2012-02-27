#ifndef RR_UNIT_H

#include "vector.h"
#include "unit_part.h"

#ifndef RR_MAX_UNIT_PARTS
#define RR_MAX_UNIT_PARTS 10
#endif

class RR_unit {
public:
    RR_unit_part p[RR_MAX_UNIT_PARTS];
    
    // Constructor
    RR_unit() {
        p[0] = RR_unit_part(2, RR_vec2(0, 0)); // Cockpit
        p[1] = RR_unit_part(1, RR_vec2(0, 0)); // Hull
        p[2] = RR_unit_part(0, RR_vec2(-11, 0)); // Engine
    }
    
    // Draw the unit on screen
    void draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
        
        // Loop through all parts
        for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].flags & 1) {
            
            // Draw any part that exists
            p[i].draw(win, position + (normal * p[i].pos.x + normal.extrude() * p[i].pos.y) * scale, normal, scale, p[i].type, 0);
        }
    }
};

#endif // RR_UNIT_H
