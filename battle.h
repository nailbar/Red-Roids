#ifndef RR_BATTLE_H
#define RR_BATTLE_H 1

#ifndef RR_BATTLE_MAX_UNITS
#define RR_BATTLE_MAX_UNITS 40
#endif

#ifndef RR_BATTLE_MAX_PARTICLES
#define RR_BATTLE_MAX_PARTICLES 400
#endif

#ifndef RR_BATTLE_FIELD_LIMIT
#define RR_BATTLE_FIELD_LIMIT 5000.0
#endif

#ifndef RR_BATTLE_MAX_FLEET
#define RR_BATTLE_MAX_FLEET 500
#endif

#ifndef RR_BATTLE_REINFORCEMENT_INTERVAL
#define RR_BATTLE_REINFORCEMENT_INTERVAL 60
#endif

#include "unit.h"
#include "particle.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_particle b[RR_BATTLE_MAX_PARTICLES]; // Particles on battlefield
    RR_vec2 cam, cam_trg;
    double zoom, zoom_trg;
    float reinforcements, player_timeout;
    int next_particle, player, player_team;
    
    // Fleets
    unsigned char fleetR[RR_BATTLE_MAX_FLEET];
    int fsizeR;
    unsigned char fleetG[RR_BATTLE_MAX_FLEET];
    int fsizeG;
    unsigned char fleetB[RR_BATTLE_MAX_FLEET];
    int fsizeB;
    
    // Constructor
    RR_battle() {
        cam = RR_vec2();
        cam_trg = cam;
        zoom = 0.001;
        zoom_trg = zoom;
        next_particle = 0;
        player = rand() % RR_BATTLE_MAX_UNITS;
        player_team = rand() % 3;
        player_timeout = 5.0;
        
        // Init fleets
        reinforcements = 0.0;
        int fleetsize = rand() % (rand() % (rand() % RR_BATTLE_MAX_FLEET));
        if(fleetsize < 4) fleetsize = 4;
        fsizeR = fleetsize;
        fsizeG = fleetsize;
        fsizeB = fleetsize;
        for(int i = 0; i < RR_BATTLE_MAX_FLEET; i++) if(i < fleetsize) {
            if(rand() % 5 > 0) {
                fleetR[i] = 1; // Arrow light fighter
                fleetG[i] = 2; // Bullet light fighter
                fleetB[i] = 3; // Raptor light fighter
            } else {
                fleetR[i] = 4; // Arrow medium fighter
                fleetG[i] = 5; // Bullet medium fighter
                fleetB[i] = 6; // Raptor medium fighter
            }
        } else {
            fleetR[i] = 0;
            fleetG[i] = 0;
            fleetB[i] = 0;
        }
    }
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        int i1;
        int teamR = 0;
        int teamG = 0;
        int teamB = 0;
        
        // Center camera on player
        if(a[player].in_use && a[player].team == player_team) {
            cam_trg = a[player].pos + a[player].spd * 0.75;
            zoom_trg = ((double(RR_g.wid + RR_g.hgt) / 2.0) / 800.0) / (1.0 + RR_g_vec2.distance(RR_vec2(), a[player].spd) * 0.002);
        } else player = rand() % RR_BATTLE_MAX_UNITS;
        
        // Switch ship
        if(keys[SDLK_n]) player = rand() % RR_BATTLE_MAX_UNITS;
        
        // Show map
        if(keys[SDLK_z]) {
            cam_trg = RR_vec2(0, 0);
            zoom_trg = ((double(RR_g.wid + RR_g.hgt) / 2.0) * 0.4) / RR_BATTLE_FIELD_LIMIT;
        }
        
        // Smooth camera transitions
        cam = cam + (cam_trg - cam) * 0.04;
        zoom = zoom + (zoom_trg - zoom) * 0.04;
        
        // Loop through ships
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) if(a[i].in_use) {
            
            // Count ships
            switch(a[i].team) {
            case 0: teamR++; break;
            case 1: teamG++; break;
            case 2: teamB++; break;
            }
            
            // Stay on battlefield
            if(a[i].pos.x > RR_BATTLE_FIELD_LIMIT) a[i].spd.x -= 1000.0 * fspd;
            if(a[i].pos.y > RR_BATTLE_FIELD_LIMIT) a[i].spd.y -= 1000.0 * fspd;
            if(a[i].pos.x < -RR_BATTLE_FIELD_LIMIT) a[i].spd.x += 1000.0 * fspd;
            if(a[i].pos.y < -RR_BATTLE_FIELD_LIMIT) a[i].spd.y += 1000.0 * fspd;
            
            // Display ship
            a[i].draw(win, (a[i].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty), a[i].nrm, zoom);
            
            // Player controls this ship
            if(i == player) {
                if(a[i].player_input(keys)) player_timeout = 15.0;
                else if(player_timeout > 0.0) player_timeout -= fspd;
                else a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
                
                // Find better target
                if(keys[SDLK_t] || RR_g_vec2.distance(a[i].pos, a[a[i].trg].pos) > 1300.0) a[i].find_better_target(a, RR_BATTLE_MAX_UNITS, i);
                
                // Draw target indicator
                if(a[i].has_valid_target(a, RR_BATTLE_MAX_UNITS, i)) {
                    a[i].target_pointer(
                        win,
                        (a[i].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty),
                        RR_g_vec2.normal(a[i].pos, a[a[i].trg].pos),
                        a[i].size,
                        zoom
                    );
                    a[a[i].trg].target_indicator(
                        win,
                        (a[a[i].trg].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty),
                        a[a[i].trg].size,
                        zoom
                    );
                } else a[i].find_better_target(a, RR_BATTLE_MAX_UNITS, i);
            }
            
            // A.I. ships follow other ships
            else a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
            
            // Fire weapons
            for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(a[i].p[u].in_use) {
                if(a[i].guns) a[i].p[u].cool(fspd, a[i].weight / float(a[i].guns));
                if(a[i].fire && a[i].p[u].weapon(false)) {
                    for(int j = next_particle; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                        b[j] = RR_particle(
                            a[i].p[u].weapon(false),
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
            if(a[i].p[i1].in_use) if(!a[i].p[a[i].p[i1].parent].in_use || (rand() % 50 < 2 && !a[i].guns)) {
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
        }
        
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
        
        // Spawn reinforcements
        if(reinforcements > 0.0) reinforcements -= fspd;
        else {
            reinforcements = RR_BATTLE_REINFORCEMENT_INTERVAL;
            for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) if(!a[i].in_use) {
                if(teamR < teamG && teamR < teamB && fsizeR) {
                    fsizeR--; teamR++;
                    a[i] = RR_unit(fleetR[fsizeR], RR_vec2(-RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT) + RR_g_vec2.box_random() * 500.0);
                } else if(teamG < teamB && fsizeG) {
                    fsizeG--; teamG++;
                    a[i] = RR_unit(fleetG[fsizeG], RR_vec2(RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT) + RR_g_vec2.box_random() * 500.0);
                } else if(fsizeB) {
                    fsizeB--; teamB++;
                    a[i] = RR_unit(fleetB[fsizeB], RR_vec2(0, -RR_BATTLE_FIELD_LIMIT) + RR_g_vec2.box_random() * 500.0);
                } else if(fsizeR) {
                    fsizeR--; teamR++;
                    a[i] = RR_unit(fleetR[fsizeR], RR_vec2(-RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT) + RR_g_vec2.box_random() * 500.0);
                } else if(fsizeG) {
                    fsizeG--; teamG++;
                    a[i] = RR_unit(fleetG[fsizeG], RR_vec2(RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT) + RR_g_vec2.box_random() * 500.0);
                }
            }
        }
        
        // Team status indicators
        RR_vec2 vec[4];
        vec[0] = RR_vec2(0, 0);
        vec[1] = RR_vec2(0, 5);
        vec[2] = RR_vec2(1 + teamR + fsizeR, 5);
        vec[3] = RR_vec2(1 + teamR + fsizeR, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 10), RR_vec2(1, 0), 1.0, 105, 0, 0);
        vec[2] = RR_vec2(1 + teamR, 5);
        vec[3] = RR_vec2(1 + teamR, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 10), RR_vec2(1, 0), 1.0, 155, 0, 0);
        vec[2] = RR_vec2(1 + teamG + fsizeG, 5);
        vec[3] = RR_vec2(1 + teamG + fsizeG, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 20), RR_vec2(1, 0), 1.0, 0, 85, 0);
        vec[2] = RR_vec2(1 + teamG, 5);
        vec[3] = RR_vec2(1 + teamG, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 20), RR_vec2(1, 0), 1.0, 0, 135, 0);
        vec[2] = RR_vec2(1 + teamB + fsizeB, 5);
        vec[3] = RR_vec2(1 + teamB + fsizeB, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 30), RR_vec2(1, 0), 1.0, 0, 0, 155);
        vec[2] = RR_vec2(1 + teamB, 5);
        vec[3] = RR_vec2(1 + teamB, 0);
        RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 30), RR_vec2(1, 0), 1.0, 0, 0, 205);
        
        // Done
        return 0;
    }
};

#endif // RR_BATTLE_H
