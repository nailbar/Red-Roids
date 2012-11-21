#include <stdio.h>

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
#define RR_BATTLE_REINFORCEMENT_INTERVAL 40
#endif

#include "text.h"
#include "unit.h"
#include "particle.h"
#include "starfield.h"

class RR_battle {
public:
    RR_unit a[RR_BATTLE_MAX_UNITS]; // Units currently on battlefield
    RR_particle b[RR_BATTLE_MAX_PARTICLES]; // Particles on battlefield
    RR_starfield stars; // Background starfield
    RR_vec2 cam, cam_trg, teamR_pos, teamG_pos, teamB_pos, top_left, bottom_right;
    double zoom, zoom_trg;
    float reinforcements, player_timeout, battle_timeout;
    int next_particle, player, player_team, wings;
    bool zoom_key, zoom_toggle;
    char battle_mode;
    
    // Fleets
    unsigned char fleetR[RR_BATTLE_MAX_FLEET];
    int fsizeR, techR;
    unsigned char fleetG[RR_BATTLE_MAX_FLEET];
    int fsizeG, techG;
    unsigned char fleetB[RR_BATTLE_MAX_FLEET];
    int fsizeB, techB;
    
    // Constructor
    RR_battle() {
        player_timeout = 0.0;
        top_left = RR_vec2(-10, -10);
        bottom_right = RR_vec2(10, 10);
        teamR_pos = RR_vec2(0) * RR_BATTLE_FIELD_LIMIT;
        teamG_pos = RR_vec2(0.3 * M_PI * 2.0) * RR_BATTLE_FIELD_LIMIT;
        teamB_pos = RR_vec2(0.6 * M_PI * 2.0) * RR_BATTLE_FIELD_LIMIT;
        zoom_key = false;
        zoom_toggle = false;
        player_team = rand() % 3;
        battle_mode = 0;
        
        // Init the battle
        next_battle();
    }
    
