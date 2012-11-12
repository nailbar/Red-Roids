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

struct RR_data_part_polygon {
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
    RR_data_part_polygon p[RR_DATA_MAX_POLYGONS];
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
    int curpart;
    RR_data() {
        curpart = 0;
        for(int i = 0; i < RR_DATA_MAX_PARTS; i++) {
            d[i].type = 0;
        }
        string line;
        char str[50];
        ifstream file;
        char type_handle = 0;
        bool in_polygon = 0;
        int curpol = 0;
        int curvec = 0;
        int tmppos = 0;
        for(int i = 0; i < 1000; i++) {
            sprintf(str, "data/part_%03d.txt", i);
            file.open(str);
            if(file.is_open()) {
                cout<<"Reading file "<<str<<endl;
                while(file.good()) {
                    getline(file, line);
                    
                    // Look for beginning
                    if(type_handle == 0) {
                        if(line.find(string("begin part")) != string::npos) {
                            d[curpart].type = 0;
                            d[curpart].size = 1;
                            d[curpart].power = 0;
                            d[curpart].weapon = 0;
                            d[curpart].reload = 0;
                            d[curpart].thrust = 0;
                            d[curpart].weight = 0;
                            d[curpart].strength = 0;
                            d[curpart].power_draw = 0;
                            d[curpart].fire_pos = RR_vec2();
                            d[curpart].fire_size = RR_vec2();
                            for(int i = 0; i < RR_DATA_MAX_POLYGONS; i++) d[curpart].p[i].polysize = 0;
                            cout<<"begin part\n";
                            type_handle = 1;
                            curpol = 0;
                            in_polygon = 0;
                        }
                    } else if(type_handle == 1) {
                        if(line.find(string("end part")) != string::npos) {
                            cout<<"end part\n";
                            type_handle = 0;
                            curpart++;
                        } else if(in_polygon) {
                            if(line.find(string("end polygon")) != string::npos) {
                                cout<<"    end polygon\n";
                                in_polygon = 0;
                                curpol++;
                            } else if((tmppos = line.find(string("color_r:"))) != int(int(string::npos))) {
                                d[curpart].p[curpol].color_r = atoi(line.substr(tmppos + 8).c_str());
                                cout<<"        color_r: "<<int(d[curpart].p[curpol].color_r)<<endl;
                            } else if((tmppos = line.find(string("color_g:"))) != int(string::npos)) {
                                d[curpart].p[curpol].color_g = atoi(line.substr(tmppos + 8).c_str());
                                cout<<"        color_g: "<<int(d[curpart].p[curpol].color_g)<<endl;
                            } else if((tmppos = line.find(string("color_b:"))) != int(string::npos)) {
                                d[curpart].p[curpol].color_b = atoi(line.substr(tmppos + 8).c_str());
                                cout<<"        color_b: "<<int(d[curpart].p[curpol].color_b)<<endl;
                            } else if((tmppos = line.find(string("tilt_dir:"))) != int(string::npos)) {
                                d[curpart].p[curpol].tilt_dir = atof(line.substr(tmppos + 9).c_str());
                                cout<<"        tilt_dir: "<<d[curpart].p[curpol].tilt_dir<<endl;
                            } else if((tmppos = line.find(string("phong:"))) != int(string::npos)) {
                                d[curpart].p[curpol].phong = atof(line.substr(tmppos + 6).c_str());
                                cout<<"        phong: "<<d[curpart].p[curpol].phong<<endl;
                            } else if((tmppos = line.find(string("ambient:"))) != int(string::npos)) {
                                d[curpart].p[curpol].ambient = atof(line.substr(tmppos + 8).c_str());
                                cout<<"        ambient: "<<d[curpart].p[curpol].ambient<<endl;
                            } else if((tmppos = line.find(string("vector:"))) != int(string::npos)) {
                                d[curpart].p[curpol].vectors[curvec].x = atof(line.substr(tmppos + 7).c_str());
                                tmppos = line.find(string(","));
                                d[curpart].p[curpol].vectors[curvec].y = atof(line.substr(tmppos + 1).c_str());
                                cout<<"        vector: "<<d[curpart].p[curpol].vectors[curvec].x<<", "<<d[curpart].p[curpol].vectors[curvec].y<<endl;
                                if(abs(d[curpart].p[curpol].vectors[curvec].x) > d[curpart].size) d[curpart].size = abs(d[curpart].p[curpol].vectors[curvec].x);
                                if(abs(d[curpart].p[curpol].vectors[curvec].y) > d[curpart].size) d[curpart].size = abs(d[curpart].p[curpol].vectors[curvec].y);
                                curvec++;
                                d[curpart].p[curpol].polysize++;
                            }
                        } else if(line.find(string("begin polygon")) != string::npos) {
                            cout<<"    begin polygon\n";
                            in_polygon = 1;
                            curvec = 0;
                            d[curpart].p[curpol].polysize = 0;
                            d[curpart].p[curpol].color_r = 255;
                            d[curpart].p[curpol].color_g = 255;
                            d[curpart].p[curpol].color_b = 255;
                            d[curpart].p[curpol].tilt_dir = 0;
                            d[curpart].p[curpol].phong = 0;
                            d[curpart].p[curpol].ambient = 0.5;
                            d[curpart].size = 1.0;
                        } else if(line.find(string("type:")) != string::npos) {
                            if(line.find(string("thruster")) != string::npos) {
                                cout<<"    type: thruster\n";
                                d[curpart].type = 1;
                            } else if(line.find(string("hull")) != string::npos) {
                                cout<<"    type: hull\n";
                                d[curpart].type = 2;
                            }
                        } else if((tmppos = line.find(string("power:"))) != int(string::npos)) {
                            d[curpart].power = atof(line.substr(tmppos + 7).c_str());
                            cout<<"    power: "<<d[curpart].power<<endl;
                        } else if((tmppos = line.find(string("weapon:"))) != int(string::npos)) {
                            d[curpart].weapon = atoi(line.substr(tmppos + 8).c_str());
                            cout<<"    weapon: "<<d[curpart].weapon<<endl;
                        } else if((tmppos = line.find(string("reload:"))) != int(string::npos)) {
                            d[curpart].reload = atof(line.substr(tmppos + 8).c_str());
                            cout<<"    reload: "<<d[curpart].reload<<endl;
                        } else if((tmppos = line.find(string("thrust:"))) != int(string::npos)) {
                            d[curpart].thrust = atof(line.substr(tmppos + 8).c_str());
                            cout<<"    thrust: "<<d[curpart].thrust<<endl;
                        } else if((tmppos = line.find(string("weight:"))) != int(string::npos)) {
                            d[curpart].weight = atof(line.substr(tmppos + 8).c_str());
                            cout<<"    weight: "<<d[curpart].weight<<endl;
                        } else if((tmppos = line.find(string("strength:"))) != int(string::npos)) {
                            d[curpart].strength = atof(line.substr(tmppos + 10).c_str());
                            cout<<"    strength: "<<d[curpart].strength<<endl;
                        } else if((tmppos = line.find(string("power_draw:"))) != int(string::npos)) {
                            d[curpart].power_draw = atof(line.substr(tmppos + 12).c_str());
                            cout<<"    power_draw: "<<d[curpart].power_draw<<endl;
                        } else if((tmppos = line.find(string("fire_pos:"))) != int(string::npos)) {
                            d[curpart].fire_pos.x = atof(line.substr(tmppos + 10).c_str());
                            tmppos = line.find(string(","));
                            d[curpart].fire_pos.y = atof(line.substr(tmppos + 1).c_str());
                            cout<<"    fire_pos: "<<d[curpart].fire_pos.x<<", "<<d[curpart].fire_pos.y<<endl;
                        } else if((tmppos = line.find(string("fire_size:"))) != int(string::npos)) {
                            d[curpart].fire_size.x = atof(line.substr(tmppos + 11).c_str());
                            tmppos = line.find(string(","));
                            d[curpart].fire_size.y = atof(line.substr(tmppos + 1).c_str());
                            cout<<"    fire_size: "<<d[curpart].fire_size.x<<", "<<d[curpart].fire_size.y<<endl;
                        }
                    }
//                     cout<<"***"<<line<<endl;
                }
                file.close();
            } else break;
        }
    }
    ~RR_data() {
    }
} RR_g_data;

#endif // RR_DATA_H
