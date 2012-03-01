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
    RR_vec2 cursor_dir, click_pos, sun_dir;
    char click;
    
    // Constructor
    RR_menu() {
        for(int i = 0; i < RR_MENU_UNITS; i++) a[i] = RR_unit(0, RR_vec2(rand() % RR_g.wid, rand() % RR_g.hgt));
        cursor = RR_unit_part(1, RR_vec2(RR_g.cntx, RR_g.cnty));
        cursor_dir = RR_vec2(0);
        click = 0;
        sun_dir = RR_vec2(0, -1);
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
        int rnd = rand() % 6;
        for(int i = 0; i < RR_MENU_UNITS; i++) {
            
            // A click forces ships away from cursor
            if(click == 1) {
                dis = vec.distance(a[i].pos, click_pos);
                if(dis < 400) {
                    vec = vec.normal(click_pos, a[i].pos);
                    a[i].spd = a[i].spd + vec * (400.0 - dis);
                }
                a[i].from_preset(rnd);
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
            a[i].draw(win, a[i].pos, a[i].nrm, 1.0, sun_dir);
        }
        
        // Darken background
        boxRGBA(win, 0, 0, RR_g.wid, RR_g.hgt, 0, 0, 0, 200);
    }
    
    // Handle the menu items
    char handle_menu(SDL_Surface* win, int* mpos) {
        char button_clicked;
        char return_value = 0;
        
        // New instant battle button
        button_clicked = draw_button(win, mpos, RR_vec2(150, 150));
        if(button_clicked) return_value = 1;
        
        // Resume battle button
        button_clicked = draw_button(win, mpos, RR_vec2(RR_g.wid - 150, 150));
        if(button_clicked) return_value = 2;
        
//         // Undefined button
//         button_clicked = draw_button(win, mpos, RR_vec2(150, 400));
//         if(button_clicked) return_value = 3;
        
        // Exit button
        button_clicked = draw_button(win, mpos, RR_vec2(RR_g.wid - 150, 400));
        if(button_clicked) return_value = 4;
        
        // Button graphics (quick and dirty)
        RR_unit unit = RR_unit(0, RR_vec2());
        unit.draw(win, RR_vec2(100, 120), RR_vec2(1, 0), 1.0, sun_dir);
        unit.draw(win, RR_vec2(100, 180), RR_vec2(1, 0), 1.0, sun_dir);
        unit.draw(win, RR_vec2(RR_g.wid - 200, 180), RR_vec2(2), 1.0, sun_dir);
        unit.from_preset(2);
        unit.draw(win, RR_vec2(200, 120), RR_vec2(-1, 0), 1.0, sun_dir);
        unit.draw(win, RR_vec2(200, 180), RR_vec2(-1, 0), 1.0, sun_dir);
        unit.draw(win, RR_vec2(RR_g.wid - 150, 100), RR_vec2(1.7), 1.0, sun_dir);
        unit.draw(win, RR_vec2(RR_g.wid - 100, 140), RR_vec2(2.4), 1.0, sun_dir);
        RR_unit_part part;
        part.draw(win, RR_vec2(RR_g.wid - 160, 400), RR_vec2(0.5), 1, 4, 0, sun_dir);
        part.draw(win, RR_vec2(RR_g.wid - 180, 430), RR_vec2(0.4), 1, 7, 0, sun_dir);
        part.draw(win, RR_vec2(RR_g.wid - 140, 380), RR_vec2(0.4), 1, 8, 0, sun_dir);
        part.draw(win, RR_vec2(RR_g.wid - 180, 380), RR_vec2(0.5), 1, 0, 0, sun_dir);
        
        
        // Tell caller if user clicked anything
        return return_value;
    }
    
    // Handle the cursor
    void handle_cursor(SDL_Surface* win, int* mpos) {
        
        // Calculate cursor position and direction
        cursor_dir = cursor_dir.normal(cursor.pos, RR_vec2(mpos[0], mpos[1]));
        cursor.pos = RR_vec2(mpos[0], mpos[1]) - cursor_dir * 13.0;
        
        // Draw cursor
        cursor.draw(win, cursor.pos, cursor_dir, 0.5, 1, 0, sun_dir);
    }
    
    // Handle the menu items
    bool draw_button(SDL_Surface* win, int* mpos, RR_vec2 position) {
        RR_vec2 vec[4];
        
        // Check for hovering
        bool hover = true;
        bool is_clicked = false;
        if(mpos[0] < position.x - 100) hover = false;
        if(mpos[0] > position.x + 100) hover = false;
        if(mpos[1] < position.y - 100) hover = false;
        if(mpos[1] > position.y + 100) hover = false;
        
        // Center
        vec[0] = RR_vec2(-90, -90);
        vec[1] = RR_vec2(90, -90);
        vec[2] = RR_vec2(90, 90);
        vec[3] = RR_vec2(-90, 90);
        
        // Hover effect
        if(hover) {
            vec[0].draw_polygon(win, vec, 4, position, 200, 0, 0);
            vec[0] = RR_vec2(-85, -85);
            vec[1] = RR_vec2(85, -85);
            vec[2] = RR_vec2(85, 85);
            vec[3] = RR_vec2(-85, 85);
            vec[0].draw_polygon(win, vec, 4, position, 15, 20, 15);
            
            // Clicked
            if(
                click == 3 && click_pos.x > position.x - 100
                && click_pos.x < position.x + 100
                && click_pos.y > position.y - 100
                && click_pos.y < position.y + 100
            ) is_clicked = true;
        } else vec[0].draw_polygon(win, vec, 4, position, 90, 100, 90);
        
        // Sides
        vec[0] = RR_vec2(100, -100);
        vec[1] = RR_vec2(90, -90);
        vec[2] = RR_vec2(90, 90);
        vec[3] = RR_vec2(100, 100);
        RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(1, 0), RR_vec2(0), sun_dir, 1.0, 90, 100, 90, 0.2, 1.0);
        RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(0, 1), RR_vec2(0), sun_dir, 1.0, 90, 100, 90, 0.2, 1.0);
        RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(0, -1), RR_vec2(0), sun_dir, 1.0, 90, 100, 90, 0.2, 1.0);
        RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(-1, 0), RR_vec2(0), sun_dir, 1.0, 90, 100, 90, 0.2, 1.0);
        
        // Tell caller about hover status
        return is_clicked;
    }
};

#endif // RR_MENU_H
