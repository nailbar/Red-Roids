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
    float trn, size;
    int trg;
    char team, type;
    
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
        team = -1;
        type = -1;
        size = 1.0;
        
        // Ship presets
        from_preset(preset);
    }
    
    // Generate from preset
    void from_preset(unsigned char preset) {
        
        // Reset parts
        for(int i = 0; i < RR_MAX_UNIT_PARTS; i++) p[i] = RR_unit_part();
        
        // Construct unit
        type = preset;
        switch(preset) {
        case 0: // Arrow light fighter
            p[0] = RR_unit_part(2, RR_vec2(0, 0)); // Red small cockpit
            p[1] = RR_unit_part(1, RR_vec2(3, 0)); // Hull
            p[2] = RR_unit_part(0, RR_vec2(-11, 0)); // Engine
            team = 0;
            break;
        case 1: // Bullet light fighter
            p[0] = RR_unit_part(3, RR_vec2(-5, 0)); // Green small cockpit
            p[1] = RR_unit_part(5, RR_vec2(0, 0)); // Hull
            p[2] = RR_unit_part(6, RR_vec2(-15, 0)); // Hull
            p[3] = RR_unit_part(0, RR_vec2(-27, 0)); // Engine
            team = 1;
            break;
        case 2: // Raptor light fighter
            p[0] = RR_unit_part(4, RR_vec2(-5, 0)); // Blue small cockpit
            p[1] = RR_unit_part(7, RR_vec2(0, 10)); // Hull right
            p[2] = RR_unit_part(8, RR_vec2(0, -10)); // Hull left
            p[3] = RR_unit_part(0, RR_vec2(-11, 0)); // Engine
            team = 2;
            break;
        default: // Pod
            p[0] = RR_unit_part(2, RR_vec2(0, 0)); // Red small cockpit
            team = -1;
            type = -1;
        }
        
        // Recalculate size
        recalculate();
    }
    
    // Calculate ships size based on part size and position
    void recalculate() {
        int parts = 0;
        RR_vec2 nose_right = RR_vec2();
        RR_vec2 aft_left = RR_vec2();
        RR_vec2 offset = RR_vec2();
        size = 0;
        float this_distance = 0;
        
        // Check for center
        for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].in_use) {
            if(parts == 0) {
                nose_right.x = p[i].pos.x + p[i].size();
                nose_right.y = p[i].pos.y + p[i].size();
                aft_left.x = p[i].pos.x - p[i].size();
                aft_left.y = p[i].pos.y - p[i].size();
            } else {
                if(p[i].pos.x + p[i].size() > nose_right.x) nose_right.x = p[i].pos.x + p[i].size();
                if(p[i].pos.y + p[i].size() > nose_right.y) nose_right.y = p[i].pos.y + p[i].size();
                if(p[i].pos.x - p[i].size() < aft_left.x) aft_left.x = p[i].pos.x - p[i].size();
                if(p[i].pos.y - p[i].size() < aft_left.y) aft_left.y = p[i].pos.y - p[i].size();
            }
            parts++;
        }
        
        // Recenter ship and calculate size
        offset = (nose_right + aft_left) / 2.0;
        for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].in_use) {
            p[i].pos = p[i].pos - offset;
            this_distance = RR_g_vec2.distance(RR_vec2(), p[i].pos) + p[i].size();
            if(this_distance > size) size = this_distance;
        }
    }
    
    // Check if unit has a valid target
    bool has_valid_target(RR_unit* a, int n, int i) {
        
        // Make sure target is within bounds
        if(trg < 0 || trg >= n) return false;
        
        // Make sure target is in use
        if(!a[trg].in_use) return false;
        
        // Make sure target is not self
        if(trg == i) return false;
        
        // Make sure target is not on same team
        if(team == a[trg].team) return false;
        
        // Target is valid
        return true;
    }
    
    // Follow target unit
    void follow_target(RR_unit* a, int n, int i) {
        burn_eng = 0;
        trn = 0;
        
        // Check target validity
        if(!has_valid_target(a, n, i)) trg = rand() % n;
        
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
        for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].in_use) {
            
            // Draw any part that exists
            p[i].draw(win, position + (normal * p[i].pos.x + normal.extrude() * p[i].pos.y) * scale, normal, scale, p[i].type, burn_eng);
        }
        if(RR_g.debugmode == 3) ellipseRGBA(win, position.x, position.y, size * scale, size * scale, 255, 0, 0, 200); // Show ship size
    }
    
    // Check if another unit is too close and bounce on it
    bool bounce(RR_unit &other) {
        RR_vec2 p1, p2, n;
        
        // Quick box-fit test first
        if(RR_g_vec2.box_distance(pos, other.pos) < size + other.size) {
            
            // Check if any part is actually touching
            for(int i = 0; i < RR_MAX_UNIT_PARTS; i++) if(p[i].in_use) {
                p1 = pos + nrm * p[i].pos.x + nrm.extrude() * p[i].pos.y; // Real position of part
                for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(other.p[u].in_use) {
                    p2 = other.pos + other.nrm * other.p[u].pos.x + other.nrm.extrude() * other.p[u].pos.y; // Real pos
                    n = RR_g_vec2.normal(p1, p2); // Bounce direction
                    if(RR_g_vec2.dot(n, p2 - p1) < p[i].size() + other.p[u].size()) {
                        spd = spd - n * 50.0;
                        other.spd = other.spd + n * 50.0;
                        return true;
                    }
                }
            }
        }
        
        // No hit
        return false;
    }
};

#endif // RR_UNIT_H
