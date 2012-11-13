#ifndef RR_UNIT_H
#define RR_UNIT_H 1

#include "vector.h"
#include "unit_part.h"

#ifndef RR_MAX_UNIT_PARTS
#define RR_MAX_UNIT_PARTS 20
#endif

#ifndef RR_BOUNCE_DAMAGE
#define RR_BOUNCE_DAMAGE 0.005
#endif

#ifndef RR_UNIT_MAX_TECH
#define RR_UNIT_MAX_TECH 7
#endif

class RR_unit {
public:
    RR_vec2 pos, nrm, spd, tmp_vec2;
    RR_unit_part p[RR_MAX_UNIT_PARTS];
    bool in_use, burn_eng, fire, guns;
    float trn, trn2, size, thrust, weight, power, power_gen, power_draw, timeout, nearest_dis, self_destoy;
    int trg, nearest_trg, test_trg;
    char team, type, mode;
    
    // Constructor
    RR_unit();
    RR_unit(unsigned char, char, RR_vec2);
    RR_unit(unsigned char, char, RR_vec2, RR_vec2);
    
    // Generate from preset
    void from_preset(unsigned char, char);
    
    // Calculate ship details based on part data
    void recalculate();
    
    // Find a better target
    void find_better_target(RR_unit*, int, int);
    
    // Check if unit has a valid target
    bool has_valid_target(RR_unit*, int, int);
    
    // Look for nearest target
    void find_nearest_target(RR_unit*, int, int);
    
    // Follow target unit
    void follow_target(RR_unit*, int, int);
    
    // Follow target coords
    void follow(RR_vec2, bool);
    
    // Move unit
    void move(float);
    
    // Draw the unit on screen
    void draw(SDL_Surface*, RR_vec2, RR_vec2, float, RR_vec2, bool);
    void draw(SDL_Surface*, RR_vec2, RR_vec2, float, RR_vec2);
    void draw(SDL_Surface*, RR_vec2, RR_vec2, float);
    
    // Check if another unit is too close and bounce on it
    float bounce(RR_unit &, bool, bool);
    
    // Control ship by player input
    bool player_input(Uint8*);
    
    // Draw a target pointer
    void target_pointer(SDL_Surface*, RR_vec2, RR_vec2, float, float);
    
    // Draw a target indicator
    void target_indicator(SDL_Surface*, RR_vec2, float, float);
    
    // Check if ship will self destruct
    bool suicide_timer(float);
};

// Constructor
RR_unit::RR_unit() {
    in_use = 0; // Not in use
}
RR_unit::RR_unit(unsigned char preset, char newteam, RR_vec2 newpos) {
    in_use = 1; // In use
    burn_eng = 0;
    fire = 0;
    pos = newpos;
    nrm = RR_g_vec2.normal(RR_vec2(), RR_g_vec2.box_random());
    spd = RR_vec2();
    trn = 0;
    trn2 = 0;
    trg = -1;
    nearest_trg = -1;
    test_trg = 0;
    nearest_dis = -1;
    team = newteam;
    type = -1;
    size = 1.0;
    thrust = 0.0;
    weight = 1.0;
    power = 0.0;
    power_gen = 0.0;
    power_draw = 0.0;
    guns = 0;
    mode = 0;
    timeout = 1.0;
    self_destoy = -1;
    
    // Ship presets
    from_preset(preset, team);
}
RR_unit::RR_unit(unsigned char preset, char newteam, RR_vec2 newpos, RR_vec2 newnrm) {
    in_use = 1; // In use
    burn_eng = 0;
    fire = 0;
    pos = newpos;
    nrm = newnrm;
    spd = RR_vec2();
    trn = 0;
    trn2 = 0;
    trg = -1;
    nearest_trg = -1;
    test_trg = 0;
    nearest_dis = -1;
    team = newteam;
    type = -1;
    size = 1.0;
    thrust = 0.0;
    weight = 1.0;
    power = 0.0;
    power_gen = 0.0;
    power_draw = 0.0;
    guns = 0;
    mode = 0;
    timeout = 1.0;
    self_destoy = -1;
    
    // Ship presets
    from_preset(preset, team);
}

