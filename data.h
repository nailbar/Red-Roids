#ifndef RR_DATA_H
#define RR_DATA_H 1

#ifndef RR_DATA_MAX_PARTS
#define RR_DATA_MAX_PARTS 50
#endif

#ifndef RR_DATA_MAX_POLYGONS
#define RR_DATA_MAX_POLYGONS 5
#endif

#ifndef RR_DATA_MAX_POLYGON_SIZE
#define RR_DATA_MAX_POLYGON_SIZE 5
#endif

#include <iostream>
#include <fstream>
using namespace std;

#include "vector.h"

// Theory:
//  * Constructor loads all files in "data" directory
//    and saves it in data nodes which can then be read
//    on demand... Or something.

struct RR_data_part_polyon {
    RR_vec2 vectors[RR_DATA_MAX_POLYGON_SIZE];
    unsigned char polysize;
    unsigned char color_r;
    unsigned char color_g;
    unsigned char color_b;
    float tilt_dir;
    float phong;
    float ambient;
};
struct RR_data_part {
    RR_data_part_polyon p[RR_DATA_MAX_POLYGONS];
    char name[50];
    char type;
    float size;
    float power;
    char weapon;
    float reload;
    float thrust;
    float weight;
    float strength;
    float power_draw;
    RR_vec2 fire_pos;
    RR_vec2 fire_size;
};

class RR_data {
public:
    RR_data_part d[RR_DATA_MAX_PARTS];
    RR_data() {
        d[0].p[0].vectors[0] = RR_vec2(-4, -5);
        d[0].p[0].vectors[1] = RR_vec2(4, -7);
        d[0].p[0].vectors[2] = RR_vec2(4, 7);
        d[0].p[0].vectors[3] = RR_vec2(-4, 5);
        d[0].p[0].polysize = 4;
        d[0].p[0].color_r = 100;
        d[0].p[0].color_g = 110;
        d[0].p[0].color_b = 130;
        d[0].p[0].tilt_dir = 1.0;
        d[0].p[0].phong = 0.2;
        d[0].p[0].ambient = 0.3;
        d[0].p[1].polysize = 0;
        d[0].p[2].polysize = 0;
        d[0].p[3].polysize = 0;
        d[0].p[4].polysize = 0;
        d[0].type = 1; // Thruster
        d[0].size = 7;
        d[0].power = 0;
        d[0].weapon = 0;
        d[0].reload = 0;
        d[0].thrust = 22.0;
        d[0].weight = 5.0;
        d[0].strength = 5.0;
        d[0].power_draw = 20.0;;
        d[0].fire_pos = RR_vec2(-4, 0);
        d[0].fire_size = RR_vec2(-15, 5);
        
        
//         cout<<"Loading data...\n";
//         string line;
//         char str[50];
//         ifstream file;
//         for(int i = 0; i < 1000; i++) {
//             sprintf(str, "data/part_%03d.txt", i);
//             file.open(str);
//             if(file.is_open()) {
//                 cout<<"Reading file "<<str<<endl;
//                 while(file.good()) {
//                     getline(file, line);
//                     cout<<line<<endl;
//                 }
//                 file.close();
//             } else {
//                 cout<<"File "<<str<<" not found. Breaking loop.\n";
//                 break;
//             }
//     // Test
//     string row;
//     ifstream file("test.txt");
//     if(file.is_open()) {
//         while(file.good()) {
//             getline(file, row);
//             cout<<row<<endl;
//         }
//         file.close();
//     } else cout<<"Could not open file bwaaah!\n";
//         }
    }
    ~RR_data() {
        cout<<"Unloading data...\n";
    }
} RR_g_data;

#endif // RR_DATA_H
