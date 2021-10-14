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
    
    //uint32_t blocks_per_set = stoi(argv[2], nullptr, 10); // for DM: 1 set = 1 block, AM: 1 set = n block

    uint32_t offset_bits_size = log2(stoi(argv[3], nullptr, 10));
    uint32_t index_bits_size = log2(stoi(argv[1], nullptr, 10));
    uint32_t tag_bits_size = total_addr_bits - index_bits_size - offset_bits_size;

    cout << "offset bit size is " << offset_bits_size << endl;
    cout << "index bit size is " << index_bits_size << endl;
    cout << "tag bit size is " << tag_bits_size << endl;

    Cache cache(argv[1]);
    string s;
    int output[7] = {0};
    while (std::getline(std::cin, s)){ // Read a line in trace file (\n terminated)
        // Analyze memory address (get bits for each component)
        check_valid_trace(s);

        string s_addr = s.substr(4, 8);                         // get the address from the string
        uint32_t int_addr = stoul(s_addr, nullptr, 16);         // convert hex string into decimal
        uint32_t index = get_bits(index_bits_size, offset_bits_size, int_addr);
        uint32_t tag = get_bits(tag_bits_size, index_bits_size + offset_bits_size, int_addr);

        cout << "index is " << index << endl;
        cout << "tag is " << tag << endl;
        uint32_t cycles = 0;
        if (s[0] == 'l') { // is a load
            output[0] = output[0] + 1;
            if (cache.find_block(tag, index) == 1) {
                output[2] = output[2] + 1;
                
            } else {
                output[3] = output[3] + 1;
            }
            cycles = cache.load(tag, index, argv[6]);
        } else {
            output[1] = output[1] + 1;
            if (cache.find_block(tag, index) == 1) {
                output[4] = output[4] + 1;            
            } else {
                output[5] = output[5] + 1;
            }
            cycles = cache.store(tag, index, argv[4], argv[5], argv[6]);
        }
        output[6] = output[6] + cycles;
    }
    
    print_output(output, 7);

    return 0;
}