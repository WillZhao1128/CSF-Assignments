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

Cache create_cache(uint32_t sets_per_cache, uint32_t block_per_set);

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
    
    uint32_t sets_per_cache = stoi(argv[1], nullptr, 10); 
    uint32_t blocks_per_set = stoi(argv[2], nullptr, 10); // for DM: 1 set = 1 block, AM: 1 set = n block
    uint32_t bytes_per_block = stoi(argv[3], nullptr, 10);

    uint32_t offset_bits_size = log2(bytes_per_block);
    uint32_t index_bits_size = log2(sets_per_cache);
    uint32_t tag_bits_size = total_addr_bits - index_bits_size - offset_bits_size;

    cout << "offset bit size is " << offset_bits_size << endl;
    cout << "index bit size is " << index_bits_size << endl;
    cout << "tag bit size is " << tag_bits_size << endl;

    string s;
    int output[7] = {0};

    Cache cache = create_cache(sets_per_cache, blocks_per_set);
    
    while (std::getline(std::cin, s)){ // Read a line in trace file (\n terminated)
        // Analyze memory address (get bits for each component)
        check_valid_trace(s);
        string s_addr = s.substr(4, 8);                         // get the address from the string
        //cout << s_addr << endl;
        uint32_t int_addr = stoul(s_addr, nullptr, 16);         // convert hex string into decimal
        
        uint32_t index = get_bits(index_bits_size, offset_bits_size, int_addr);
        uint32_t tag = get_bits(tag_bits_size, index_bits_size + offset_bits_size, int_addr);

        cout << "index is " << index << endl;
        cout << "tag is " << tag << endl;
        
        if (s[0] == 'l') { // is a load
            output[0] = output[0] + 1;
            if (cache.find_block(tag, index) == 1) {
                output[2] = output[2] + 1;       
                
            } else {
                output[3] = output[3] + 1;
            }
        } else {
            output[1] = output[1] + 1;
            if (cache.find_block(tag, index) == 1) {
                output[4] = output[4] + 1;            
            } else {
                output[5] = output[5] + 1;
            }
        }


        //cout << offset << endl;
        //cout << index << endl;
        //cout << s << endl;
        output[6] = output[6] + 1;
    }
    
    print_output(output, 7);

    return 0;
}

bool check_hit() {
    //todo
    return 0;
}

Cache create_cache(uint32_t sets_per_cache, uint32_t block_per_set) {
    Cache cache;
    // Loop over all of the sets
    for (int i = 0; i < sets_per_cache; i++) {
        Set set;
        // Loop over all of the blocks
        for (int i = 0; i < block_per_set; i++) {
            Block block;
            set.create_block(block);
        }
        cache.create_set(set);
    }

    cout << "number sets " << cache.num_sets() << endl;
    cout << "number blocks per set " << cache.get_set(0).num_blocks() << endl;

    return cache;
}