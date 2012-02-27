#ifndef RR_VEC2_H
#define RR_VEC2_H 1

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
    RR_vec2 rotate(RR_vec2 vec1, RR_vec2 vec2) {
        return RR_vec2(
            vec2.x * vec1.x + vec2.y * vec1.y,
            vec2.y * vec1.x - vec2.x * vec1.y
        );
    }
    
    // Return the extruded vector
    RR_vec2 extrude() {
        return RR_vec2(y, -x);
    }
    
    // Draw a translated, rotated, scaled and tilted polygon
    void draw_polygon(
        SDL_Surface* win, RR_vec2* vec, unsigned char n,
        RR_vec2 translate, RR_vec2 rotate,
        RR_vec2 tilt_dir, RR_vec2 light_dir,
        double scale, float r, float g, float b,
        float ambient, float phong)
    {
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
//         lineRGBA(win, translate.x, translate.y, translate.x + v.x * 40, translate.y + v.y * 40, 0, 255, 0, 255); // Debug tilt_dir
        
        // Draw the polygon
        filledPolygonRGBA(win, x, y, n, r * light, g * light, b * light, 255);
//         polygonRGBA(win, x, y, n, 255, 0, 0, 55); // Debug polygon
    }
};

#endif // RR_VEC2_H