// Generate from preset
void RR_unit::from_preset(unsigned char preset, char newteam) {
    
    // Reset parts
    for(int i = 0; i < RR_MAX_UNIT_PARTS; i++) p[i] = RR_unit_part();
    
    // Construct unit
    type = preset;
    team = newteam % 3;
    switch(preset) {
    case 1: // Scout 1
        p[0] = RR_unit_part(2 + team, RR_vec2(-1, 0), 1); // Cockpit
        p[1] = RR_unit_part(5, RR_vec2(4, 0), 4); // Hull
        p[2] = RR_unit_part(0, RR_vec2(-13, 0), 4); // Thruster
        p[3] = RR_unit_part(12, RR_vec2(28, 0), 1); // Blaster
        p[4] = RR_unit_part(13, RR_vec2(0, 0), 0); // Core
        break;
    case 2: // Scout 2
        p[0] = RR_unit_part(2 + team, RR_vec2(1, 0), 5); // Cockpit
        p[1] = RR_unit_part(12, RR_vec2(10, 0), 0); // Blaster
        p[2] = RR_unit_part(7, RR_vec2(5, 9), 5); // Wing
        p[3] = RR_unit_part(8, RR_vec2(5, -9), 5); // Wing
        p[4] = RR_unit_part(0, RR_vec2(-12, 0), 5); // Thruster
        p[5] = RR_unit_part(13, RR_vec2(0, 0), 0); // Core
        break;
    case 3: // Light fighter 1
        p[0] = RR_unit_part(2 + team, RR_vec2(-4, 0), 8); // Cockpit
        p[1] = RR_unit_part(9, RR_vec2(16, 0), 8); // Hull
        p[2] = RR_unit_part(10, RR_vec2(-7, 8), 8); // Wing
        p[3] = RR_unit_part(11, RR_vec2(-7, -8), 8); // Wing
        p[4] = RR_unit_part(0, RR_vec2(-13, 6), 2); // Thruster
        p[5] = RR_unit_part(0, RR_vec2(-13, -6), 3); // Thruster
        p[6] = RR_unit_part(12, RR_vec2(-15, 18), 2); // Blaster
        p[7] = RR_unit_part(12, RR_vec2(-15, -18), 3); // Blaster
        p[8] = RR_unit_part(13, RR_vec2(0, 0), 0); // Core
        break;
    case 4: // Light fighter 2
        p[0] = RR_unit_part(2 + team, RR_vec2(-4, 0), 7); // Cockpit
        p[1] = RR_unit_part(1, RR_vec2(11, 0), 7); // Hull
        p[2] = RR_unit_part(7, RR_vec2(-13, 10), 7); // Wing
        p[3] = RR_unit_part(8, RR_vec2(-13, -10), 7); // Wing
        p[4] = RR_unit_part(0, RR_vec2(-12, 0), 7); // Thruster
        p[5] = RR_unit_part(12, RR_vec2(-6, 15), 2); // Blaster
        p[6] = RR_unit_part(12, RR_vec2(-6, -15), 3); // Blaster
        p[7] = RR_unit_part(13, RR_vec2(0, 0), 0); // Core
        break;
    case 5: // Medium fighter 1
        p[0] = RR_unit_part(2 + team, RR_vec2(0, 0), 12); // Cockpit
        p[1] = RR_unit_part(7, RR_vec2(26, 17), 3); // Wing
        p[2] = RR_unit_part(8, RR_vec2(26, -17), 4); // Wing
        p[3] = RR_unit_part(5, RR_vec2(8, 12), 12); // Hull
        p[4] = RR_unit_part(5, RR_vec2(8, -12), 13); // Hull
        p[5] = RR_unit_part(10, RR_vec2(-11, 17), 3); // Wing
        p[6] = RR_unit_part(11, RR_vec2(-11, -17), 4); // Wing
        p[7] = RR_unit_part(0, RR_vec2(-11, 6), 12); // Thruster
        p[8] = RR_unit_part(0, RR_vec2(-11, -6), 13); // Thruster
        p[9] = RR_unit_part(12, RR_vec2(12, 0), 0); // Blaster
        p[10] = RR_unit_part(12, RR_vec2(33, 9), 3); // Blaster
        p[11] = RR_unit_part(12, RR_vec2(33, -9), 4); // Blaster
        p[12] = RR_unit_part(13, RR_vec2(1, 6), 13); // Core
        p[13] = RR_unit_part(13, RR_vec2(1, -6), 0); // Core
        break;
    case 6: // Medium fighter 2
        p[0] = RR_unit_part(2 + team, RR_vec2(0, 0), 2); // Cockpit
        p[1] = RR_unit_part(1, RR_vec2(26, 0), 11); // Hull
        p[2] = RR_unit_part(9, RR_vec2(-4, 0), 10); // Hull
        p[3] = RR_unit_part(5, RR_vec2(-1, 9), 2); // Hull
        p[4] = RR_unit_part(5, RR_vec2(-1, -9), 2); // Hull
        p[5] = RR_unit_part(0, RR_vec2(-23, 7), 2); // Thruster
        p[6] = RR_unit_part(0, RR_vec2(-23, -7), 2); // Thruster
        p[7] = RR_unit_part(12, RR_vec2(48, 0), 1); // Blaster
        p[8] = RR_unit_part(12, RR_vec2(23, 11), 3); // Blaster
        p[9] = RR_unit_part(12, RR_vec2(23, -11), 4); // Blaster
        p[10] = RR_unit_part(13, RR_vec2(0, 0), 11); // Core
        p[11] = RR_unit_part(13, RR_vec2(18, 0), 0); // Core
        break;
    case 7: // Heavy fighter 1
        p[0] = RR_unit_part(2 + team, RR_vec2(0, 0), 2); // Cockpit
        p[1] = RR_unit_part(9, RR_vec2(17, 0), 2); // Hull
        p[2] = RR_unit_part(6, RR_vec2(-5, 0), 12); // Hull
        p[3] = RR_unit_part(5, RR_vec2(-16, 15), 2); // Hull
        p[4] = RR_unit_part(5, RR_vec2(-16, -15), 2); // Hull
        p[5] = RR_unit_part(0, RR_vec2(-30, 15), 3); // Thruster
        p[6] = RR_unit_part(0, RR_vec2(-30, -15), 4); // Thruster
        p[7] = RR_unit_part(12, RR_vec2(9, 14), 3); // Blaster
        p[8] = RR_unit_part(12, RR_vec2(9, 18), 3); // Blaster
        p[9] = RR_unit_part(12, RR_vec2(9, -14), 4); // Blaster
        p[10] = RR_unit_part(12, RR_vec2(9, -18), 4); // Blaster
        p[11] = RR_unit_part(13, RR_vec2(9, 0), 0); // Core
        p[12] = RR_unit_part(13, RR_vec2(-9, 0), 11); // Core
        break;
    default: // Pod
        p[0] = RR_unit_part(2 + team, RR_vec2(0, 0), 0); // Small cockpit
        break;
    }
    
    // Recalculate size, power, thrust, etc
    power_draw = 0.0;
    recalculate();
}

