/*
 * Main function implementation for cache simulator
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include "helper.h"
#include "cache.h"
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

bool check_hit();
void print_output(int output[], int len) ;

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
    uint32_t total_addr_bits = 32;
    uint32_t offset_size_bits = log2(stoi(argv[3], nullptr, 10));
    uint32_t index_size_bits = log2(stoi(argv[1], nullptr, 10));
    uint32_t tag_size_bits = total_addr_bits - offset_size_bits - index_size_bits;

    string s;
    int output[7] = {0};

    Cache cache;
    
    while (std::getline(std::cin, s)){ // Read a line in trace file (\n terminated)
        // Analyze memory address (get bits for each component)
        check_valid_trace(s);
        string s_addr = s.substr(4, 8);
        uint32_t int_addr = stoi(s_addr, nullptr, 16);
        uint32_t offset = get_bits(offset_size_bits, 0, int_addr);// DOESN'T MATTER
        uint32_t index = get_bits(index_size_bits, offset_size_bits, int_addr);

        bool hit = check_hit();
        
        if (s[0] == 'l') { // is a load
            output[0] = output[0] + 1;
        } else {
            output[1] = output[1] + 1;
        }


        cout << offset << endl;
        cout << index << endl;
        cout << s << endl;
        output[6] = output[6] + 1;
    }
    
    print_output(output, 7);

    return 0;
}

bool check_hit() {
    //todo
    return 0;
}

void print_output(int output[], int len) {
    cout << "Total loads: " << output[0] << endl;
    cout << "Total stores: " << output[1] << endl;
    cout << "Load hits: " << "" << endl;
    cout << "Load misses: " << "" << endl;
    cout << "Store hits: " << "" << endl;
    cout << "store misses: " << "" << endl;
    cout << "Total cycles: " << output[6] << endl;
}