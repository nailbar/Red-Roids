#ifndef RR_STARFIELD_H
#define RR_STARFIELD_H 1

#ifndef RR_STARFIELD_STARS
#define RR_STARFIELD_STARS 200
#endif

#include "vector.h"

class RR_starfield {
public:
    RR_vec2 s[RR_STARFIELD_STARS];
    float d[RR_STARFIELD_STARS];
    
    // Constructor
    RR_starfield() {
        for(int i = 0; i < RR_STARFIELD_STARS; i++) {
            s[i] = RR_g_vec2.box_random();
            d[i] = (rand() % 10000) / 10000.0;
        }
    }
    
    // Draw starfield
    void draw(SDL_Surface* win, RR_vec2 cam) {
        RR_vec2 v1;
        RR_vec2 vec[5];
        vec[0] = RR_vec2(-2, -2) * RR_g.wid / 2000.0;
        vec[1] = RR_vec2(2, -2) * RR_g.wid / 2000.0;
        vec[2] = RR_vec2(2, 2) * RR_g.wid / 2000.0;
        vec[3] = RR_vec2(-2, 2) * RR_g.wid / 2000.0;
        for(int i = 0; i < RR_STARFIELD_STARS; i++) {
            v1 = RR_vec2(
                s[i].x * RR_g.wid - cam.x / (3.0 + d[i]),
                s[i].y * RR_g.hgt - cam.y / (3.0 + d[i])
            );
            while(v1.x < 0) v1.x += RR_g.wid * 20.0;
            while(v1.y < 0) v1.y += RR_g.hgt * 20.0;
            v1 = RR_vec2(
                int(v1.x) % RR_g.wid,
                int(v1.y) % RR_g.hgt
            );
            RR_g_vec2.draw_polygon(win, vec, 4, v1, int(150 / (1.0 + d[i] * 20.0)), int(200 / (1.0 + d[i] * 20.0)), int(255 / (1.0 + d[i] * 20.0)));
        }
    }
};

#endif // RR_STARFIELD_H