// Calculate ship details based on part data
void RR_unit::recalculate() {
    int parts = 0;
    RR_vec2 nose_right = RR_vec2();
    RR_vec2 aft_left = RR_vec2();
    RR_vec2 offset = RR_vec2();
    size = 0;
    guns = 0;
    thrust = 0.0;
    weight = 0.0;
    power_gen = 0.0;
    float this_distance = 0;
    bool fresh_ship = (power_draw <= 0.0);
    
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
        
        // Get thrust and weight for part
        thrust += p[i].thrust();
        weight += p[i].weight();
        power_gen += p[i].power();
        if(fresh_ship) power_draw += p[i].power_draw();
        if(p[i].weapon(true)) guns++;
    }
    
    // Remove ship if no parts
    if(!parts) {
        in_use = false;
        return;
    }
    
    // Calculate power distribution
    power = 1.0; // Enough power
    if(power_gen <= 0.0) power = 0.0; // No power
    if(power_draw > power_gen) power = power_gen / power_draw; // Power draw more than power generation
    if(thrust) thrust = thrust * power;
    
    // If ship is too slow or has no guns it will self destruct
    if(thrust / weight < 0.35 || guns == 0) if(self_destoy < 0) self_destoy = 1.0;
    
    // Recenter ship and calculate size
    offset = (nose_right + aft_left) / 2.0;
    for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].in_use) {
        p[i].pos = p[i].pos - offset;
        this_distance = RR_g_vec2.distance(RR_vec2(), p[i].pos) + p[i].size();
        if(this_distance > size) size = this_distance;
    }
}

