/*
 * Main function implementation for cache simulator
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include "helper.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::endl;
using std::cerr;
using std::isdigit;
using std::string;


int main(int argc, char *argv[]) {
    if (argc != 7) {
        cerr << "Invalid number of arguments!" << endl;
        exit(1);
    }
    if (handle_all_arguments(argv) == 0) {
        cerr << "Invalid argument!" << endl;
        exit(1);
    }
    string s;
    while (std::getline(std::cin, s)){
        cout << s << endl;
    }
    
    cout << "hello " << argv[1] << "!" << endl;
    return 0;
}



