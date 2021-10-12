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
#include <cctype>

using std::cout;
using std::endl;
using std::cerr;
using std::isdigit;
using std::string;
using std::stoi;

int main(int argc, char *argv[]) {
    // If the argument count is greater than 7, then it is invalid
    if (argc != 7) {
        cerr << "Invalid number of arguments!" << endl;
        exit(1);
    }
    // Handle all possible arguments
    if (handle_all_arguments(argv) == 0) {
        cerr << "Invalid argument!" << endl;
        exit(1);
    }
    string s;
    while (std::getline(std::cin, s)){ // Read a line in trace file (\n terminated)
        // Analyze memory address
        check_valid_trace(s);
        string address = s.substr(2, 10);
        cout << address << endl;

        //cout << s << endl;
    }
    
    cout << "hello " << argv[1] << "!" << endl;
    return 0;
}
