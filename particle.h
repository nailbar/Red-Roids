#ifndef RR_PARTICLE_H
#define RR_PARTICLE_H 1

#include "vector.h"

class RR_particle {
public:
    unsigned char type, subtype;
    RR_vec2 pos, lpos, spd, nrm;
    bool in_use;
    float life;
    
    // Constructor
    RR_particle() {
        in_use = 0; // Not in use
    }
    RR_particle(unsigned char newtype, RR_vec2 newpos) {
        switch(newtype) {
        case 0: // Spark (max life 3 sec)
            type = newtype;
            pos = newpos;
            lpos = newpos;
            spd = RR_g_vec2.rad_random() * 70.0;
            nrm = RR_g_vec2.random();
            in_use = 1;
            life = (rand() % 3000) / 1000.0;
            break;
        case 2: // Hull fragment (max life 3 sec)
            type = newtype;
            pos = newpos;
            lpos = newpos;
            spd = RR_g_vec2.rad_random() * 70.0;
            nrm = RR_g_vec2.random();
            in_use = 1;
            life = (rand() % 4000) / 1000.0;
            break;
        case 3: // Blast light
            type = newtype;
            pos = newpos;
            lpos = newpos;
            in_use = 1;
            life = 0.25 + (rand() % 1250) / 10000.0;
            break;
        }
    }
    RR_particle(unsigned char newtype, RR_vec2 newpos, RR_vec2 newspddir) {
        switch(newtype) {
        case 0: // Spark (max life 3 sec)
            type = newtype;
            pos = newpos;
            lpos = newpos;
            spd = newspddir * 70.0;
            nrm = RR_g_vec2.random();
            in_use = 1;
            life = (rand() % 3000) / 1000.0;
            break;
        case 2: // Hull fragment (max life 3 sec)
            type = newtype;
            pos = newpos;
            lpos = newpos;
            spd = newspddir * 70.0;
            nrm = RR_g_vec2.random();
            in_use = 1;
            life = (rand() % 4000) / 1000.0;
            break;
        }
    }
    RR_particle(unsigned char newtype, RR_vec2 newpos, RR_vec2 newnrm, RR_vec2 newspd) {
        switch(newtype) {
        case 1: // Light blast (3 seconds)
            type = newtype;
            pos = newpos + newnrm * 5.0; // Out of Light blaster barrel
            lpos = pos;
            spd = newspd + newnrm * 600.0; // Speed of blast plus speed of host
            nrm = newnrm;
            in_use = 1;
            life = 3.0;
            break;
        case 5: // Medium blast (3 seconds)
            type = newtype;
            pos = newpos + newnrm * 10.0; // Out of Medium blaster barrel
            lpos = pos;
            spd = newspd + newnrm * 700.0; // Speed of blast plus speed of host
            nrm = newnrm;
            in_use = 1;
            life = 3.0;
            break;
        }
    }
    RR_particle(unsigned char newtype, RR_vec2 newpos, RR_vec2 newnrm, RR_vec2 newspd, unsigned char newsubtype) {
        switch(newtype) {
        case 4: // Destroyed part (30 seconds)
            type = newtype;
            subtype = newsubtype;
            pos = newpos;
            lpos = pos;
            spd = newspd + RR_g_vec2.rad_random() * 50.0;
            nrm = newnrm;
            in_use = 1;
            life = 10.0 + float(rand() % 20);
            break;
        }
    }
    
