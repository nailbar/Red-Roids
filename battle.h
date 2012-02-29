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
            
            // Ships follow cursor
            a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
            
            // Some bouncing
            for(int u = i + 1; u < RR_BATTLE_MAX_UNITS; u++) {
                a[i].bounce(a[u]);
            }
            
            // Move ships
            a[i].move(fspd);
            
            // Display ships
            a[i].draw(win, a[i].pos - cam + RR_vec2(400, 300), a[i].nrm, 1.0);
            cam_fix = cam_fix + a[i].pos;
        } else a[i] = RR_unit(2, RR_g_vec2.box_random() * 1000.0);
        cam = cam_fix / double(RR_BATTLE_MAX_UNITS);
        
        // Done
        return 0;
    }
};

#endif // RR_BATTLE_H
