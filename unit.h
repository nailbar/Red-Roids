#ifndef RR_UNIT_H

#include "vector.h"

class RR_unit {
public:
    
    // Draw the unit on screen
    void draw(SDL_Surface* win, RR_vector position, RR_vector normal, float scale) {
        
        // Polygon vectors
        RR_vector vec[5];
        
        draw_part(win, position - normal * 11.0 * scale, normal, scale, 0, 0); // Engine
        draw_part(win, position, normal, scale, 1, 0); // Hull
        draw_part(win, position, normal, scale, 2, 0); // Cockpit
    }
    
    // Draw a part
    void draw_part(
        SDL_Surface* win, RR_vector position, RR_vector normal,
        float scale, unsigned char partid, unsigned char action
    ) {
        RR_vector vec[5];
        switch(partid) {
        case 0: // Engine
            vec[0] = RR_vector(-4, -5);
            vec[1] = RR_vector(4, -7);
            vec[2] = RR_vector(4, 7);
            vec[3] = RR_vector(-4, 5);
            position.draw_polygon(win, vec, 4, position, normal, RR_vector(M_PI), RR_vector(0, -1), scale, 100, 110, 130, 0.2, 0.3);
            
            // Action 1: engine burn
            if(action & 1) {
                vec[0] = RR_vector(-4, -5);
                vec[1] = RR_vector(-7 - (rand() % 7), -4);
                vec[2] = RR_vector(-7 - (rand() % 7), 0);
                vec[3] = RR_vector(-7 - (rand() % 7), 4);
                vec[4] = RR_vector(-4, 5);
                position.draw_polygon(win, vec, 5, position, normal, RR_vector(rand() % 1000), RR_vector(0, -1), scale, 255, rand() % 255, 0, 0.2, 0.2);
            }
            break;
        case 1: // Hull
            
            // Left side
            vec[0] = RR_vector(27, 0);
            vec[1] = RR_vector(-13, 0);
            vec[2] = RR_vector(-3, 12);
            position.draw_polygon(win, vec, 3, position, normal, RR_vector(-1.1), RR_vector(0, -1), scale, 180, 180, 200, 0.2, 1.0);
            
            // Right side
            vec[0] = RR_vector(27, 0);
            vec[1] = RR_vector(-13, 0);
            vec[2] = RR_vector(-3, -12);
            position.draw_polygon(win, vec, 3, position, normal, RR_vector(1.1), RR_vector(0, -1), scale, 180, 180, 200, 0.2, 1.0);
            break;
        case 2: // Cockpit
            
            // Cockpit rear
            vec[0] = RR_vector(-7, -2);
            vec[1] = RR_vector(-3, -4);
            vec[2] = RR_vector(-3, 4);
            vec[3] = RR_vector(-7, 2);
            position.draw_polygon(win, vec, 4, position, normal, RR_vector(M_PI), RR_vector(0, -1), scale, 180, 180, 200, 0.2, 0.5);
            
            // Cockpit front
            vec[0] = RR_vector(7, -3);
            vec[1] = RR_vector(-3, -4);
            vec[2] = RR_vector(-3, 4);
            vec[3] = RR_vector(7, 3);
            vec[4] = RR_vector(10, 0);
            position.draw_polygon(win, vec, 5, position, normal, RR_vector(0), RR_vector(0, -1), scale, 255, 0, 0, 0.2, 0.2);
            break;
        }
//         pixelRGBA(win, position.x, position.y, 255, 255, 255, 255); // Debug position
    }
};

#endif // RR_UNIT_H
