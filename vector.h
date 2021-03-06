#ifndef RR_VEC2_H
#define RR_VEC2_H 1

#include <cmath>

class RR_vec2 {
public:
    double x, y;
    
    // Constructors
    RR_vec2() {
        x = 0;
        y = 0;
    }
    RR_vec2(double newx, double newy) {
        x = newx;
        y = newy;
    }
    RR_vec2(double radians) {
        x = cos(radians);
        y = sin(radians);
    }
    
    // Overloaded operators
    RR_vec2 operator+(RR_vec2 v) {
        return RR_vec2(x + v.x, y + v.y);
    }
    RR_vec2 operator-(RR_vec2 v) {
        return RR_vec2(x - v.x, y - v.y);
    }
    RR_vec2 operator*(double n) {
        return RR_vec2(x * n, y * n);
    }
    RR_vec2 operator/(double n) {
        return RR_vec2(x / n, y / n);
    }
    
    // Return a random vector (box -1.0 to 1.0)
    RR_vec2 box_random() {
        return RR_vec2(
            (rand() % 8000) / 4000.0 - 1.0,
            (rand() % 8000) / 4000.0 - 1.0
        );
    }
    
    // Return a random vector (radial 1.0)
    RR_vec2 rad_random() {
        return RR_vec2((rand() % 8000) / 4000.0 * M_PI * 2.0) * ((rand() % 8000) / 4000.0);
    }
    
    // Return a random normal
    RR_vec2 random() {
        return RR_vec2((rand() % 8000) / 4000.0 * M_PI * 2.0);
    }
    
    // Return the light value
    //  * Assume this and light_dir are both normalized
    double light_value(RR_vec2 light_dir) {
        double val = dot(light_dir) * 0.5 + 0.5;
        return val;
    }
    
    // Calculate the dot value for two vectors
    double dot(RR_vec2 vec2) {
        return x * vec2.x + y * vec2.y;
    }
    double dot(RR_vec2 vec1, RR_vec2 vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }
    
    // Return a vector rotated by another vector
    RR_vec2 rotate(RR_vec2 nrm, RR_vec2 rot) {
        return RR_vec2(
            nrm.x * rot.x - nrm.y * rot.y,
            nrm.y * rot.x + nrm.x * rot.y
        );
    }
    
    // Return the extruded vector
    RR_vec2 extrude() {
        return RR_vec2(-y, x);
    }
    
    // Return the normal from one vector to another
    RR_vec2 normal(RR_vec2 from, RR_vec2 to) {
        RR_vec2 v1 = to - from;
        double d1 = sqrt(v1.x * v1.x + v1.y * v1.y);
        if(d1 > 0.0) return v1 / d1;
        return RR_vec2(1.0, 0.0);
    }
    
    // Return the distance between two vectors using box-fit (quickest)
    double box_distance(RR_vec2 from, RR_vec2 to) {
        RR_vec2 v1 = to - from;
        if(v1.x < 0.0) v1.x = 0.0 - v1.x;
        if(v1.y < 0.0) v1.y = 0.0 - v1.y;
        if(v1.x > v1.y) return v1.x;
        return v1.y;
    }
    
    // Return the distance between two vectors
    double distance(RR_vec2 from, RR_vec2 to) {
        RR_vec2 v1 = to - from;
        return sqrt(v1.x * v1.x + v1.y * v1.y);
    }
    
