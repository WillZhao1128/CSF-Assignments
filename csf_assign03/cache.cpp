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
    //cout << num_blocks << endl;
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
        Block block(tag);
        blocks.push_back(block);
        return 101;

    } else {
        return 100;
    }
}

// decides what to do in event of store (hit or miss)
uint32_t Set::write_through(char* wt, int index, uint32_t tag) {
    // if write-through is true, then we have to access both the cache and main memory
    if (strcmp(wt, "write-through")) {
        //blocks[index].set_tag(tag);
        return 101;
    } else {
        blocks[index].set_dirty();
        return 1;
    }
}

// Handles all stores
uint32_t Set::store(uint32_t tag, char* wa, char* wt, char* lru) {
    int index = find_block(tag);
    uint32_t cycles = 0;
    // Cache hit! Only have to worry about write-through
    if (index >= 0) {
        cycles = write_through(wt, index, tag);
        // Handle lru
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);
        }
        return cycles;

    } else { // Cache miss!
        evict(tag);

        // If lru or fifo, just add to end of vector
        index = (blocks.size() - 1);
        cycles = write_allocate(wa, index, tag);
        cycles = cycles + write_through(wt, index, tag);
        
        return cycles;
    }
}

uint32_t Set::load(uint32_t tag, char*lru) {
    int index = find_block(tag);
    uint32_t cycles = 0;
    
    // Cache hit!
    if (index >= 0) {
        //cout << "hi" << endl;
        // If lru, update vector so most recent is block accessed
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);
        }

        // If fifo, don't have to update anything cuz order already self-managed
        return 1;
    } else { // Cache miss! Have to get from main
        cycles = evict(tag);
        Block block(tag);
        blocks.push_back(block);
        cycles = cycles + 100;
        return cycles;
    }       
}

void Set::lru_rearrange(int index) {
    Block block = blocks[index];
    blocks.erase(blocks.begin() + index);
    blocks.push_back(block);
}

// decides to evict or not; 
uint32_t Set::evict(uint32_t tag) {
    // If not full, just pushback
    uint32_t cycles = 0;
    if (blocks.size() != size) {
        // Works for both lru and fifo
        Block block(tag);
        blocks.push_back(block);
        return cycles;

    } else { // size is full! have to evict... remove from head! (works for both lru and fifo)
        if (blocks[0].is_dirty()) {
            cycles = 100;
        }
        blocks.erase(blocks.begin());
        return cycles;
    }
}



bool Cache::find_block(uint32_t tag, uint32_t index) {
    // If a tag at a specific set, or index, is found, then it was hit
    if (sets[index].find_block(tag) >= 0) {
        return 1;
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