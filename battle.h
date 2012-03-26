#ifndef RR_BATTLE_H
#define RR_BATTLE_H 1

#ifndef RR_BATTLE_MAX_UNITS
#define RR_BATTLE_MAX_UNITS 40
#endif

#include "unit.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_vec2 cam, cam_trg;
    double zoom, zoom_trg;
    
    // Constructor
    RR_battle() {
        cam = RR_vec2();
        cam_trg = cam;
        zoom = 0.001;
        zoom_trg = zoom;
    }
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        
        // Center camera on player
        cam_trg = a[0].pos + a[0].spd * 0.5;
        zoom_trg = ((double(RR_g.wid + RR_g.hgt) / 2.0) / 800.0) / (1.0 + RR_g_vec2.distance(RR_vec2(), a[0].spd) * 0.001);
        
        // Smooth camera transitions
        cam = cam + (cam_trg - cam) * 0.04;
        zoom = zoom + (zoom_trg - zoom) * 0.04;
        
        // Loop through ships
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) if(a[i].in_use) {
            
            // Display ship
            a[i].draw(win, (a[i].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty), a[i].nrm, zoom);
            
            // Player controls this ship
            if(i == 0) a[i].player_input(keys);
            
            // A.I. ships follow other ships
            else a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
            
            // Some bouncing
            for(int u = i + 1; u < RR_BATTLE_MAX_UNITS; u++) if(a[i].bounce(a[u])) {
                
                // "Eat" other ship
                if(a[i].has_valid_target(a, RR_BATTLE_MAX_UNITS, i) && a[i].trg == u) {
                    a[u].from_preset(a[i].type);
                
                // Or get "eaten" by other ship
                } else if(a[u].has_valid_target(a, RR_BATTLE_MAX_UNITS, u) && a[u].trg == i) {
                    a[i].from_preset(a[u].type);
                }
            }
            
            // Move ships
            a[i].move(fspd);
        } else a[i] = RR_unit(rand() % 6, RR_g_vec2.box_random() * 1000.0);
        
        // Done
        return 0;
    }
};

#endif // RR_BATTLE_H
