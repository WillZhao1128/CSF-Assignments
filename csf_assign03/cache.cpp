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


int Set::find_block(uint32_t tag) {
    int num_blocks = blocks.size();
    for (int i = 0; i < num_blocks; i++) {
        if (blocks[i].get_tag() == tag) {
            return i;
        }
    }
    
    return -1;
}




uint32_t Set::write_allocate(char* wa, int index, uint32_t tag) {
    // determines what happens for a cache miss during a store:
    if (strcmp(wa, "write-allocate")) {
        blocks[index].set_tag(tag);
        return 101;

    } else {
        return 100;
    }
}



bool Cache::find_block(uint32_t tag, uint32_t index) {
    // If a tag at a specific set, or index, is found, then it was hit
    if (sets[index].find_block(tag) == 1) {
        return 1;
    } else { // if no tag was found, it was a miss. Depending on

    }
    
    return 0;
}

Cache::Cache(char* arg1) {
    uint32_t sets_per_cache = stoi(arg1, nullptr, 10); 

    
    for (uint32_t i = 0; i < sets_per_cache; i++) {
        Set set;
        sets.push_back(set);
    }
}