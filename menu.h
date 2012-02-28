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
    RR_vec2 cursor_dir, click_pos;
    char click;
    
    // Constructor
    RR_menu() {
        for(int i = 0; i < RR_MENU_UNITS; i++) a[i] = RR_unit(0, RR_vec2(rand() % 800, rand() % 600));
        cursor = RR_unit_part(1, RR_vec2(400, 300));
        cursor_dir = RR_vec2(0);
        click = 0;
    }
    
    // Handle information about mouse clicks
    void set_clicks(bool lmb, int* mpos) {
        if(lmb) {
            if(click == 0) { // Click start
                click = 1;
                click_pos = RR_vec2(mpos[0], mpos[1]);
            } else click = 2; // Click continue
        } else {
            if(click == 1 || click == 2) click = 3; // Release start
            else if(click == 3) click = 0; // Release continue
        }
    }
    
    // Handle the background effects (ships follow cursor)
    void handle_background(SDL_Surface* win, int* mpos, float fspd) {
        RR_vec2 vec;
        double dis;
        for(int i = 0; i < RR_MENU_UNITS; i++) {
            
            // A click
            if(click == 1) {
                vec = RR_vec2(mpos[0], mpos[1]);
                dis = vec.distance(a[i].pos, vec);
                if(dis < 400) {
                    vec = vec.normal(vec, a[i].pos);
                    a[i].spd = a[i].spd + vec * (400.0 - dis);
                }
            }
            
            // Ships follow cursor
            a[i].follow(RR_vec2(mpos[0], mpos[1]));
            
            // Some bouncing
            for(int u = i + 1; u < RR_MENU_UNITS; u++) {
                a[i].bounce(a[u]);
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