// Find a better target
void RR_unit::find_better_target(RR_unit* a, int n, int i) {
    int newtrg = rand() % n;
    
    // Use nearest target
    if(nearest_trg > -1 && nearest_trg < n) newtrg = nearest_trg;
    if(newtrg == trg) return;
    
    // Check if new target is valid and closer than old
    if(has_valid_target(a, n, i)) {
        
        // Make sure new target is in use
        if(!a[newtrg].in_use) return;
        
        // Make sure new target is not self
        if(newtrg == i) return;
        
        // Make sure new target is not on same team
        if(team == a[newtrg].team) return;
        
        // Make sure new target is closer than old
        if(RR_g_vec2.distance(pos, a[trg].pos) > RR_g_vec2.distance(pos, a[newtrg].pos)) trg = newtrg;
        
    // Previous target invalid, no tests required
    } else trg = newtrg;
}

// Check if unit has a valid target
bool RR_unit::has_valid_target(RR_unit* a, int n, int i) {
    
    // Make sure target is within bounds
    if(trg < 0 || trg >= n) return false;
    
    // Make sure target is in use
    if(!a[trg].in_use) return false;
    
    // Make sure target is not self
    if(trg == i) return false;
    
    // Make sure target is not on same team
    if(team == a[trg].team) return false;
    
    // Make sure target is a threat
    if(a[trg].self_destoy > 0) return false;
    
    // Target is valid
    return true;
}

// Look for nearest target
void RR_unit::find_nearest_target(RR_unit* a, int n, int i) {
    RR_vec2 v1;
    if(test_trg < 0 || test_trg >= n) test_trg = 0;
    for(int u = 0; u < 20; u++) {
        test_trg++;
        if(test_trg >= n) test_trg = 0;
        if(test_trg != i && a[test_trg].in_use && a[test_trg].team != team && a[test_trg].self_destoy < 0) break;
    }
    
    // Get distance to current nearest
    if(nearest_trg > -1 && nearest_trg < n && nearest_trg != i) {
        if(a[nearest_trg].in_use && a[nearest_trg].team != team && a[nearest_trg].self_destoy < 0) {
            v1 = a[i].pos - a[nearest_trg].pos;
            nearest_dis = v1.x * v1.x + v1.y * v1.y;
        } else nearest_dis = -1;
    } else nearest_dis = -1;
    
    // Get distance to test
    float f1 = -1;
    if(test_trg != i && a[test_trg].in_use && a[test_trg].team != team && a[test_trg].self_destoy < 0) {
        v1 = a[i].pos - a[test_trg].pos;
        f1 = v1.x * v1.x + v1.y * v1.y;
    }
    
    // Compare and select
    if(f1 > -1 && (f1 < nearest_dis || nearest_dis < 0)) {
        nearest_trg = test_trg;
        nearest_dis = f1;
    }
}