    // Draw the particle on screen
    void draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
        RR_g.t[RR_T_PARTICLE_DRAW]++;
        RR_vec2 vec[6];
        RR_unit_part p;
        switch(type) {
        case 0: // Spark (max life 3 sec)
            vec[0] = RR_vec2(-2, -6);
            vec[1] = RR_vec2(2, -6);
            vec[2] = RR_vec2(0, 6);
            if(life <= 3.0) RR_g_vec2.draw_polygon(win, vec, 3, position, normal, scale / (4.0 - life), 255, 255 / (4.0 - life), 0);
            break;
        case 1: // Light blast (max life 3 sec)
            vec[0] = RR_vec2(2, 0);
            vec[1] = RR_vec2(1, -2);
            vec[2] = RR_vec2(-1, -2);
            vec[3] = RR_vec2(-8, 0);
            vec[4] = RR_vec2(-1, 2);
            vec[5] = RR_vec2(1, 2);
            if(life < 2.0) RR_g_vec2.draw_polygon(win, vec, 6, position, normal, scale, 120 * life * 0.5, 255 * life * 0.5, 0);
            else RR_g_vec2.draw_polygon(win, vec, 6, position, normal, scale, 120, 255, 0);
            vec[0] = RR_vec2(-1, -1);
            vec[1] = RR_vec2(1, -1);
            vec[2] = RR_vec2(1, 1);
            vec[3] = RR_vec2(-1, 1);
            RR_g_vec2.draw_polygon(win, vec, 4, position, normal, scale, 200, 255, 0);
            break;
        case 2: // Hull fragment (max life 4 sec)
            vec[0] = RR_vec2(-3, -5);
            vec[1] = RR_vec2(2, -4);
            vec[2] = RR_vec2(0, 5);
            vec[3] = RR_vec2(-3, 4);
            if(life < 1.0) RR_g_vec2.draw_polygon(win, vec, 4, position, normal, RR_vec2(1, 0), RR_vec2(1, 0), scale, 255 * life, 255 * life, 255 * life, 0.2, 0.3);
            else RR_g_vec2.draw_polygon(win, vec, 4, position, normal, RR_vec2(1, 0), RR_vec2(1, 0), scale, 180, 180, 200, 0.2, 0.3);
            break;
        case 3: // Blast light
            vec[0] = RR_vec2(-40, -20);
            vec[1] = RR_vec2(-40, 20);
            vec[2] = RR_vec2(0, 40);
            vec[3] = RR_vec2(40, 20);
            vec[4] = RR_vec2(40, -20);
            vec[5] = RR_vec2(0, -40);
            RR_g_vec2.draw_polygon(win, vec, 6, position, RR_vec2(life * 10.0), scale * life, 255, 255, 255);
            break;
        case 4: // Destroyed part
            p.draw(win, position, normal, scale, subtype, 0, RR_vec2(1,0));
            break;
        case 5: // Medium blast (max life 3 sec)
            vec[0] = RR_vec2(3, 0);
            vec[1] = RR_vec2(1, -3);
            vec[2] = RR_vec2(-1, -3);
            vec[3] = RR_vec2(-11, 0);
            vec[4] = RR_vec2(-1, 3);
            vec[5] = RR_vec2(1, 3);
            if(life < 2.0) RR_g_vec2.draw_polygon(win, vec, 6, position, normal, scale, 0, 120 * life * 0.5, 255 * life * 0.5);
            else RR_g_vec2.draw_polygon(win, vec, 6, position, normal, scale, 0, 120, 255);
            vec[0] = RR_vec2(-1.5, -1.5);
            vec[1] = RR_vec2(1.5, -1.5);
            vec[2] = RR_vec2(1.5, 1.5);
            vec[3] = RR_vec2(-1.5, 1.5);
            RR_g_vec2.draw_polygon(win, vec, 4, position, normal, scale, 0, 200, 255);
            break;
        }
    }
    
    // Move particle
    void move(float fspd) {
        lpos = pos;
        switch(type) {
        case 0: // Spark (max life 3 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            else if(life <= 3.0) nrm = nrm.rotate(nrm, RR_vec2(M_PI * (12.0 / (4.0 - life)) * fspd));
            break;
        case 1: // Light blast (max life 3 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            break;
        case 2: // Hull fragment (max life 4 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            else if(life <= 4.0) nrm = nrm.rotate(nrm, RR_vec2(M_PI * (12.0 / (5.0 - life)) * fspd));
            break;
        case 3: // Blast light
            life -= fspd;
            if(life < 0.0) in_use = 0;
            break;
        case 4: // Destroyed part
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) {
                life = 0.25;
                type = 3;
            }
            nrm = nrm.rotate(nrm, RR_vec2(M_PI * fspd));
            break;
        case 5: // Medium blast (max life 3 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            break;
        }
    }
    
    // Get hit damage for particle
    float hitdamage() {
        switch(type) {
        case 1: // Light blast (max life 3 sec)
            if(life > 2.85) return 0; // Prevent blast from hitting host
            return 7.0; // Enough to destroy a light thruster on first hit if lucky
        case 5: // Medium blast (max life 3 sec)
            if(life > 2.85) return 0; // Prevent blast from hitting host
            return 15.0;
        default: return 0;
        }
    }
    
    // Hit ships
    void hitships(RR_unit* a, int amax, RR_particle* b, int bmax, int current) {
        RR_g.t[RR_T_PARTICLE_HIT]++;
        RR_vec2 p1, n1, p2, inter_pos, top_pos;
        float inter_dis;
        int top_i, top_u;
        float top_dis = -1;
        
        // Skip this if not a hitting particle
        if(hitdamage() <= 0.0) return;
        for(int i = 0; i < amax; i++) if(a[i].in_use) {
            
            // Quick test
            if(RR_g_vec2.box_distance(pos, a[i].pos) < a[i].size) {
                RR_g.t[RR_T_PARTICLE_HIT2]++;
                
                // Ship is close enough to possibly be hit -> Hit check each part of ship
                for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(a[i].p[u].in_use) {
                    
                    // Calculate part position
                    p1 = a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y;
                    
                    // Test if part is hit
                    if(a[i].p[u].intersect(a[i].p[u].type, p1, a[i].nrm, lpos - (nrm * 5.0), pos, inter_pos, inter_dis)) {
                        if(top_dis < 0 || inter_dis < top_dis) {
                            top_dis = inter_dis;
                            top_pos = inter_pos;
                            top_i = i;
                            top_u = u;
                        }
                    }
                }
            }
        }
        
        // Did any ship get hit?
        if(top_dis > 0) {
            in_use = false; // Particle is no more
            
            // Damage and optionally destroy part
            a[top_i].p[top_u].health -= ((rand() % 10000) / 10000.0) * hitdamage();
            if(a[top_i].p[top_u].health <= 0.0) {
                a[top_i].p[top_u].in_use = false;
                a[top_i].recalculate();
            }
            
            // Calculate direction of explosion
            p1 = a[top_i].pos + a[top_i].nrm * a[top_i].p[top_u].pos.x + a[top_i].nrm.extrude() * a[top_i].p[top_u].pos.y;
            n1 = RR_g_vec2.normal(p1, top_pos);
            
            // Generate sparks and small parts
            for(int k = 0; k < 15; k++) for(int j = current; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                if(k == 0) b[j] = RR_particle(3, top_pos); // Light pulse
                else if(!a[top_i].p[top_u].in_use && k == 1) b[j] = RR_particle( // Destroyed part
                    4,
                    p1,
                    a[top_i].nrm,
                    a[top_i].spd,
                    a[top_i].p[top_u].type
                );
                else if(rand() % 100 < 80) b[j] = RR_particle(0, top_pos, n1 + RR_g_vec2.rad_random()); // Sparks
                else b[j] = RR_particle(2, top_pos, n1 + RR_g_vec2.rad_random()); // Fragments
                current = j + 1;
                break;
            }
        }
    }
};

#endif // RR_PARTICLE_H
