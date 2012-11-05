#ifndef RR_TEXT_H
#define RR_TEXT_H 1

#ifndef RR_TEXT_MAX_LENGTH
#define RR_TEXT_MAX_LENGTH 5000
#endif

#include "vector.h"

class RR_text {
public:
    void draw(SDL_Surface* win, RR_vec2 pos, int scale, float r, float g, float b, const char* str) {
        int flags;
        RR_vec2 v1 = pos;
        for(int i = 0; i < RR_TEXT_MAX_LENGTH; i++) if(!str[i]) return;
        else {
            flags = 0;
            switch(str[i]) {
            case '\n': v1 = RR_vec2(pos.x - scale * 4, v1.y + scale * 6); break;
            case 'a': case 'A': flags = 1+2+4+8+32+64+128+256+512+2048+4096+16384; break;
            case 'b': case 'B': flags = 1+2+4+8+32+64+128+512+2048+4096+8192+16384; break;
            case 'c': case 'C': flags = 1+2+4+8+64+512+4096+8192+16384; break;
            case 'd': case 'D': flags = 1+2+8+32+64+256+512+2048+4096+8192; break;
            case 'e': case 'E': flags = 1+2+4+8+64+128+256+512+4096+8192+16384; break;
            case 'f': case 'F': flags = 1+2+4+8+64+128+256+512+4096; break;
            case 'g': case 'G': flags = 1+2+4+8+64+256+512+2048+4096+8192+16384; break;
            case 'h': case 'H': flags = 1+4+8+32+64+128+256+512+2048+4096+16384; break;
            case 'i': case 'I': flags = 2+16+128+1024+8192; break;
            case 'j': case 'J': flags = 4+32+256+512+2048+4096+8192+16384; break;
            case 'k': case 'K': flags = 1+4+8+32+64+128+512+2048+4096+16384; break;
            case 'l': case 'L': flags = 1+8+64+512+4096+8192+16384; break;
            case 'm': case 'M': flags = 1+4+8+16+32+64+128+256+512+2048+4096+16384; break;
            case 'n': case 'N': flags = 1+4+8+16+32+64+128+256+512+1024+2048+4096+16384; break;
            case 'o': case 'O': flags = 1+2+4+8+32+64+256+512+2048+4096+8192+16384; break;
            case 'p': case 'P': flags = 1+2+4+8+32+64+128+256+512+4096; break;
            case 'q': case 'Q': flags = 1+2+4+8+32+64+256+512+2048+1024+16384; break;
            case 'r': case 'R': flags = 1+2+4+8+32+64+128+512+2048+4096+16384; break;
            case 's': case 'S': flags = 2+4+8+64+128+256+2048+4096+8192; break;
            case 't': case 'T': flags = 1+2+4+16+128+1024+8192; break;
            case 'u': case 'U': flags = 1+4+8+32+64+256+512+2048+4096+8192+16384; break;
            case 'v': case 'V': flags = 1+4+8+32+64+256+512+2048+8192; break;
            case 'w': case 'W': flags = 1+4+8+32+64+128+256+512+1024+2048+4096+16384; break;
            case 'x': case 'X': flags = 1+4+8+32+128+512+2048+4096+16384; break;
            case 'y': case 'Y': flags = 1+4+8+32+128+1024+8192; break;
            case 'z': case 'Z': flags = 1+2+4+32+128+512+4096+8192+16384; break;
            case '1': flags = 4+16+32+256+2048+16384; break;
            case '2': flags = 1+2+32+128+512+4096+8192+16384; break;
            case '3': flags = 1+2+32+64+128+2048+4096+8192; break;
            case '4': flags = 1+4+8+32+64+128+256+2048+16384; break;
            case '5': flags = 1+2+4+8+64+128+256+2048+4096+8192; break;
            case '6': flags = 2+4+8+64+128+256+512+2048+4096+8192+16384; break;
            case '7': flags = 1+2+4+32+256+2048+16384; break;
            case '8': flags = 1+2+4+8+32+64+128+256+512+2048+4096+8192+16384; break;
            case '9': flags = 1+2+4+8+32+64+128+256+2048+4096+8192; break;
            case '0': flags = 1+2+4+8+32+64+256+512+2048+4096+8192+16384; break;
            case '.': flags = 4096; break;
            case ':': flags = 8+512; break;
            case '-': flags = 64+128+256; break;
            case '+': flags = 16+64+128+256+1024; break;
            case '_': flags = 4096+8192+16384; break;
            case '!': flags = 2+16+128+8192; break;
            case '?': flags = 1+2+4+32+128+8192; break;
            case '*': flags = 8+32+128+512+2048; break;
            case ' ': flags = 0; break;
            default: flags = 1+2+4+8+16+32+64+128+256+512+1024+2048+4096+8192+16384;
            }
            if(flags) draw_box(win, v1, scale, r, g, b, flags);
            v1 = v1 + RR_vec2(4, 0) * scale;
        }
    }
    void draw_box(SDL_Surface* win, RR_vec2 pos, int scale, float r, float g, float b, int flags) {
        int spacing = int(scale / 7) + 1;
        
        if(flags & 1) boxRGBA(win,     pos.x,             pos.y,             pos.x + scale - spacing,     pos.y + scale - spacing,     r, g, b, 255);
        if(flags & 2) boxRGBA(win,     pos.x + scale,     pos.y,             pos.x + scale * 2 - spacing, pos.y + scale - spacing,     r, g, b, 255);
        if(flags & 4) boxRGBA(win,     pos.x + scale * 2, pos.y,             pos.x + scale * 3 - spacing, pos.y + scale - spacing,     r, g, b, 255);
        
        if(flags & 8) boxRGBA(win,     pos.x,             pos.y + scale,     pos.x + scale - spacing,     pos.y + scale * 2 - spacing, r, g, b, 255);
        if(flags & 16) boxRGBA(win,    pos.x + scale,     pos.y + scale,     pos.x + scale * 2 - spacing, pos.y + scale * 2 - spacing, r, g, b, 255);
        if(flags & 32) boxRGBA(win,    pos.x + scale * 2, pos.y + scale,     pos.x + scale * 3 - spacing, pos.y + scale * 2 - spacing, r, g, b, 255);
        
        if(flags & 64) boxRGBA(win,    pos.x,             pos.y + scale * 2, pos.x + scale - spacing,     pos.y + scale * 3 - spacing, r, g, b, 255);
        if(flags & 128) boxRGBA(win,   pos.x + scale,     pos.y + scale * 2, pos.x + scale * 2 - spacing, pos.y + scale * 3 - spacing, r, g, b, 255);
        if(flags & 256) boxRGBA(win,   pos.x + scale * 2, pos.y + scale * 2, pos.x + scale * 3 - spacing, pos.y + scale * 3 - spacing, r, g, b, 255);
        
        if(flags & 512) boxRGBA(win,   pos.x,             pos.y + scale * 3, pos.x + scale - spacing,     pos.y + scale * 4 - spacing, r, g, b, 255);
        if(flags & 1024) boxRGBA(win,  pos.x + scale,     pos.y + scale * 3, pos.x + scale * 2 - spacing, pos.y + scale * 4 - spacing, r, g, b, 255);
        if(flags & 2048) boxRGBA(win,  pos.x + scale * 2, pos.y + scale * 3, pos.x + scale * 3 - spacing, pos.y + scale * 4 - spacing, r, g, b, 255);
        
        if(flags & 4096) boxRGBA(win,  pos.x,             pos.y + scale * 4, pos.x + scale - spacing,     pos.y + scale * 5 - spacing, r, g, b, 255);
        if(flags & 8192) boxRGBA(win,  pos.x + scale,     pos.y + scale * 4, pos.x + scale * 2 - spacing, pos.y + scale * 5 - spacing, r, g, b, 255);
        if(flags & 16384) boxRGBA(win, pos.x + scale * 2, pos.y + scale * 4, pos.x + scale * 3 - spacing, pos.y + scale * 5 - spacing, r, g, b, 255);
    }
} RR_g_text;

#endif // RR_TEXT_H