// Follow target unit
void RR_unit::follow_target(RR_unit* a, int n, int i) {
    burn_eng = 0;
    fire = 0;
    trn = 0;
    if(self_destoy > 0) return;
    
    // Avoid doing the same thing all the time
    if(timeout < 0.0) {
        timeout = rand() % 30;
        mode = rand() % 2;
        if(rand() % 100 < 20) trg = rand() % n;
        else if(rand() % 100 < 40) find_better_target(a, n, i);
    }
    
    // Check target validity
    if(!has_valid_target(a, n, i)) trg = rand() % n;
    
    // Follow target
    else follow(a[trg].pos, false);
}

// Follow target coords
void RR_unit::follow(RR_vec2 target, bool fixmode) {
    
    // Take own speed and distance into account
    double distance = nrm.distance(pos, target);
    double distance2 = distance;
    RR_vec2 target_fix = target;
    
    // Switch mode depending on situation unless fixed mode
    if(!fixmode) {
        
        // Always avoid target if no guns
        if(!guns) mode = 1;
        
        // Start following target if far away
        else if(distance > 1000.0) mode = 0;
        
        // Avoid hitting target if too close
        else if(distance < 300.0) mode = 1;
    }
    
    // Calculate interception course if attack mode
    if(mode == 0) {
        if(distance > 600) distance = 600;
        distance = distance / 600.0 + 0.25;
        target_fix = target - spd * distance;
    }
    
    // Get relative direction to target
    RR_vec2 t_nrm = nrm.normal(pos, target_fix);
    double t_dot = t_nrm.dot(t_nrm, nrm); // How much in front of unit
    double e_dot = t_nrm.dot(t_nrm, nrm.extrude()); // How much beside unit
    
    // Attack mode
    if(mode == 0) {
        
        // Rotate in direction of target
        if(e_dot < 0) trn = -1.0;
        else if(e_dot > 0) trn = 1.0;
        
        // Smoother rotation
        if(t_dot > 0.8) trn = trn * ((1.0 - t_dot) / 0.2);
        
        // Burners on if target in front of ship
        if(t_dot > 0.5) burn_eng = 1;
        else burn_eng = 0;
        
        // Fire if target is close enough
        if(t_dot > 0.9 && distance2 < 600.0) fire = true;
        else fire = false;
    
    // Escape mode
    } else if(mode == 1) {
        
        // Rotate away from of target
        if(e_dot < 0) trn = 1.0;
        else if(e_dot > 0) trn = -1.0;
        
        // Smoother rotation
        if(t_dot < -0.8) trn = trn * ((1.0 + t_dot) / 0.2);
        
        // Burners on if target in front of ship
        if(t_dot < 0.0) burn_eng = 1;
        else burn_eng = 0;
        
        // No firing
        fire = false;
    }
}

// Move unit
void RR_unit::move(float fspd) {
    timeout -= fspd;
    
    // Smooth turning
    if(self_destoy < 0) {
        if(trn > 0.01) {
            if(trn2 < trn) trn2 += (trn2 < 0.0 ? 6.0 : 2.0) * fspd;
            if(trn2 > trn) trn2 = trn;
        } else if(trn < -0.01) {
            if(trn2 > trn) trn2 -= (trn2 > 0.0 ? 6.0 : 2.0) * fspd;
            if(trn2 < trn) trn2 = trn;
        } else {
            if(trn2 < trn) {
                trn2 += 6.0 * fspd;
                if(trn2 > trn) trn2 = trn;
            } else if(trn2 > trn) {
                trn2 -= 6.0 * fspd;
                if(trn2 < trn) trn2 = trn;
            }
        }
    }
    
    // Turn ship
    nrm = nrm.rotate(nrm, RR_vec2(trn2 * (thrust / weight) * M_PI * 2.0 * fspd));
    
    // Acceleration (thrust and weight taken into account)
    //  * High acceleration value lowered a lot by friction
    if(burn_eng && self_destoy < 0) spd = spd + nrm * (thrust / weight) * 600.0 * fspd;
    
    // Friction in space \o/
    if(self_destoy < 0) spd = spd - spd * 0.8 * fspd;
    
    // Move the ship
    pos = pos + spd * fspd;
}

