#ifndef RR_VECTOR_H
#define RR_VECTOR_H 1

class RR_vector {
public:
    double x, y;
    
    // Constructors
    RR_vector() {
        x = 0;
        y = 0;
    }
    RR_vector(double newx, double newy) {
        x = newx;
        y = newy;
    }
    RR_vector(double radians) {
        x = cos(radians);
        y = sin(radians);
    }
    RR_vector operator+(RR_vector v) {
        return RR_vector(x + v.x, y + v.y);
    }
    RR_vector operator-(RR_vector v) {
        return RR_vector(x - v.x, y - v.y);
    }
    RR_vector operator*(double n) {
        return RR_vector(x * n, y * n);
    }
    RR_vector operator/(double n) {
        return RR_vector(x / n, y / n);
    }
    
    // Return the light value
    //  * Assume this and light_dir are both normalized
    double light_value(RR_vector light_dir) {
        double val = dot(light_dir) * 0.5 + 0.5;
        return val;
    }
    
    // Calculate the dot value for two vectors
    double dot(RR_vector vec2) {
        return x * vec2.x + y * vec2.y;
    }
    double dot(RR_vector vec1, RR_vector vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }
    
    // Return a vector rotated by another vector
    RR_vector rotate(RR_vector vec1, RR_vector vec2) {
        return RR_vector(
            vec2.x * vec1.x + vec2.y * vec1.y,
            vec2.y * vec1.x - vec2.x * vec1.y
        );
    }
    
    // Draw a translated, rotated, scaled and tilted polygon
    void draw_polygon(
        SDL_Surface* win, RR_vector* vec, unsigned char n,
        RR_vector translate, RR_vector rotate,
        RR_vector tilt_dir, RR_vector light_dir,
        double scale, float r, float g, float b,
        float ambient, float phong)
    {
        short x[n];
        short y[n];
        RR_vector v;
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

#endif // RR_VECTOR_H
