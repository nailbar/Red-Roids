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
    RR_vec2 oldcam;
    RR_vec2 starcam;
    
    // Constructor
    RR_starfield() {
        for(int i = 0; i < RR_STARFIELD_STARS; i++) {
            s[i] = RR_g_vec2.box_random();
            d[i] = (rand() % 10000) / 10000.0;
        }
        oldcam = RR_vec2(0, 0);
        starcam = RR_vec2(0, 0);
    }
    
    // Draw starfield
    void draw(SDL_Surface* win, RR_vec2 cam, float zoom) {
        starcam = starcam + (cam - oldcam) * zoom;
        oldcam = cam;
        RR_vec2 v1;
        RR_vec2 vec[8];
        vec[0] = RR_vec2(-2, -2);
        vec[1] = RR_vec2(0, -1);
        vec[2] = RR_vec2(2, -2);
        vec[3] = RR_vec2(1, 0);
        vec[4] = RR_vec2(2, 2);
        vec[5] = RR_vec2(0, 1);
        vec[6] = RR_vec2(-2, 2);
        vec[7] = RR_vec2(-1, 0);
        int r = 150 * (zoom < 1.0 ? zoom : 1.0);
        int g = 200 * (zoom < 1.0 ? zoom : 1.0);
        int b = 255 * (zoom < 1.0 ? zoom : 1.0);
        for(int i = 0; i < RR_STARFIELD_STARS; i++) {
            v1 = RR_vec2(
                s[i].x * RR_g.wid - starcam.x / (2.0 + d[i] * 2.0),
                s[i].y * RR_g.hgt - starcam.y / (2.0 + d[i] * 2.0)
            );
            while(v1.x < 0) v1.x += RR_g.wid * 20.0;
            while(v1.y < 0) v1.y += RR_g.hgt * 20.0;
            v1 = RR_vec2(
                int(v1.x) % RR_g.wid,
                int(v1.y) % RR_g.hgt
            );
            RR_g_vec2.draw_polygon(
                win,
                vec,
                8,
                v1,
                RR_vec2(i * 0.1),
                (RR_g.wid / 1000.0) / (1.0 + d[i]),
                int(r / (1.0 + d[i] * 10.0)),
                int(g / (1.0 + d[i] * 10.0)),
                int(b / (1.0 + d[i] * 10.0))
            );
        }
    }
};

#endif // RR_STARFIELD_H