// Draw the unit on screen
void RR_unit::draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale, RR_vec2 sun_dir, bool showdmg) {
    
    // Loop through all parts
    for(int i = RR_MAX_UNIT_PARTS - 1; i >= 0; i--) if(p[i].in_use) {
        
        // Draw any part that exists
        p[i].draw(win, position + (normal * p[i].pos.x + normal.extrude() * p[i].pos.y) * scale, normal, scale, p[i].type, burn_eng, sun_dir, showdmg);
    }
    if(RR_g.debugmode == 3) ellipseRGBA(win, position.x, position.y, size * scale, size * scale, 255, 0, 0, 200); // Show ship size
}
void RR_unit::draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale, RR_vec2 sun_dir) {
    draw(win, position, normal, scale, sun_dir, false);
}
void RR_unit::draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
    draw(win, position, normal, scale, RR_vec2(0, -1), false);
}

// Check if another unit is too close and bounce on it
float RR_unit::bounce(RR_unit &other, bool keepoff, bool hulldamage) {
    RR_vec2 p1, p2, n, v1;
    double s;
    float f1, inter_dis;
    
    // Sanity check
    if(!other.in_use) return 0;
    
    // Quick box-fit test first
    if(RR_g_vec2.box_distance(pos, other.pos) < size + other.size) {
        
        // Check if any part is actually touching
        for(int i = 0; i < RR_MAX_UNIT_PARTS; i++) if(p[i].in_use) {
            p1 = pos + nrm * p[i].pos.x + nrm.extrude() * p[i].pos.y; // Real position of part
            for(int u = 0; u < RR_MAX_UNIT_PARTS; u++) if(other.p[u].in_use) {
                p2 = other.pos + other.nrm * other.p[u].pos.x + other.nrm.extrude() * other.p[u].pos.y; // Real pos
                n = RR_g_vec2.normal(p1, p2); // Bounce direction
                
                // Part touches
                if(p[i].intersect(p[i].type, p2, nrm, other.p[u].type, p2, other.nrm, v1, inter_dis, tmp_vec2)) {
//                 if(RR_g_vec2.dot(n, p2 - p1) < p[i].size() + other.p[u].size()) {
                    
                    // Get relative speed
                    s = RR_g_vec2.dot(n, spd) + RR_g_vec2.dot(n, RR_vec2() - other.spd);
                    f1 = (weight + other.weight) / 100.0;
                    if(s > 0.0) {
                        spd = spd - n * s * (2.0 / (weight + other.weight) * other.weight);
                        other.spd = other.spd + n * s * (2.0 / (weight + other.weight) * weight);
                        
                        // Exact spot that touches
//                         tmp_vec2 = (p1 + n * p[i].size() + p2 - n * other.p[u].size()) / 2.0;
                        
                        // Move ships away from each other
                        if(keepoff) {
//                             pos = pos - n * ((p[i].size() + other.p[u].size()) / 2.0);
//                             other.pos = other.pos + n * ((p[i].size() + other.p[u].size()) / 2.0);
                            pos = pos - n * (inter_dis / 2.0);
                            other.pos = other.pos + n * (inter_dis / 2.0);
                        }
                        
                        // Damage and optionally destroy part
                        if(hulldamage) {
                            p[i].health -= ((rand() % 10000) / 10000.0) * s * RR_BOUNCE_DAMAGE * f1;
                            if(p[i].health <= 0.0) {
                                p[i].in_use = false;
                                recalculate();
                            }
                            other.p[u].health -= ((rand() % 10000) / 10000.0) * s * RR_BOUNCE_DAMAGE * f1;
                            if(other.p[u].health <= 0.0) {
                                other.p[u].in_use = false;
                                other.recalculate();
                            }
                        }
                    }
                    return s * f1;
                }
            }
        }
    }
    
    // No hit
    return 0;
}

