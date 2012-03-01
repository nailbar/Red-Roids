#ifndef RR_UNIT_PART_H
#define RR_UNIT_PART_H 1

#include "vector.h"

class RR_unit_part {
public:
    unsigned char type;
    RR_vec2 pos;
    bool in_use;
    
    // Constructor
    RR_unit_part() {
        in_use = false; // Not in use
    }
    RR_unit_part(unsigned char newtype, RR_vec2 newpos) {
        type = newtype;
        pos = newpos;
        in_use = true; // In use
    }
    
    // Draw the part on screen
    void draw(
        SDL_Surface* win, RR_vec2 position, RR_vec2 normal,
        float scale, unsigned char partid, unsigned char action,
        RR_vec2 sun_dir
    ) {
        RR_vec2 vec[5];
        switch(partid) {
        case 0: // Engine
            vec[0] = RR_vec2(-4, -5);
            vec[1] = RR_vec2(4, -7);
            vec[2] = RR_vec2(4, 7);
            vec[3] = RR_vec2(-4, 5);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(M_PI), sun_dir, scale, 100, 110, 130, 0.2, 0.3);
            
            // Action 1: engine burn
            if(action & 1) {
                vec[0] = RR_vec2(-4, -5);
                vec[1] = RR_vec2(-7 - (rand() % 7), -4);
                vec[2] = RR_vec2(-7 - (rand() % 7), 0);
                vec[3] = RR_vec2(-7 - (rand() % 7), 4);
                vec[4] = RR_vec2(-4, 5);
                position.draw_polygon(win, vec, 5, position, normal, RR_vec2(rand() % 1000), sun_dir, scale, 255, rand() % 255, 0, 0.2, 0.2);
            }
            break;
        case 1: // Hull
            
            // Right side
            vec[0] = RR_vec2(24, 0);
            vec[1] = RR_vec2(-16, 0);
            vec[2] = RR_vec2(-6, 12);
            position.draw_polygon(win, vec, 3, position, normal, RR_vec2(1.1), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            
            // Left side
            vec[0] = RR_vec2(24, 0);
            vec[1] = RR_vec2(-16, 0);
            vec[2] = RR_vec2(-6, -12);
            position.draw_polygon(win, vec, 3, position, normal, RR_vec2(-1.1), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            break;
        case 2: // Red small cockpit
            
            // Cockpit rear
            vec[0] = RR_vec2(-7, -2);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(-7, 2);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(M_PI), sun_dir, scale, 180, 180, 200, 0.2, 0.5);
            
            // Cockpit front
            vec[0] = RR_vec2(7, -3);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(7, 3);
            vec[4] = RR_vec2(10, 0);
            position.draw_polygon(win, vec, 5, position, normal, RR_vec2(0), sun_dir, scale, 255, 0, 0, 0.2, 0.2);
            break;
        case 3: // Green small cockpit
            
            // Cockpit rear
            vec[0] = RR_vec2(-7, -2);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(-7, 2);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(M_PI), sun_dir, scale, 180, 180, 200, 0.2, 0.5);
            
            // Cockpit front
            vec[0] = RR_vec2(7, -3);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(7, 3);
            vec[4] = RR_vec2(10, 0);
            position.draw_polygon(win, vec, 5, position, normal, RR_vec2(0), sun_dir, scale, 0, 200, 0, 0.2, 0.2);
            break;
        case 4: // Blue small cockpit
            
            // Cockpit rear
            vec[0] = RR_vec2(-7, -2);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(-7, 2);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(M_PI), sun_dir, scale, 180, 180, 200, 0.1, 0.5);
            
            // Cockpit front
            vec[0] = RR_vec2(7, -3);
            vec[1] = RR_vec2(-3, -4);
            vec[2] = RR_vec2(-3, 4);
            vec[3] = RR_vec2(7, 3);
            vec[4] = RR_vec2(10, 0);
            position.draw_polygon(win, vec, 5, position, normal, RR_vec2(0), sun_dir, scale, 0, 0, 255, 0.6, 0.2);
            break;
        case 5: // Hull

            // Front
            vec[0] = RR_vec2(-13, 0);
            vec[1] = RR_vec2(21, 4);
            vec[2] = RR_vec2(21, -4);
            position.draw_polygon(win, vec, 3, position, normal, RR_vec2(0), sun_dir, scale, 180, 180, 200, 0.4, 1.0);
            
