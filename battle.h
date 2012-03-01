#ifndef RR_BATTLE_H
#define RR_BATTLE_H 1

#ifndef RR_BATTLE_MAX_UNITS
#define RR_BATTLE_MAX_UNITS 40
#endif

#include "unit.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_vec2 cam;
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        RR_vec2 cam_fix;
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) if(a[i].in_use) {
            
            // Ships follow other ships
            a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
            
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
            
            // Display ships
            a[i].draw(win, a[i].pos - cam + RR_vec2(RR_g.cntx, RR_g.cnty), a[i].nrm, 1.0);
            
            // Center camera
            cam_fix = cam_fix + a[i].pos;
        } else a[i] = RR_unit(rand() % 5, RR_g_vec2.box_random() * 1000.0);
        cam = cam_fix / double(RR_BATTLE_MAX_UNITS);
        
        // Done
        return 0;
    }
};

#endif // RR_BATTLE_H
