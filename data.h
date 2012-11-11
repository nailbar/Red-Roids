#ifndef RR_DATA_H
#define RR_DATA_H 1

#include <iostream>
#include <fstream>
using namespace std;

#include "vector.h"

// Theory:
//  * Constructor loads all files in "data" directory
//    and saves it in data nodes which can then be read
//    on demand... Or something.

class RR_data {
public:
    RR_data() {
        cout<<"Loading data...\n";
        string line;
        char str[50];
        ifstream file;
        for(int i = 0; i < 1000; i++) {
            sprintf(str, "data/part_%03d.txt", i);
            file.open(str);
            if(file.is_open()) {
                cout<<"Reading file "<<str<<endl;
                while(file.good()) {
                    getline(file, line);
                    cout<<line<<endl;
                }
                file.close();
            } else {
                cout<<"File "<<str<<" not found. Breaking loop.\n";
                break;
            }
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
        }
    }
    ~RR_data() {
        cout<<"Unloading data...\n";
    }
} RR_g_data;

#endif // RR_DATA_H
