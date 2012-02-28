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
            
            // A click forces ships away from cursor
            if(click == 1) {
                dis = vec.distance(a[i].pos, click_pos);
                if(dis < 400) {
                    vec = vec.normal(click_pos, a[i].pos);
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
    
    // Handle the menu items
    char handle_menu(SDL_Surface* win, int* mpos) {
        char button_clicked;
        char return_value = 0;
        
        // Instant battle button
        button_clicked = draw_button(win, mpos, RR_vec2(150, 150));
        if(button_clicked) return_value = 1;
        
        // Undefined button
        button_clicked = draw_button(win, mpos, RR_vec2(650, 150));
        if(button_clicked) return_value = 2;
        
        // Undefined button
        button_clicked = draw_button(win, mpos, RR_vec2(650, 400));
        if(button_clicked) return_value = 3;
        
        // Exit button
        button_clicked = draw_button(win, mpos, RR_vec2(150, 400));
        if(button_clicked) return_value = 4;
        
        // Tell caller if user clicked anything
        return return_value;
    }
    
    // Handle the cursor
    void handle_cursor(SDL_Surface* win, int* mpos) {
        
        // Calculate cursor position and direction
        cursor_dir = cursor_dir.normal(cursor.pos, RR_vec2(mpos[0], mpos[1]));
        cursor.pos = RR_vec2(mpos[0], mpos[1]) - cursor_dir * 13.0;
        
        // Draw cursor
        cursor.draw(win, cursor.pos, cursor_dir, 0.5, 1, 0);
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
            
            // Clicked
            if(
                click == 3 && click_pos.x > position.x - 100
                && click_pos.x < position.x + 100
                && click_pos.y > position.y - 100
                && click_pos.y < position.y + 100
            ) is_clicked = true;
        }
        vec[0].draw_polygon(win, vec, 4, position, 50, 50, 50);
        
        // Top
        vec[0] = RR_vec2(-100, -100);
        vec[1] = RR_vec2(-90, -90);
        vec[2] = RR_vec2(90, -90);
        vec[3] = RR_vec2(100, -100);
        vec[0].draw_polygon(win, vec, 4, position, 200, 200, 200);
        
        // Bottom
        vec[0] = RR_vec2(-100, 100);
        vec[1] = RR_vec2(-90, 90);
        vec[2] = RR_vec2(90, 90);
        vec[3] = RR_vec2(100, 100);
        vec[0].draw_polygon(win, vec, 4, position, 20, 20, 20);
        
        // Left
        vec[0] = RR_vec2(-100, -100);
        vec[1] = RR_vec2(-90, -90);
        vec[2] = RR_vec2(-90, 90);
        vec[3] = RR_vec2(-100, 100);
        vec[0].draw_polygon(win, vec, 4, position, 80, 80, 80);
        
        // Right
        vec[0] = RR_vec2(100, -100);
        vec[1] = RR_vec2(90, -90);
        vec[2] = RR_vec2(90, 90);
        vec[3] = RR_vec2(100, 100);
        vec[0].draw_polygon(win, vec, 4, position, 80, 80, 80);
        
        // Tell caller about hover status
        return is_clicked;
    }
};

#endif // RR_MENU_H
