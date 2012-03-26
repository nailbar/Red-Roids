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
    
    // Draw the particle on screen
    void draw(SDL_Surface* win, RR_vec2 position, RR_vec2 normal, float scale) {
        RR_vec2 vec[5];
        switch(type) {
        case 0: // Spark (max life 3 sec)
            vec[0] = RR_vec2(-2, -6);
            vec[1] = RR_vec2(2, -6);
            vec[2] = RR_vec2(0, 6);
            if(life <= 3.0) RR_g_vec2.draw_polygon(win, vec, 3, position, normal, scale / (4.0 - life), 255, 255 / (4.0 - life), 0);
            break;
        }
    }
    
    // Move particle
    void move(float fspd) {
        switch(type) {
        case 0: // Spark (max life 3 sec)
            life -= fspd;
            pos = pos + spd * fspd;
            if(life < 0.0) {
                in_use = 0;
            } else if(life <= 3.0) nrm = nrm.rotate(nrm, RR_vec2(M_PI * (12.0 / (4.0 - life)) * fspd));
            break;
        }
    }
};

#endif // RR_PARTICLE_H
