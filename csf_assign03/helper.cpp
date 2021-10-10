#include "helper.h"

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <cassert>
#include <string.h>
#include <math.h>

using std::cout;
using std::endl;
using std::cerr;
using std::isdigit;

bool is_valid_number(char* arg) {
    for (int i = 0; i < strlen(arg); i++) {
        if (!isdigit(arg[i])) {
            return 0;
        }
    }
    int input = std::stoi(arg,nullptr,10);
    if (input <= 0) {
        return 0;
    }
    if (ceil(log2(input)) != floor(log2(input))) {
        if (input != 1) {
            return 0;
        }
    }
    return 1;
}

bool handle_number_arguments(char* argv[]) {
    // Check if the first three arguments are numbers
    if (is_valid_number(argv[1]) == 0 || is_valid_number(argv[2]) == 0 || is_valid_number(argv[3]) == 0) {
        return 0;
    }
    if (std::stoi(argv[3],nullptr,10) < 4 ) {
        return 0;
    }
    return 1;

}

bool handle_string_arguments(char* argv[]) {
    if (strcmp(argv[4], "write-allocate") != 0 && strcmp(argv[4], "no-write-allocate") != 0) {
        cout << "You typed: " << argv[3] << endl;
        return 0;
    } else if (strcmp(argv[5], "write-through") != 0 && strcmp(argv[5], "write-back") != 0) {
        cout << "You typed: " << argv[4] << endl;
        return 0;
    } else if (strcmp(argv[6], "lru") != 0 && strcmp(argv[6], "fifo") != 0) {
        cout << "You typed: " << argv[5] << endl;
        return 0;
    } else if (strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0) {
        cout << "no-write-allocate cannot be used with write-back!" << endl;
        return 0;
    }
    return 1;
}