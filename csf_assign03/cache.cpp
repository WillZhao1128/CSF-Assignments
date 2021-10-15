/*
 * Implementation of class functions for classes declared in cache.h
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include "cache.h"
#include "set.h"
#include <cstring>
#include <string>
#include <cmath>

using std::string;
using std::stoi;

bool Cache::find_block(uint32_t tag, uint32_t index) {
    // If a tag at a specific set, or index, is found, then it was hit
    if (sets[index].find_block(tag) >= 0) {
        return 1;
    }
    return 0;
}

Cache::Cache(char* arg1, char* arg2, char* arg3) {
    uint32_t sets_per_cache = stoi(arg1, nullptr, 10); 
    uint32_t blocks_per_set = stoi(arg2, nullptr, 10); 
    uint32_t bytes_per_block = stoi(arg3, nullptr, 10);
    block_size = log2(bytes_per_block);
    
    for (uint32_t i = 0; i < sets_per_cache; i++) {
        Set set(blocks_per_set, block_size);
        sets.push_back(set);
    }
}

uint32_t Cache::store_all_dirty() {
    uint32_t num_sets = sets.size();
    uint32_t cycles = 0;
    for (uint32_t i = 0; i < num_sets; i++) {
        cycles = cycles + sets[i].store_all_dirty();
        
    }
    return cycles;
}