    // Return if two lines intersect and calculate the intersection point
    bool intersect(RR_vec2 l1a, RR_vec2 l1b, RR_vec2 l2a, RR_vec2 l2b, RR_vec2 &v) {
        
        // Make all coordinates relative to l1a
        RR_vec2 v_l1a = RR_vec2(0, 0);
        RR_vec2 v_l1b = l1b - l1a;
        RR_vec2 v_l2a = l2a - l1a;
        RR_vec2 v_l2b = l2b - l1a;
        
        // Get normal and extruded normal for l1
        RR_vec2 l1_nrm = normal(v_l1a, v_l1b);
        RR_vec2 l1_ext = l1_nrm.extrude();
        
        // Get dot values for l2
        double l2a_dot = dot(l1_ext, v_l2a);
        double l2b_dot = dot(l1_ext, v_l2b);
        
        // Check if l2a and l2b is on different sides of l1
        if(l2a_dot < 0 && l2b_dot < 0) return false;
        if(l2a_dot > 0 && l2b_dot > 0) return false;
        
        // Get distance and normal between l2 ends
        double l2_dis = distance(l2a, l2b);
        RR_vec2 l2_nrm = normal(l2a, l2b);
        
        // Calculate the intersection point
        v = l2a + l2_nrm * (l2_dis * ((0 - l2a_dot) / (l2b_dot - l2a_dot)));
        
        // Check if intersection point is outside of l1
        RR_vec2 v_v = v - l1a;
        double v_dot = dot(l1_nrm, v_v);
        double l1_dis = distance(l1a, l1b);
        if(v_dot < 0) return false;
        if(v_dot > l1_dis) return false;
        
        // All checkes passed, we have an intersection
        return true;
    }
    
    // Draw a translated polygon
    void draw_polygon(
        SDL_Surface* win, RR_vec2* vec, unsigned char n,
        RR_vec2 translate, float r, float g, float b
    ) {
        short x[n];
        short y[n];
        
        // Process the vectors
        for(int i = 0; i < n; i++) {
            x[i] = vec[i].x + translate.x;
            y[i] = vec[i].y + translate.y;
        }
        
        // Draw the polygon
        if(RR_g.debugmode == 1) polygonRGBA(win, x, y, n, r, g, b, 55); // Wire frame
        else filledPolygonRGBA(win, x, y, n, r, g, b, 255);
    }
    
    // Draw a translated, rotated and scaled polygon
    void draw_polygon(
        SDL_Surface* win, RR_vec2* vec, unsigned char n,
        RR_vec2 translate, RR_vec2 rotate, double scale,
        float r, float g, float b
    ) {
        short x[n];
        short y[n];
        RR_vec2 v;
        
        // Process the vectors
        for(int i = 0; i < n; i++) {
            v = v.rotate(vec[i], rotate) * scale + translate;
            x[i] = v.x;
            y[i] = v.y;
        }
        
        // Draw the polygon
        if(RR_g.debugmode == 1) polygonRGBA(win, x, y, n, r, g, b, 55); // Wire frame
        else filledPolygonRGBA(win, x, y, n, r, g, b, 255);
    }
    
    // Draw a translated, rotated, scaled and tilted polygon
    void draw_polygon(
        SDL_Surface* win, RR_vec2* vec, unsigned char n,
        RR_vec2 translate, RR_vec2 rotate,
        RR_vec2 tilt_dir, RR_vec2 light_dir,
        double scale, float r, float g, float b,
        float ambient, float phong
    ) {
        short x[n];
        short y[n];
        RR_vec2 v;
        double light;
        
        // Process the vectors
        for(int i = 0; i < n; i++) {
            v = v.rotate(vec[i], rotate) * scale + translate;
            x[i] = v.x;
            y[i] = v.y;
        }
        
        // Calculate the light
        v = v.rotate(tilt_dir, rotate);
        light = v.light_value(light_dir) * (1.0 - ambient) + ambient; // 0.2 ambient value
        
        // Add phong effect
        if(light > 0.95) {
            r = r + (255.0 - r) * (light - 0.95) * 20.0 * phong;
            g = g + (255.0 - g) * (light - 0.95) * 20.0 * phong;
            b = b + (255.0 - b) * (light - 0.95) * 20.0 * phong;
        }
        
        // Debug tilt normals
        if(RR_g.debugmode == 2) lineRGBA(win, translate.x, translate.y, translate.x + v.x * 40, translate.y + v.y * 40, 0, 255, 0, 255);
        
        // Draw the polygon
        if(RR_g.debugmode == 1) polygonRGBA(win, x, y, n, r * light, g * light, b * light, 55); // Wire frame
        else filledPolygonRGBA(win, x, y, n, r * light, g * light, b * light, 255);
    }
} RR_g_vec2;

#endif // RR_VEC2_H