// Control ship by player input
bool RR_unit::player_input(Uint8* keys) {
    
    // Burn engines on W key
    if(keys[SDLK_i] || keys[SDLK_w] || keys[SDLK_UP]) burn_eng = 1;
    else burn_eng = 0;
    
    // Turn ship on A and D
    trn = 0;
    if(keys[SDLK_j] || keys[SDLK_a] || keys[SDLK_LEFT]) trn -= 1.0;
    if(keys[SDLK_l] || keys[SDLK_d] || keys[SDLK_RIGHT]) trn += 1.0;
    
    // Fire blasters
    if(keys[SDLK_q] || keys[SDLK_SPACE] || keys[SDLK_RSHIFT]) fire = true;
    else fire = false;
    
    // Return action or inaction
    if(burn_eng || trn || fire) {
        if(self_destoy > 0) burn_eng = 0;
        return true;
    }
    if(self_destoy > 0) burn_eng = 0;
    return false;
}

// Draw a target pointer
void RR_unit::target_pointer(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float size, float scale) {
    RR_vec2 vec[4];
    vec[0] = RR_vec2(size + 20.0, 0);
    vec[1] = RR_vec2(size + 25.0, -4.0);
    vec[2] = RR_vec2(size + 40.0, 0);
    vec[3] = RR_vec2(size + 25.0, 4.0);
    RR_g_vec2.draw_polygon(win, vec, 4, position, normal, scale, 255, 50, 0);
    vec[0] = RR_vec2(size + 22.0, 0);
    vec[1] = RR_vec2(size + 25.0, -2.0);
    vec[2] = RR_vec2(size + 30.0, 0);
    vec[3] = RR_vec2(size + 25.0, 2.0);
    RR_g_vec2.draw_polygon(win, vec, 4, position, normal, scale, 0, 0, 0);
    
    // Also nose indicator
    vec[0] = RR_vec2(size + 60.0, -4.0);
    vec[1] = RR_vec2(size + 60.0, -7.0);
    vec[2] = RR_vec2(size + 80.0, -4.0);
    RR_g_vec2.draw_polygon(win, vec, 3, position, nrm, scale, 200, 150, 0);
    vec[0] = RR_vec2(size + 60.0, 4.0);
    vec[1] = RR_vec2(size + 60.0, 7.0);
    vec[2] = RR_vec2(size + 80.0, 4.0);
    RR_g_vec2.draw_polygon(win, vec, 3, position, nrm, scale, 200, 150, 0);
}

// Draw a target indicator
void RR_unit::target_indicator(SDL_Surface* win, RR_vec2 position, float size, float scale) {
    RR_vec2 vec[4];
    vec[0] = RR_vec2(size + 20.0, size + 20.0);
    vec[1] = RR_vec2(size, size + 20.0);
    vec[2] = RR_vec2(size + 25.0, size + 25.0);
    vec[3] = RR_vec2(size + 20.0, size);
    RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(1, 0), scale, 255, 50, 0);
    RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(-1, 0), scale, 255, 50, 0);
    RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(0, -1), scale, 255, 50, 0);
    RR_g_vec2.draw_polygon(win, vec, 4, position, RR_vec2(0, 1), scale, 255, 50, 0);
}
    
// Check if ship will self destruct
bool RR_unit::suicide_timer(float fspd) {
    if(self_destoy > 0) self_destoy += fspd;
    if(self_destoy > 5.0) return true;
    return false;
}

#endif // RR_UNIT_H
