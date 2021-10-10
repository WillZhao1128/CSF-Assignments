/*
 * Implementation of helper functions declared in helper.h
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

bool is_valid_number(char* arg) {
    for (int i = 0; i < strlen(arg); i++) {
        if (!isdigit(arg[i])) {
            cerr << "Not a number!" << endl;
            return 0;
        }
    }
    int input = std::stoi(arg,nullptr,10);
    if (input <= 0) {
        cerr << "Must be a positive number!" << endl;
        return 0;
    }
    if (ceil(log2(input)) != floor(log2(input))) {
        if (input != 1) {
            cerr << "Integers must be powers of 2!" << endl;
            return 0;
        }
    }
    return 1;
}

bool handle_all_arguments(char* argv[]) {
    if (handle_number_arguments(argv) == 0) {
        return 0;
    } else if (handle_string_arguments(argv) == 0) {
        return 0;
    }
    return 1;
}

bool handle_number_arguments(char* argv[]) {
    // Check if the first three arguments are numbers
    if (is_valid_number(argv[1]) == 0 || is_valid_number(argv[2]) == 0 || is_valid_number(argv[3]) == 0) {
        return 0;
    } else if (std::stoi(argv[3],nullptr,10) < 4 ) {
        cerr << "Argument 3 must be at least 4!" << endl;
        return 0;
    }
    return 1;

}

bool handle_string_arguments(char* argv[]) {
    if (strcmp(argv[4], "write-allocate") != 0 && strcmp(argv[4], "no-write-allocate") != 0) {
        cerr << "You typed: " << argv[3] << endl;
        return 0;
    } else if (strcmp(argv[5], "write-through") != 0 && strcmp(argv[5], "write-back") != 0) {
        cerr << "You typed: " << argv[4] << endl;
        return 0;
    } else if (strcmp(argv[6], "lru") != 0 && strcmp(argv[6], "fifo") != 0) {
        cerr << "You typed: " << argv[5] << endl;
        return 0;
    } else if (strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0) {
        cerr << "no-write-allocate cannot be used with write-back!" << endl;
        return 0;
    }
    return 1;
}