            // Right side
            vec[0] = RR_vec2(21, 4);
            vec[1] = RR_vec2(-13, 0);
            vec[2] = RR_vec2(-9, 9);
            position.draw_polygon(win, vec, 3, position, normal, RR_vec2(1.4), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            
            // Left side
            vec[0] = RR_vec2(21, -4);
            vec[1] = RR_vec2(-13, 0);
            vec[2] = RR_vec2(-9, -9);
            position.draw_polygon(win, vec, 3, position, normal, RR_vec2(-1.4), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            break;
        case 6: // Hull
            
            // Middle
            vec[0] = RR_vec2(10, 4);
            vec[1] = RR_vec2(10, -4);
            vec[2] = RR_vec2(-10, -4);
            vec[3] = RR_vec2(-10, 4);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(M_PI), sun_dir, scale, 180, 180, 200, 0.6, 1.0);
            
            // Right side
            vec[0] = RR_vec2(10, 4);
            vec[1] = RR_vec2(-10, 4);
            vec[2] = RR_vec2(-6, 12);
            vec[3] = RR_vec2(6, 12);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(1.57), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            
            // Left side
            vec[0] = RR_vec2(10, -4);
            vec[1] = RR_vec2(-10, -4);
            vec[2] = RR_vec2(-6, -12);
            vec[3] = RR_vec2(6, -12);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(-1.57), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            break;
        case 7: // Hull right
            vec[0] = RR_vec2(27, -4);
            vec[1] = RR_vec2(-13, -4);
            vec[2] = RR_vec2(-7, 5);
            vec[3] = RR_vec2(0, 5);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(1.4), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            break;
        case 8: // Hull right
            vec[0] = RR_vec2(27, 4);
            vec[1] = RR_vec2(-13, 4);
            vec[2] = RR_vec2(-7, -5);
            vec[3] = RR_vec2(0, -5);
            position.draw_polygon(win, vec, 4, position, normal, RR_vec2(-1.4), sun_dir, scale, 180, 180, 200, 0.2, 1.0);
            break;
        }
        if(RR_g.debugmode == 1) pixelRGBA(win, position.x, position.y, 255, 255, 255, 255); // Debug position
        if(RR_g.debugmode == 3) ellipseRGBA(win, position.x, position.y, size(partid) * scale, size(partid) * scale, 255, 255, 0, 200); // Show part size
    }
    void draw(
        SDL_Surface* win, RR_vec2 position, RR_vec2 normal,
        float scale, unsigned char partid, unsigned char action
    ) {
        draw(win, position, normal, scale, partid, action, RR_vec2(0, -1));
    }
    
    // Get size of a part
    float size(int partid) {
        switch(partid) {
        case 0: return 5.0; // Engine
        case 1: return 11.0; // Hull
        case 2: return 6.0; // Red small cockpit
        case 3: return 6.0; // Green small cockpit
        case 4: return 6.0; // Blue small cockpit
        case 5: return 9.0; // Hull
        case 6: return 12.0; // Hull
        case 7: return 7.0; // Hull right
        case 8: return 7.0; // Hull right
        default: return 1.0;
        }
    }
    float size() {
        return size(type);
    }
    
    // Get thrust of a part (only parts with thrusters return value)
    float thrust(int partid) {
        switch(partid) {
        case 0: return 20.0; // Engine
        case 1: return 0.0; // Hull
        case 2: return 1.0; // Red small cockpit
        case 3: return 1.0; // Green small cockpit
        case 4: return 1.0; // Blue small cockpit
        case 5: return 0.0; // Hull
        case 6: return 0.0; // Hull
        case 7: return 0.0; // Hull right
        case 8: return 0.0; // Hull right
        default: return 0.0;
        }
    }
    float thrust() {
        return thrust(type);
    }
    
    // Get weight of a part
    float weight(int partid) {
        switch(partid) {
        case 0: return 5.0; // Engine
        case 1: return 11.0; // Hull
        case 2: return 6.0; // Red small cockpit
        case 3: return 6.0; // Green small cockpit
        case 4: return 6.0; // Blue small cockpit
        case 5: return 9.0; // Hull
        case 6: return 12.0; // Hull
        case 7: return 7.0; // Hull right
        case 8: return 7.0; // Hull right
        default: return 1.0;
        }
    }
    float weight() {
        return weight(type);
    }
};

#endif // RR_UNIT_PART_H