    void next_battle() {
        next_battle(false);
    }
    void next_battle(bool random_team) {
        cam = RR_vec2();
        cam_trg = cam;
        zoom = 0.001;
        zoom_trg = zoom;
        
        // Clear field
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) a[i].in_use = false;
        next_particle = 0;
        player = rand() % RR_BATTLE_MAX_UNITS;
        reinforcements = RR_BATTLE_REINFORCEMENT_INTERVAL;
        battle_timeout = 5.0;
        if(random_team) {
            player_team = rand() % 3;
        
            // Set battle mode
            battle_mode = rand() % 2;
        }
        switch(battle_mode) {
            
        // 1 = survival (Survive increasingly difficult attacks)
        case 1:
            techR = 2;
            techG = 2;
            techB = 2;
            fsizeR = RR_BATTLE_MAX_FLEET;
            fsizeG = RR_BATTLE_MAX_FLEET;
            fsizeB = RR_BATTLE_MAX_FLEET;
            for(int i = 0; i < RR_BATTLE_MAX_FLEET; i++) {
                if(i < fsizeR) fleetR[i] = RR_BATTLE_MAX_FLEET - i;
                if(i < fsizeG) fleetG[i] = RR_BATTLE_MAX_FLEET - i;
                if(i < fsizeB) fleetB[i] = RR_BATTLE_MAX_FLEET - i;
            }
            
            // Add player
            switch(player_team) {
            case 0: techR = RR_g_data.maxunit - 1; addfleet(1, player_team); break;
            case 1: techG = RR_g_data.maxunit - 1; addfleet(1, player_team); break;
            case 2: techB = RR_g_data.maxunit - 1; addfleet(1, player_team); break;
            }
            break;
        
        // 0 = normal skirmish (three random fleets attacking each other)
        default:
            fsizeR = (rand() % 10) + 1;
            fsizeG = (rand() % 10) + 1;
            fsizeB = (rand() % 10) + 1;
            techR = rand() % RR_g_data.maxunit + 1;
            techG = rand() % RR_g_data.maxunit + 1;
            techB = rand() % RR_g_data.maxunit + 1;
            for(int i = 0; i < RR_BATTLE_MAX_FLEET; i++) {
                if(i < fsizeR) fleetR[i] = rand() % 6 + 2;
                if(i < fsizeG) fleetG[i] = rand() % 6 + 2;
                if(i < fsizeB) fleetB[i] = rand() % 6 + 2;
            }
        }
    }
    
    // Main battle loop
    bool main(SDL_Surface* win, float fspd, Uint8* keys) {
        int i1, i2;
        float f1;
        bool first_ship = 0;
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
        
        // Show all ships
        if(keys[SDLK_z]) zoom_key = true;
        else if(zoom_key) {
            zoom_key = false;
            if(zoom_toggle) zoom_toggle = false;
            else zoom_toggle = true;
        }
        if(zoom_toggle || a[player].team != player_team || !a[player].in_use) {
            cam_trg = (top_left + bottom_right) / 2.0;
            zoom_trg = (RR_g.wid / (bottom_right.x - top_left.x)) / 1.5;
            f1 = (RR_g.hgt / (bottom_right.y - top_left.y)) / 1.5;
            if(f1 < zoom_trg) zoom_trg = f1;
        }
        
        // Smooth camera transitions
        cam = cam + (cam_trg - cam) * 0.04;
        zoom = zoom + (zoom_trg - zoom) * 0.04;
        
        // Draw starfield
        stars.draw(win, cam, zoom);
        
        // Loop through ships
        for(int i = 0; i < RR_BATTLE_MAX_UNITS; i++) if(a[i].in_use) {
            if(!first_ship) {
                first_ship = true;
                top_left = RR_vec2(
                    a[i].pos.x - a[i].size * 3.0,
                    a[i].pos.y - a[i].size * 3.0
                );
                bottom_right = RR_vec2(
                    a[i].pos.x + a[i].size * 3.0,
                    a[i].pos.y + a[i].size * 3.0
                );
            }
            if(a[i].pos.x < top_left.x) top_left.x = a[i].pos.x;
            if(a[i].pos.y < top_left.y) top_left.y = a[i].pos.y;
            if(a[i].pos.x > bottom_right.x) bottom_right.x = a[i].pos.x;
            if(a[i].pos.y > bottom_right.y) bottom_right.y = a[i].pos.y;
            
            // Count ships
            switch(a[i].team) {
            case 0: teamR++; break;
            case 1: teamG++; break;
            case 2: teamB++; break;
            }
            
            // All ships are attracted to center of battlefield
            a[i].spd = a[i].spd - a[i].pos * 0.005 * fspd;
            
            // Display ship
            a[i].draw(win, (a[i].pos - cam) * zoom + RR_vec2(RR_g.cntx, RR_g.cnty), a[i].nrm, zoom);
            
            // Update nearest target id
            a[i].find_nearest_target(a, RR_BATTLE_MAX_UNITS, i);
            
            // Player controls this ship
            if(i == player && a[i].team == player_team) {
                if(a[i].player_input(keys)) player_timeout = 15.0;
                else if(player_timeout > 0.0) player_timeout -= fspd;
                else a[i].follow_target(a, RR_BATTLE_MAX_UNITS, i);
                
                // Find better target
                if(keys[SDLK_t] || keys[SDLK_KP0] || RR_g_vec2.distance(a[i].pos, a[a[i].trg].pos) > 1500.0) a[i].find_better_target(a, RR_BATTLE_MAX_UNITS, i);
                
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
                if(a[i].guns) a[i].p[u].cool(fspd, a[i].power);
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
            for(int u = i + 1; u < RR_BATTLE_MAX_UNITS; u++) if((i2 = a[i].bounce(a[u], true, true)) > 0) {
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
            if(a[i].p[i1].in_use) if(!a[i].p[a[i].p[i1].parent].in_use) {
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
            
            // Self destruction
            if(a[i].suicide_timer(fspd)) {
                for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(a[i].p[u].in_use) {
                    for(int j = next_particle; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                        b[j] = RR_particle( // Destroyed part
                            4,
                            a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y,
                            a[i].nrm,
                            a[i].spd,
                            a[i].p[u].type
                        );
                        next_particle = j + 1;
                        break;
                    }
                    for(int k = 0; k < a[i].p[u].size() * 3.0; k++) for(int j = next_particle; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                        if(k == 0) b[j] = RR_particle(3, a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y); // Light pulse
                        else if(rand() % 100 < 80) b[j] = RR_particle(0, a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y); // Sparks
                        else b[j] = RR_particle(2, a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y); // Fragments
                        next_particle = j + 1;
                        break;
                    }
                }
                a[i].in_use = false;
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
        
        // Target status indicator
        char str[128];
        f1 = RR_g.wid / 1000.0;
        if(a[player].in_use && a[player].team == player_team) {
            a[player].draw(
                win,
                RR_vec2(
                    50 + a[player].size * f1,
                    RR_g.hgt - 50 - a[player].size * f1
                ),
                a[player].nrm,
                f1,
                RR_vec2(0, -1),
                true
            );
            if(a[player].has_valid_target(a, RR_BATTLE_MAX_UNITS, player)) {
                a[a[player].trg].draw(
                    win,
                    RR_vec2(
                        RR_g.wid - 50 - a[a[player].trg].size * f1,
                        RR_g.hgt - 50 - a[a[player].trg].size * f1
                    ),
                    a[a[player].trg].nrm,
                    f1,
                    RR_vec2(0, -1),
                    true
                );
                sprintf(str, "Distance: %d", int(RR_g_vec2.distance(a[player].pos, a[a[player].trg].pos) * 0.01));
                RR_g_text.draw(win, RR_vec2(RR_g.wid - 100 * f1, RR_g.hgt - 20 * f1), 2.0 * f1, 120, 160, 205, str);
            }
        }
        
        // Spawn reinforcements
        switch(battle_mode) {
        case 1:
            
            // Next battle if player is dead
            if((teamR < 1 && player_team == 0) || (teamG < 1 && player_team == 1) || (teamB < 1 && player_team == 2)) {
                battle_timeout -= fspd;
                if(battle_timeout <= 0.0) next_battle();
            
            // Next wave if enemies are dead
            } else if(teamG < 1 && player_team == 0 && fsizeG > 0) {
                fsizeG--;
                teamG += addfleet(fleetG[fsizeG], 1);
                if(techG < RR_g_data.maxunit) techG++;
            } else if(teamB < 1 && player_team == 1 && fsizeB > 0) {
                fsizeB--;
                teamB += addfleet(fleetG[fsizeB], 2);
                if(techB < RR_g_data.maxunit) techB++;
            } else if(teamR < 1 && player_team == 2 && fsizeR > 0) {
                fsizeR--;
                teamR += addfleet(fleetR[fsizeR], 0);
                if(techR < RR_g_data.maxunit) techR++;
            }
            break;
        default:
            if(reinforcements <= 0.0 || (teamR < 4 && fsizeR) || (teamG < 4 && fsizeG) || (teamB < 4 && fsizeB)) {
                if(teamR < teamG && teamR < teamB && fsizeR) {
                    fsizeR--;
                    teamR += addfleet(fleetR[fsizeR], 0);
                } else if(teamG < teamB && fsizeG) {
                    fsizeG--;
                    teamG += addfleet(fleetG[fsizeG], 1);
                } else if(fsizeB) {
                    fsizeB--;
                    teamB += addfleet(fleetB[fsizeB], 2);
                } else if(fsizeR) {
                    fsizeR--;
                    teamR += addfleet(fleetR[fsizeR], 0);
                } else if(fsizeG) {
                    fsizeG--;
                    teamG += addfleet(fleetG[fsizeG], 1);
                }
                if(teamR > 0 && teamG > 0 && teamB > 0) reinforcements = RR_BATTLE_REINFORCEMENT_INTERVAL * ((rand() % 10000) / 10000.0);
            } else reinforcements -= fspd;
            
            // Team status indicators
            RR_g_text.draw(win, RR_vec2(10, 10), 2.0 * f1, 120, 160, 205, "Ships on field:\nReinforcements:\nTech level:");
            sprintf(str, "                 %d\n                 %d\n                 %d", teamR, fsizeR, techR);
            RR_g_text.draw(win, RR_vec2(10, 10), 2.0 * f1, 255, 0, 0, str);
            sprintf(str, "                    %d\n                    %d\n                    %d", teamG, fsizeG, techG);
            RR_g_text.draw(win, RR_vec2(10, 10), 2.0 * f1, 0, 200, 0, str);
            sprintf(str, "                       %d\n                       %d\n                       %d", teamB, fsizeB, techB);
            RR_g_text.draw(win, RR_vec2(10, 10), 2.0 * f1, 50, 100, 255, str);
            if(fsizeR || fsizeG || fsizeB) {
                sprintf(str, "\n                          %d", int(reinforcements));
                RR_g_text.draw(win, RR_vec2(10, 10), 2.0 * f1, 150, 150, 150, str);
            }
            
            // Next battle
            if(teamR + teamG < 1 || teamR + teamB < 1 || teamB + teamG < 1) {
                battle_timeout -= fspd;
                if(battle_timeout <= 0.0) next_battle();
            }
        }
        
//         // Dev tests
//         RR_unit_part testpart;
//         testpart.draw(win, RR_vec2(200, 200), a[player].nrm, 2.0, 20, 0, RR_vec2(0), false);
//         testpart.draw(win, RR_vec2(200, 300), a[player].nrm, 2.0, 21, 0, RR_vec2(0), false);
        
        // Done
        return 0;
    }
    
    // Add a fleet to the battlefield
    int addfleet(int fleet_type, int fleet_team) {
        int i1 = 0;
        int added = 0;
        for(int i = 0; i < fleet_type; i++) {
            added++;
            if(fleet_team == 0) {
                i1 = addship((rand() % techR) + 1, fleet_team, teamR_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamR_pos, RR_vec2()), i1);
            } else if(fleet_team == 1) {
                i1 = addship((rand() % techG) + 1, fleet_team, teamG_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamG_pos, RR_vec2()), i1);
            } else if(fleet_team == 2) {
                i1 = addship((rand() % techB) + 1, fleet_team, teamB_pos + RR_g_vec2.box_random() * 500.0, RR_g_vec2.normal(teamB_pos, RR_vec2()), i1);
            }
        }
        return added;
    }
    
    // Add a single ship to the battlefield
    int addship(int ship_type, char ship_team, RR_vec2 ship_pos, RR_vec2 ship_nrm, int ifrom) {
//         std::cout<<"addship: "<<ship_type<<", "<<int(ship_team)<<"\n";
        for(int i = ifrom; i < RR_BATTLE_MAX_UNITS; i++) if(!a[i].in_use) {
            a[i] = RR_unit(ship_type, ship_team, ship_pos, ship_nrm);
            return i + 1;
        }
        return RR_BATTLE_MAX_UNITS;
    }
};

#endif // RR_BATTLE_H
