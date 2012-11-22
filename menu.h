#ifndef RR_MENU_H
#define RR_MENU_H 1

#include "text.h"
#include "unit.h"
#include "starfield.h"

#ifndef RR_MENU_UNITS
#define RR_MENU_UNITS 20
#endif

class RR_menu {
public:
    RR_unit a[RR_MENU_UNITS];
    RR_unit_part cursor;
    RR_vec2 cursor_dir, click_pos, sun_dir;
    char click;
    RR_starfield stars; // Background starfield
    float starpos;
    char cur_entry;
    char cur_key;
    
    // Constructor
    RR_menu() {
        for(int i = 0; i < RR_MENU_UNITS; i++) a[i] = RR_unit(1, 0, RR_vec2(rand() % RR_g.wid, rand() % RR_g.hgt));
        cursor = RR_unit_part(1, RR_vec2(RR_g.cntx, RR_g.cnty), 0);
        cursor_dir = RR_vec2(0);
        click = 0;
        sun_dir = RR_vec2(0, -1);
        starpos = 0.0;
        cur_entry = 0;
        cur_key = 0;
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
        sun_dir = RR_g_vec2.rotate(sun_dir, RR_vec2(M_PI * 0.03 * fspd));
        RR_vec2 vec;
        double dis;
        int rnd = rand();
        
        // Draw starfield
        stars.draw(win, RR_vec2(starpos += 300.0 * fspd, 0), 1.0);
        
        // Draw ships
        for(int i = 0; i < RR_MENU_UNITS; i++) {
            
            // A click forces ships away from cursor
            if(click == 1) {
                dis = vec.distance(a[i].pos, click_pos);
                if(dis < 400) {
                    vec = vec.normal(click_pos, a[i].pos);
                    a[i].spd = a[i].spd + vec * (400.0 - dis);
                }
                a[i].from_preset(rnd % RR_g_data.maxunit + 1, rnd % 3);
            }
            
            // Ships follow cursor
            a[i].follow(RR_vec2(mpos[0], mpos[1]), false);
            
            // Some bouncing
            for(int u = i + 1; u < RR_MENU_UNITS; u++) {
                a[i].bounce(a[u], false, false);
            }
            
            // Move ships
            a[i].move(fspd);
            
            // Display ships
            a[i].draw(win, a[i].pos, a[i].nrm, 1.0, sun_dir);
        }
        
        // Darken background
        boxRGBA(win, 0, 0, RR_g.wid, RR_g.hgt, 0, 0, 0, 200);
    }
    
    // Handle the menu items
    char handle_menu(SDL_Surface* win, int* mpos) {
        float f1 = RR_g.wid / 1000.0;
        
        // Title and credits
        RR_g_text.draw(win, RR_vec2(250 * f1, 50 * f1), 14 * f1, 120, 160, 255, "Red Roids");
        RR_g_text.draw(win, RR_vec2(250 * f1, 130 * f1), 4 * f1, 20, 60, 155, " * by Martin Wikstrom");
        
        // Up key
        if(RR_g.keys[SDLK_w] || RR_g.keys[SDLK_i] || RR_g.keys[SDLK_UP]) {
            RR_g.midle = true;
            cur_key = 1;
        } else if(cur_key == 1) {
            cur_key = 0;
            if(cur_entry == 2) cur_entry = 1;
            else if(cur_entry == 4) cur_entry = 2;
            else if(cur_entry == 0) cur_entry = 4;
        }
        
        // Down key
        if(RR_g.keys[SDLK_s] || RR_g.keys[SDLK_k] || RR_g.keys[SDLK_DOWN]) {
            RR_g.midle = true;
            cur_key = 2;
        } else if(cur_key == 2) {
            cur_key = 0;
            if(cur_entry == 2) cur_entry = 4;
            else if(cur_entry == 1) cur_entry = 2;
            else if(cur_entry == 0) cur_entry = 1;
        }
        
        // Fire key
        if(RR_g.keys[SDLK_q] || RR_g.keys[SDLK_SPACE] || RR_g.keys[SDLK_RETURN]) {
            RR_g.midle = true;
            cur_key = 3;
        } else if(cur_key == 3) {
            cur_key = 0;
            click = 3;
        }
        
        // Mouse over
        if(!RR_g.midle) {
            if(mpos[1] > 200 * f1 && mpos[1] < 250 * f1) cur_entry = 1;
            else if(mpos[1] > 260 * f1 && mpos[1] < 310 * f1) cur_entry = 2;
            else if(mpos[1] > 320 * f1 && mpos[1] < 370 * f1) cur_entry = 4;
            else cur_entry = 0;
        }
        
        // Menu entries
        if(cur_entry == 1) RR_g_text.draw(win, RR_vec2(250 * f1, 200 * f1), 10 * f1, 255, 60, 20, "Instant action");
        else RR_g_text.draw(win, RR_vec2(250 * f1, 200 * f1), 10 * f1, 180, 180, 200, "Instant action");
        
        if(cur_entry == 2) RR_g_text.draw(win, RR_vec2(250 * f1, 260 * f1), 10 * f1, 255, 60, 20, "Continue");
        else RR_g_text.draw(win, RR_vec2(250 * f1, 260 * f1), 10 * f1, 150, 150, 170, "Continue");
        
        if(cur_entry == 4) RR_g_text.draw(win, RR_vec2(250 * f1, 320 * f1), 10 * f1, 255, 60, 20, "Exit");
        else RR_g_text.draw(win, RR_vec2(250 * f1, 320 * f1), 10 * f1, 120, 120, 140, "Exit");
        
        // Tell caller if user clicked anything
        if(click == 3) return cur_entry;
        return 0;
    }
    
    // Handle the cursor
    void handle_cursor(SDL_Surface* win, int* mpos) {
        
        // Calculate cursor position and direction
        cursor_dir = cursor_dir.normal(cursor.pos, RR_vec2(mpos[0], mpos[1]));
        cursor.pos = RR_vec2(mpos[0], mpos[1]) - cursor_dir * 13.0;
        
        // Draw cursor
        cursor.draw(win, cursor.pos, cursor_dir, 0.5, 1, 0, sun_dir);
    }
};

#endif // RR_MENU_H
