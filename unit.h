#ifndef RR_UNIT_H

#include "vector.h"

class RR_unit {
public:
    
    // Draw the unit on screen
    void draw(SDL_Surface* win, RR_vector position, RR_vector normal, float scale) {
        
        // Polygon vectors
        RR_vector vec[5];
        
        // Rear
        vec[0] = RR_vector(-12, -5);
        vec[1] = RR_vector(-4, -7);
        vec[2] = RR_vector(-4, 7);
        vec[3] = RR_vector(-12, 5);
        position.draw_polygon(win, vec, 4, position, normal, RR_vector(M_PI), RR_vector(0, -1), scale, 100, 110, 130, 0.3);
        
        // Left side
        vec[0] = RR_vector(30, 0);
        vec[1] = RR_vector(-10, 0);
        vec[2] = RR_vector(0, 12);
        position.draw_polygon(win, vec, 3, position, normal, RR_vector(-1.1), RR_vector(0, -1), scale, 180, 180, 200, 1.0);
        
        // Right side
        vec[0] = RR_vector(30, 0);
        vec[1] = RR_vector(-10, 0);
        vec[2] = RR_vector(0, -12);
        position.draw_polygon(win, vec, 3, position, normal, RR_vector(1.1), RR_vector(0, -1), scale, 180, 180, 200, 1.0);
        
        // Cockpit rear
        vec[0] = RR_vector(-4, -2);
        vec[1] = RR_vector(0, -4);
        vec[2] = RR_vector(0, 4);
        vec[3] = RR_vector(-4, 2);
        position.draw_polygon(win, vec, 4, position, normal, RR_vector(M_PI), RR_vector(0, -1), scale, 180, 180, 200, 0.5);
        
        // Cockpit front
        vec[0] = RR_vector(10, -3);
        vec[1] = RR_vector(0, -4);
        vec[2] = RR_vector(0, 4);
        vec[3] = RR_vector(10, 3);
        vec[4] = RR_vector(13, 0);
        position.draw_polygon(win, vec, 5, position, normal, RR_vector(0), RR_vector(0, -1), scale, 255, 0, 0, 0.2);
    }
};

#endif // RR_UNIT_H
