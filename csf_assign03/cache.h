/*
 * Declaration of class functions for the cache object
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#ifndef CACHE_H 
#define CACHE_H

#include "set.h"
#include <vector>

using std::vector;

class Cache { // Has a sequence of set objects;

public:
    Cache() {}
    
    // Creates a cache with specifications
    Cache(char* arg1, char* arg2, char* arg3);

    // Getter for a specific set
    Set get_set(int index) { return sets[index]; }

    // Setter for a specific set
    void set_set(int index, Set set) { sets[index] = set; }

    // returns how many sets this cache has
    uint32_t num_sets() { return sets.size(); }

    // finds the block in a specified set
    bool find_block(uint32_t tag, uint32_t index);

    // performs store operation on a specific set
    uint32_t store(uint32_t tag, uint32_t index, char* wa, char* wt, char* lru) {
        return sets[index].store(tag, wa, wt, lru);
    }

    // performs a load operation on a specific set
    uint32_t load(uint32_t tag, uint32_t index, char* lru) {
        return sets[index].load(tag, lru);
    }

    uint32_t store_all_dirty();

private:
    vector<Set> sets;
    uint32_t block_size;
};


#endif