#ifndef RR_MENU_H
#define RR_MENU_H 1

#include "unit.h"

#ifndef RR_MENU_UNITS
#define RR_MENU_UNITS 20
#endif

class RR_menu {
public:
    RR_unit a[RR_MENU_UNITS];
    RR_unit_part cursor;
    RR_vec2 cursor_dir;
    
    // Constructor
    RR_menu() {
        for(int i = 0; i < RR_MENU_UNITS; i++) a[i] = RR_unit(0, RR_vec2(rand() % 800, rand() % 600));
        cursor = RR_unit_part(1, RR_vec2(400, 300));
        cursor_dir = RR_vec2(0);
    }
    
    // Handle the background effects (ships follow cursor)
    void handle_background(SDL_Surface* win, int* mpos, float fspd) {
        RR_vec2 vec;
        for(int i = 0; i < RR_MENU_UNITS; i++) {
            
            // Ships follow cursor
            a[i].follow(RR_vec2(mpos[0], mpos[1]));
            
            // Some bouncing
            for(int u = i + 1; u < RR_MENU_UNITS; u++) {
                vec = vec.normal(a[i].pos, a[u].pos);
                if(vec.dot(vec, a[u].pos - a[i].pos) < 20) {
                    a[i].spd = a[i].spd - vec * 50.0;
                    a[u].spd = a[u].spd + vec * 50.0;
                }
            }
            
            // Move ships
            a[i].move(fspd);
            
            // Display ships
            a[i].draw(win, a[i].pos, a[i].nrm, 1.0);
        }
        
        // Darken background
        boxRGBA(win, 0, 0, 800, 600, 0, 0, 0, 200);
    }
    
    // Handle the cursor
    void handle_cursor(SDL_Surface* win, int* mpos) {
        
        // Calculate cursor position and direction
        cursor_dir = cursor_dir.normal(cursor.pos, RR_vec2(mpos[0], mpos[1]));
        cursor.pos = RR_vec2(mpos[0], mpos[1]) - cursor_dir * 13.0;
        
        // Draw cursor
        cursor.draw(win, cursor.pos, cursor_dir, 0.5, 1, 0);
    }
};

#endif // RR_MENU_H
