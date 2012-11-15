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

#ifndef RR_DATA_MAX_UNITS
#define RR_DATA_MAX_UNITS 50
#endif

#ifndef RR_DATA_MAX_UNIT_PARTS
#define RR_DATA_MAX_UNIT_PARTS 20
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
struct RR_data_unit_part {
    unsigned char type;
    bool increment;
    RR_vec2 pos;
    int parent;
    bool in_use;
};
struct RR_data_unit {
    RR_data_unit_part p[RR_DATA_MAX_UNIT_PARTS];
    char name[50];
    char type;
};

class RR_data {
public:
    RR_data_part d[RR_DATA_MAX_PARTS];
    RR_data_unit u[RR_DATA_MAX_UNITS];
    int curpart, curunit;
    RR_data() {
        curpart = 0;
        curunit = 0;
        for(int i = 0; i < RR_DATA_MAX_PARTS; i++) {
            d[i].type = 0;
        }
        for(int i = 0; i < RR_DATA_MAX_UNITS; i++) {
            u[i].type = 0;
        }
        string line;
        char str[50];
        ifstream file;
        char type_handle = 0;
        bool in_polygon = 0;
        int curpol = 0;
        int curvec = 0;
        int tmppos = 0;
        int unitparts = 0;
        bool in_upart = 0;
        for(int i = 0; i < 100; i++) {
            sprintf(str, "data/data_%02d.txt", i);
            file.open(str);
            if(file.is_open()) {
                cout<<"Reading file "<<str<<endl;
                while(file.good()) {
                    getline(file, line);
                    
                    // Skip comments
                    if(line.find(string("#")) != string::npos) continue;
                    
                    // Look for beginning
                    if(type_handle == 0) {
                        if((tmppos = line.find(string("begin part"))) != int(string::npos)) {
                            curpart = atoi(line.substr(tmppos + 11).c_str());
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
                            cout<<"  * Loading part: #"<<curpart;
                            type_handle = 1;
                            curpol = 0;
                            in_polygon = 0;
                        } else if((tmppos = line.find(string("begin unit"))) != int(string::npos)) {
                            curunit = atoi(line.substr(tmppos + 11).c_str());
                            u[curunit].type = 0;
                            cout<<"  * Loading unit: #"<<curunit;
                            type_handle = 2;
                            for(int i = 0; i < RR_DATA_MAX_UNIT_PARTS; i++) u[curunit].p[i].in_use = 0;
                            unitparts = 0;
                            in_upart = 0;
                        }
                    } else if(type_handle == 1) {
                        if(line.find(string("end part")) != string::npos) {
                            switch(d[curpart].type) {
                            case 1: cout<<" (Type Thruster"; break;
                            case 2: cout<<" (Type Hull"; break;
                            case 3: cout<<" (Type Cockpit"; break;
                            case 4: cout<<" (Type Weapon"; break;
                            case 5: cout<<" (Type Core"; break;
                            default: cout<<" (Type Unknown";
                            }
                            if(curpol == 1) cout<<", 1 polygon)\n";
                            else cout<<", "<<curpol<<" polygons)\n";
                            type_handle = 0;
                        } else if(in_polygon) {
                            if(line.find(string("end polygon")) != string::npos) {
                                in_polygon = 0;
                                curpol++;
                            } else if((tmppos = line.find(string("color_r:"))) != int(int(string::npos))) {
                                d[curpart].p[curpol].color_r = atoi(line.substr(tmppos + 8).c_str());
                            } else if((tmppos = line.find(string("color_g:"))) != int(string::npos)) {
                                d[curpart].p[curpol].color_g = atoi(line.substr(tmppos + 8).c_str());
                            } else if((tmppos = line.find(string("color_b:"))) != int(string::npos)) {
                                d[curpart].p[curpol].color_b = atoi(line.substr(tmppos + 8).c_str());
                            } else if((tmppos = line.find(string("tilt_dir:"))) != int(string::npos)) {
                                d[curpart].p[curpol].tilt_dir = atof(line.substr(tmppos + 9).c_str());
                            } else if((tmppos = line.find(string("phong:"))) != int(string::npos)) {
                                d[curpart].p[curpol].phong = atof(line.substr(tmppos + 6).c_str());
                            } else if((tmppos = line.find(string("ambient:"))) != int(string::npos)) {
                                d[curpart].p[curpol].ambient = atof(line.substr(tmppos + 8).c_str());
                            } else if((tmppos = line.find(string("vector:"))) != int(string::npos)) {
                                d[curpart].p[curpol].vectors[curvec].x = atof(line.substr(tmppos + 7).c_str());
                                tmppos = line.find(string(","));
                                d[curpart].p[curpol].vectors[curvec].y = atof(line.substr(tmppos + 1).c_str());
                                if(abs(d[curpart].p[curpol].vectors[curvec].x) > d[curpart].size) d[curpart].size = abs(d[curpart].p[curpol].vectors[curvec].x);
                                if(abs(d[curpart].p[curpol].vectors[curvec].y) > d[curpart].size) d[curpart].size = abs(d[curpart].p[curpol].vectors[curvec].y);
                                curvec++;
                                d[curpart].p[curpol].polysize++;
                            }
                        } else if(line.find(string("begin polygon")) != string::npos) {
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
                                d[curpart].type = 1;
                            } else if(line.find(string("hull")) != string::npos) {
                                d[curpart].type = 2;
                            } else if(line.find(string("cockpit")) != string::npos) {
                                d[curpart].type = 3;
                            } else if(line.find(string("weapon")) != string::npos) {
                                d[curpart].type = 4;
                            } else if(line.find(string("core")) != string::npos) {
                                d[curpart].type = 5;
                            }
                        } else if((tmppos = line.find(string("power:"))) != int(string::npos)) {
                            d[curpart].power = atof(line.substr(tmppos + 7).c_str());
                        } else if((tmppos = line.find(string("weapon:"))) != int(string::npos)) {
                            d[curpart].weapon = atoi(line.substr(tmppos + 8).c_str());
                            switch(d[curpart].weapon) {
                            case 1: d[curpart].weapon = 1; break;
                            case 2: d[curpart].weapon = 5; break;
                            default: cout<<"Unknown weapon type: "<<d[curpart].weapon<<endl;
                            }
                        } else if((tmppos = line.find(string("reload:"))) != int(string::npos)) {
                            d[curpart].reload = atof(line.substr(tmppos + 8).c_str());
                        } else if((tmppos = line.find(string("thrust:"))) != int(string::npos)) {
                            d[curpart].thrust = atof(line.substr(tmppos + 8).c_str());
                        } else if((tmppos = line.find(string("weight:"))) != int(string::npos)) {
                            d[curpart].weight = atof(line.substr(tmppos + 8).c_str());
                        } else if((tmppos = line.find(string("strength:"))) != int(string::npos)) {
                            d[curpart].strength = atof(line.substr(tmppos + 10).c_str());
                        } else if((tmppos = line.find(string("power_draw:"))) != int(string::npos)) {
                            d[curpart].power_draw = atof(line.substr(tmppos + 12).c_str());
                        } else if((tmppos = line.find(string("fire_pos:"))) != int(string::npos)) {
                            d[curpart].fire_pos.x = atof(line.substr(tmppos + 10).c_str());
                            tmppos = line.find(string(","));
                            d[curpart].fire_pos.y = atof(line.substr(tmppos + 1).c_str());
                        } else if((tmppos = line.find(string("fire_size:"))) != int(string::npos)) {
                            d[curpart].fire_size.x = atof(line.substr(tmppos + 11).c_str());
                            tmppos = line.find(string(","));
                            d[curpart].fire_size.y = atof(line.substr(tmppos + 1).c_str());
                        }
                    } else if(type_handle == 2) {
                        if(line.find(string("end unit")) != string::npos) {
                            switch(u[curunit].type) {
                            case 1: cout<<" (Type Fighter"; break;
                            default: cout<<" (Type Unknown";
                            }
                            if(unitparts == 1) cout<<", 1 part)\n";
                            else cout<<", "<<unitparts<<" parts)\n";
                            type_handle = 0;
                        } else if(in_upart) {
                            if(line.find(string("end upart")) != string::npos) {
                                in_upart = 0;
                                unitparts++;
                            } else if((tmppos = line.find(string("type:"))) != int(int(string::npos))) {
                                u[curunit].p[curpart].type = atoi(line.substr(tmppos + 5).c_str());
                            } else if(line.find(string("increment:")) != string::npos) {
                                if(line.find(string("team")) != string::npos) u[curunit].p[curpart].increment = 1;
                                else u[curunit].p[curpart].increment = 0;
                            } else if((tmppos = line.find(string("vector:"))) != int(string::npos)) {
                                u[curunit].p[curpart].pos.x = atof(line.substr(tmppos + 7).c_str());
                                tmppos = line.find(string(","));
                                u[curunit].p[curpart].pos.y = atof(line.substr(tmppos + 1).c_str());
                            } else if((tmppos = line.find(string("parent:"))) != int(string::npos)) {
                                u[curunit].p[curpart].parent = atoi(line.substr(tmppos + 7).c_str());
                            }
                        } else if((tmppos = line.find(string("begin upart"))) != int(string::npos)) {
                            in_upart = 1;
                            curpart = atoi(line.substr(tmppos + 11).c_str());
                            u[curunit].p[curpart].in_use = 1;
                        } else if(line.find(string("type:")) != string::npos) {
                            if(line.find(string("fighter")) != string::npos) {
                                u[curunit].type = 1;
                            }
                        }
                    }
                }
                file.close();
            }
        }
    }
    ~RR_data() {
    }
} RR_g_data;

#endif // RR_DATA_H
