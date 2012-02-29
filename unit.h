#ifndef RR_UNIT_H
#define RR_UNIT_H 1

#include "vector.h"
#include "unit_part.h"

#ifndef RR_MAX_UNIT_PARTS
#define RR_MAX_UNIT_PARTS 10
#endif

class RR_unit {
public:
    RR_vec2 pos, nrm, spd;
    RR_unit_part p[RR_MAX_UNIT_PARTS];
    bool in_use, burn_eng;
    float trn;
    int trg;
    
    // Constructor
    RR_unit() {
        in_use = 0; // Not in use
    }
    RR_unit(unsigned char preset, RR_vec2 newpos) {
        in_use = 1; // In use
        burn_eng = 0;
        pos = newpos;
        nrm = RR_g_vec2.normal(RR_vec2(), RR_g_vec2.box_random());
        spd = RR_vec2();
        trn = 0;
        trg = -1;
        
        // Ship presets
        switch(preset) {
        default:
            p[0] = RR_unit_part(2, RR_vec2(0, 0)); // Cockpit
            p[1] = RR_unit_part(1, RR_vec2(0, 0)); // Hull
            p[2] = RR_unit_part(0, RR_vec2(-11, 0)); // Engine
        }
    }
    
    // Follow target unit
    void follow_target(RR_unit* a, int n, int i) {
        burn_eng = 0;
        trn = 0;
        
        // Make sure target is within bounds
        if(trg < 0 || trg >= n) trg = rand() % n;
        
        // Make sure target is in use
        else if(!a[trg].in_use) trg = rand() % n;
        
        // Make sure target is not self
        else if(trg == i) trg = rand() % n;
        
        // Follow target
        else follow(a[trg].pos);
    }
    
    // Follow target coords
    void follow(RR_vec2 target) {
        
        // Take own speed and distance into account
        double distance = nrm.distance(pos, target);
        if(distance > 400) distance = 400;
        distance = distance / 600.0 + 0.25;
        RR_vec2 target_fix = target - spd * distance;
        
        // Get relative direction to target
        RR_vec2 t_nrm = nrm.normal(pos, target_fix);
        double t_dot = t_nrm.dot(t_nrm, nrm); // How much in front of unit
        double e_dot = t_nrm.dot(t_nrm, nrm.extrude()); // How much beside unit
        
        // Rotate in direction of target
        if(e_dot < 0) trn = -1.0;
        else if(e_dot > 0) trn = 1.0;
        
        // Smoother rotation
        if(t_dot > 0.8) trn = trn * ((1.0 - t_dot) / 0.2);
        
        // Burners on if target in front of ship
        if(t_dot > 0.5) burn_eng = 1;
        else burn_eng = 0;
    }
    
    // Move unit
    void move(float fspd) {
        
        // Turn ship
        nrm = nrm.rotate(nrm, RR_vec2(trn * M_PI * 2.0 * fspd));
        
        // Acceleration
        //  * High acceleration value lowered a lot by friction
        if(burn_eng) spd = spd + nrm * 500.0 * fspd;
        
        // Friction in space
        spd = spd - spd * 0.5 * fspd;
        
        // Move the ship
        pos = pos + spd * fspd;
    }
    
    // Draw the unit on screen
    void draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
        
        // Loop through all parts
        if(in_use) for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].flags & 1) {
            
            // Draw any part that exists
            p[i].draw(win, position + (normal * p[i].pos.x + normal.extrude() * p[i].pos.y) * scale, normal, scale, p[i].type, burn_eng);
        }
    }
    
    // Check if another unit is too close and bounce on it
    void bounce(RR_unit &other) {
        RR_vec2 vec = vec.normal(pos, other.pos);
        if(vec.dot(vec, other.pos - pos) < 20) {
            spd = spd - vec * 50.0;
            other.spd = other.spd + vec * 50.0;
        }
    }
};

#endif // RR_UNIT_H
