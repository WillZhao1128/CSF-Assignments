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
using std::string;

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


void check_valid_trace(string s) {
    if (s[0] != 'l' && s[0] != 's' && s[1] != ' ') {
        cerr << "Invalid trace!" << endl;
        exit(1);
    }
    string address = s.substr(2, 10);
    if (address.substr(0, 2) != "0x") {
        cerr << "Address must be in hex!1" << endl;
        exit(1);
    }
    int num_byte = 8; // 32 bit address
    for (int i = 2; i < num_byte + 2; i++) {
        if (!isxdigit(address[i])) {
            cerr << "Address must be in hex!" << endl;
            exit(1);
        }
    
    }
}

uint32_t get_bits(uint32_t num_bits, uint32_t left, uint32_t address) {
    uint32_t mask = 1;
    mask = (((mask << num_bits) - 1) << left); // Mask with 1's in desired location
    uint32_t val = (address & mask) >> left;
    return val;
}

void print_output(int output[], int len) {
    if (len != 7) {
        cerr << "print_output is meant for diagnostic data for cache!" << endl;
        exit(2);
    }
    cout << "Total loads: " << output[0] << endl;
    cout << "Total stores: " << output[1] << endl;
    cout << "Load hits: " << "" << endl;
    cout << "Load misses: " << "" << endl;
    cout << "Store hits: " << "" << endl;
    cout << "store misses: " << "" << endl;
    cout << "Total cycles: " << output[6] << endl;
}
