#ifndef RR_BATTLE_H
#define RR_BATTLE_H 1

#ifndef RR_BATTLE_MAX_UNITS
#define RR_BATTLE_MAX_UNITS 40
#endif

#ifndef RR_BATTLE_MAX_PARTICLES
#define RR_BATTLE_MAX_PARTICLES 400
#endif

#include "unit.h"
#include "particle.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_particle b[RR_BATTLE_MAX_PARTICLES]; // Particles on battlefield
    RR_vec2 cam, cam_trg;
    double zoom, zoom_trg;
    int next_particle;
    
    // Constructor
    RR_battle() {
        cam = RR_vec2();
        cam_trg = cam;
        zoom = 0.001;
        zoom_trg = zoom;
        next_particle = 0;
    }
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        int i1;
        
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
            
            // Fire weapons
            for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(a[i].p[u].in_use) {
                a[i].p[u].cool(fspd);
                if(a[i].fire && a[i].p[u].weapon()) {
                    for(int j = next_particle; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                        b[j] = RR_particle(
                            a[i].p[u].weapon(),
                            a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y,
                            a[i].nrm,
                            a[i].spd
                        );
                        next_particle = j + 1;
                        a[i].p[u].reload();
                        break;
                    }
                }
            }
            
            // Some bouncing
            for(int u = i + 1; u < RR_BATTLE_MAX_UNITS; u++) a[i].bounce(a[u]);
            
            // Check ship integrity
            i1 = rand() % RR_MAX_UNIT_PARTS;
            
            // Find parts without parents
            if(a[i].p[i1].in_use) if(!a[i].p[a[i].p[i1].parent].in_use || (rand() % 200 < 2 && a[i].self_destruct)) {
                a[i].p[i1].in_use = false;
                for(int u = next_particle; u < RR_BATTLE_MAX_PARTICLES; u++) if(!b[u].in_use) {
                    b[u] = RR_particle( // Destroyed part
                        4,
                        a[i].pos + a[i].nrm * a[i].p[i1].pos.x + a[i].nrm.extrude() * a[i].p[i1].pos.y,
                        a[i].nrm,
                        a[i].spd,
                        a[i].p[i1].type
                    );
                    a[i].recalculate();
                    next_particle = u + 1;
                    break;
                }
            }
            
            // Move ships
            a[i].move(fspd);
        } else a[i] = RR_unit(rand() % 6, RR_g_vec2.box_random() * 1000.0);
        
        // Loop through particles
        next_particle = 0;
        for(int i = 0; i < RR_BATTLE_MAX_PARTICLES; i++) if(b[i].in_use) {
            
            // Display particle
            b[i].draw(win, (b[i].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty), b[i].nrm, zoom);
            
            // Move particle
            b[i].move(fspd);
            
            // Calculate hits
            b[i].hitships(a, RR_BATTLE_MAX_UNITS, b, RR_BATTLE_MAX_PARTICLES, i);
        } else if(next_particle == 0) next_particle = i;
        
        // Done
        return 0;
    }
};

#endif // RR_BATTLE_H
