#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include <cmath>

// Practical globals
struct RR_practical_globals {
    
    // Debug modes:
    //  * 0 = Off
    //  * 1 = Wireframe + centers
    //  * 2 = Tilt normals
    char debugmode;
    
    // Constructor
    RR_practical_globals() {
        debugmode = 0; // Off
    }
} RR_g;

// Include classes
// #include "roidmap.h"
#include "unit.h"
#include "menu.h"
#include "battle.h"

int main(int argc, char* args[]) {
    
    // Get it going
    SDL_Init(SDL_INIT_VIDEO);
    
    // This is the windows
    SDL_Surface* win = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    
    // Set the window title
    SDL_WM_SetCaption("Red Roids", NULL);
    
    // No cursor
    SDL_ShowCursor(SDL_DISABLE);
    
    // Variables for game loop
    SDL_Event event;
    bool inloop = true;
    Uint8* keys; // Keyboard input
    keys = SDL_GetKeyState(NULL);
    int mpos[2] = {400, 300}; // Mouse coordinates
    Uint8 mkeys; // Mouse keys
    char gamemode = 2;//1; // 1 = menu, 2 = battle
    
    // Framerate
    Uint32 time1 = SDL_GetTicks();
    Uint32 time2 = 0;
    float fspd;
    fspd = 1.0;
    srand(time1);
    
    // Game models
    RR_menu menu;
    RR_battle battle;
    
    //Game loop
    while(inloop){
        while(SDL_PollEvent(&event)){
            switch(event.type){
            case SDL_QUIT: inloop=false; break;
            
            //Mouse moved
            case SDL_MOUSEMOTION:
                mpos[0]=event.motion.x;
                mpos[1]=event.motion.y;
                break;
            
            // Check for ESC key
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    if(gamemode == 1) inloop = false;
                    else gamemode = 1;
                } else if(event.key.keysym.sym == SDLK_F1) {
                    RR_g.debugmode++;
                    if(RR_g.debugmode > 2) RR_g.debugmode = 0;
                }
                break;
            }
        }
        
        //Get framerate
        time1=SDL_GetTicks();
        if(time2) {
            fspd = float(time1 - time2) / 1000.0;
            
            //Limit to max framerate
            if(1.0 / fspd > 60) SDL_Delay(int((1.0 / 60 - fspd) * 1000.0));
            
            // Limit to min framerate
            if(fspd > 0.1) fspd = 0.1;
            
            // Slow motion in debug mode
            if(RR_g.debugmode) fspd = fspd * 0.1;
        }
        time2=time1;
        
        //Mouse keys
        mkeys = SDL_GetMouseState(NULL, NULL);
        
        //Clear screen
        boxRGBA(win, -5, -5, 805, 605, 0, 0, 0, 255);
        
        // Select game mode
        switch(gamemode) {
            
        // Menu
        case 1:
            
            // Tell menu about clicks
            menu.set_clicks(mkeys & SDL_BUTTON(1), mpos);
            
            // Menu background effects
            menu.handle_background(win, mpos, fspd);
            
            // Draw menu items
            switch(menu.handle_menu(win, mpos)) {
            case 1: gamemode = 2; break; // Instant battle
            case 4: inloop = false; break; // Exit button
            }
            
            // Draw cursor
            menu.handle_cursor(win, mpos);
            break;
        
        // Battle
        case 2:
            battle.main(win, fspd, keys);
            break;
        default: // Unknown (error?)
            inloop = false;
        }
        
        //Swap double buffer
        SDL_Flip(win);
    }
    
    //Clean up
    SDL_Quit();
    
    //Done
    return 0;
}
