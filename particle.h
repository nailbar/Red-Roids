#ifndef RR_PARTICLE_H
#define RR_PARTICLE_H 1

#include "vector.h"

class RR_particle {
public:
    unsigned char type;
    RR_vec2 pos, spd, nrm;
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
            spd = RR_g_vec2.rad_random() * 70.0;
            nrm = RR_g_vec2.random();
            in_use = 1;
            life = (rand() % 3000) / 1000.0;
            break;
        }
    }
    RR_particle(unsigned char newtype, RR_vec2 newpos, RR_vec2 newnrm, RR_vec2 newspd) {
        switch(newtype) {
        case 1: // Light blast (4 seconds)
            type = newtype;
            pos = newpos + newnrm * 4.0; // Out of Light blaster barrel
            spd = newspd + newnrm * 400.0; // Speed of blast plus speed of host
            nrm = newnrm;
            in_use = 1;
            life = 4.0;
            break;
        }
    }
    
    // Draw the particle on screen
    void draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
        RR_vec2 vec[6];
        switch(type) {
        case 0: // Spark (max life 3 sec)
            vec[0] = RR_vec2(-2, -6);
            vec[1] = RR_vec2(2, -6);
            vec[2] = RR_vec2(0, 6);
            if(life <= 3.0) RR_g_vec2.draw_polygon(win, vec, 3, position, normal, scale / (4.0 - life), 255, 255 / (4.0 - life), 0);
            break;
        case 1: // Light blast (max life 4 sec)
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
        }
    }
    
    // Move particle
    void move(float fspd) {
        switch(type) {
        case 0: // Spark (max life 3 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            else if(life <= 3.0) nrm = nrm.rotate(nrm, RR_vec2(M_PI * (12.0 / (4.0 - life)) * fspd));
            break;
        case 1: // Light blast (max life 4 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) in_use = 0;
            break;
        }
    }
    
    // Is this a hitting particle?
    bool hitting() {
        switch(type) {
        case 1: // Light blast (max life 4 sec)
            if(life > 3.8) return false; // Prevent blast from hitting host
            return true;
        default: return false;
        }
    }
    
    // Hit ships
    void hitships(RR_unit* a, int amax, RR_particle* b, int bmax, int current) {
        RR_vec2 p1, n1, p2;
        float d1, d2;
        int i1;
        
        // Skip this if not a hitting particle
        if(!hitting()) return;
        for(int i = 0; i < amax; i++) if(a[i].in_use) if(RR_g_vec2.box_distance(pos, a[i].pos) < a[i].size) {
            
            // Ship is close enough to possibly be hit -> Hit check each part of ship
            d1 = 10000.0;
            i1 = -1;
            for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(a[i].p[u].in_use) {
                p1 = a[i].pos + a[i].nrm * a[i].p[u].pos.x + a[i].nrm.extrude() * a[i].p[u].pos.y; // Part position
                n1 = RR_g_vec2.normal(p1, pos); // Hit direction
                d2 = n1.dot(pos - p1); // Distance from part to particle
                
                // Particle hits part and this part is closer than any other part it also have hit
                if(d2 < a[i].p[u].size() && d2 < d1) {
                    d1 = d2;
                    i1 = u;
                    p2 = p1 + n1 * a[i].p[u].size();
                }
            }
            
            // Did any part get hit?
            if(i1 > -1) {
                in_use = false; // Particle is no more
                
                // Generate sparks
                for(int k = 0; k < 15; k++) for(int j = current; j < RR_BATTLE_MAX_PARTICLES; j++) if(!b[j].in_use) {
                    b[j] = RR_particle(0, p2);
                    current = j + 1;
                    break;
                }
            }
        }
    }
};

#endif // RR_PARTICLE_H
