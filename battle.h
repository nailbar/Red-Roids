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
#include "starfield.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_particle b[RR_BATTLE_MAX_PARTICLES]; // Particles on battlefield
    RR_starfield stars; // Background starfield
    RR_vec2 cam, cam_trg, teamR_pos, teamG_pos, teamB_pos;
    double zoom, zoom_trg;
    float reinforcements, player_timeout, battle_timeout;
    int next_particle, player, player_team, wings;
    
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
        player_timeout = 0.0;
        teamR_pos = RR_vec2(-RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT);
        teamG_pos = RR_vec2(RR_BATTLE_FIELD_LIMIT, RR_BATTLE_FIELD_LIMIT);
        teamB_pos = RR_vec2(0, -RR_BATTLE_FIELD_LIMIT);
        battle_timeout = 5.0;
        
        // Init fleets
        wings = 1;
        fsizeR = wings;
        fsizeG = wings;
        fsizeB = wings;
        for(int i = 0; i < wings && i < RR_BATTLE_MAX_FLEET; i++) {
            fleetR[i] = rand() % 4;
            fleetG[i] = rand() % 4;
            fleetB[i] = rand() % 4;
        }
    }
    
    void next_battle() {
        
        // Clear field
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) a[i].in_use = false;
        next_particle = 0;
        player = rand() % RR_BATTLE_MAX_UNITS;
        player_team = rand() % 3;
        reinforcements = 0.0;
        battle_timeout = 5.0;
            
        wings++;
        fsizeR = wings;
        fsizeG = wings;
        fsizeB = wings;
        for(int i = 0; i < wings && i < RR_BATTLE_MAX_FLEET; i++) {
            fleetR[i] = rand() % 4;
            fleetG[i] = rand() % 4;
            fleetB[i] = rand() % 4;
        }
    }
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        int i1, i2;
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
        
        // Draw starfield
        stars.draw(win, cam);
        
        // Darken background if zoom-out is in use
        if(keys[SDLK_z]) boxRGBA(win, 0, 0, RR_g.wid, RR_g.hgt, 0, 0, 0, 200);
        
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
            
            // A.I. ships follow other ships
            } else a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
            
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
            for(int u = i + 1; u < RR_BATTLE_MAX_UNITS; u++) if((i2 = a[i].bounce(a[u], true, true))) {
                i1 = 0;
                for(int k = 0; k < i2 * 0.01; k++) for(int j = i1; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                    if(rand() % 100 < 30) b[j] = RR_particle(0, a[i].tmp_vec2); // Sparks
                    else b[j] = RR_particle(2, a[i].tmp_vec2); // Fragments
                    i1 = j + 1;
                    break;
                }
            }
            
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
            if(teamR < teamG && teamR < teamB && fsizeR) {
                fsizeR--;
                addfleet(fleetR[fsizeR], 1);
            } else if(teamG < teamB && fsizeG) {
                fsizeG--;
                addfleet(fleetG[fsizeG], 2);
            } else if(fsizeB) {
                fsizeB--;
                addfleet(fleetB[fsizeB], 3);
            } else if(fsizeR) {
                fsizeR--;
                addfleet(fleetR[fsizeR], 1);
            } else if(fsizeG) {
                fsizeG--;
                addfleet(fleetG[fsizeG], 2);
            }
            if(teamR > 0 && teamG > 0 && teamB > 0) reinforcements = RR_BATTLE_REINFORCEMENT_INTERVAL;
        }
        
        // Target status indicator
        if(a[player].in_use) {
            a[player].draw(win, RR_vec2(50 + a[player].size, RR_g.hgt - 50 - a[player].size), a[player].nrm, 1.0, RR_vec2(0, -1), true);
            if(a[player].has_valid_target(a, RR_BATTLE_MAX_UNITS, player)) {
                a[a[player].trg].draw(win, RR_vec2(RR_g.wid - 50 - a[a[player].trg].size, RR_g.hgt - 50 - a[a[player].trg].size), a[a[player].trg].nrm, 1.0, RR_vec2(0, -1), true);
            }
        }
        
        // Team status indicators
        RR_vec2 vec[4];
        vec[0] = RR_vec2(0, 0);
        vec[1] = RR_vec2(0, 5);
        if(teamR + fsizeR) {
            vec[2] = RR_vec2(1 + teamR + fsizeR, 5);
            vec[3] = RR_vec2(1 + teamR + fsizeR, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 10), RR_vec2(1, 0), 1.0, 105, 0, 0);
            vec[2] = RR_vec2(1 + teamR, 5);
            vec[3] = RR_vec2(1 + teamR, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 10), RR_vec2(1, 0), 1.0, 155, 0, 0);
        }
        if(teamG + fsizeG) {
            vec[2] = RR_vec2(1 + teamG + fsizeG, 5);
            vec[3] = RR_vec2(1 + teamG + fsizeG, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 20), RR_vec2(1, 0), 1.0, 0, 85, 0);
            vec[2] = RR_vec2(1 + teamG, 5);
            vec[3] = RR_vec2(1 + teamG, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 20), RR_vec2(1, 0), 1.0, 0, 135, 0);
        }
        if(teamB + fsizeB) {
            vec[2] = RR_vec2(1 + teamB + fsizeB, 5);
            vec[3] = RR_vec2(1 + teamB + fsizeB, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 30), RR_vec2(1, 0), 1.0, 0, 0, 155);
            vec[2] = RR_vec2(1 + teamB, 5);
            vec[3] = RR_vec2(1 + teamB, 0);
            RR_g_vec2.draw_polygon(win, vec, 4, RR_vec2(10, 30), RR_vec2(1, 0), 1.0, 0, 0, 205);
        }
        
        // Next battle
        if(teamR + teamG < 1 || teamR + teamB < 1 || teamB + teamG < 1) {
            battle_timeout -= fspd;
            if(battle_timeout <= 0.0) next_battle();
        }
        
        // Done
        return 0;
    }
    
    // Add a fleet to the battlefield
    void addfleet(int fleet_type, int fleet_team) {
        int i1 = 0;
        switch(fleet_type) {
        case 0: // Light patrol
            if(fleet_team == 1) {
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
            } else if(fleet_team == 2) {
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
            } else if(fleet_team == 3) {
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
            }
            break;
        case 1: // Medium patrol
            if(fleet_team == 1) {
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
            } else if(fleet_team == 2) {
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
            } else if(fleet_team == 3) {
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
            }
            break;
        case 2: // Heavy patrol
            if(fleet_team == 1) {
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(1, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
            } else if(fleet_team == 2) {
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(2, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
            } else if(fleet_team == 3) {
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(3, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
            }
            break;
        case 3: // Assault wing
            if(fleet_team == 1) {
                i1 = addship(7, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(7, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
                i1 = addship(4, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
            } else if(fleet_team == 2) {
                i1 = addship(8, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(8, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
                i1 = addship(5, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
            } else if(fleet_team == 3) {
                i1 = addship(9, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(9, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
                i1 = addship(6, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
            }
            break;
        }
    }
    
    // Add a single ship to the battlefield
    int addship(int ship_type, RR_vec2 ship_pos, RR_vec2 ship_nrm, int ifrom) {
        for(int i = ifrom; i < RR_BATTLE_MAX_UNITS; i++) if(!a[i].in_use) {
            a[i] = RR_unit(ship_type, ship_pos, ship_nrm);
            return i + 1;
        }
        return RR_BATTLE_MAX_UNITS;
    }
};

#endif // RR_BATTLE_H
