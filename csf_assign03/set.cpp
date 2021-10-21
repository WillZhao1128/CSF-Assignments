/*
 * Implementation of class functions for set class declared in set.h
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include "set.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

// determines if block with specific tag is in the set
int Set::find_block(uint32_t tag) {
    int num_blocks = blocks.size();
    for (int i = 0; i < num_blocks; i++) {               // Iterate over all blocks in the set; if same tag as one provided, then cache hit
        if (blocks[i].get_tag() == tag) {
            return i;
        }   
    }
    return -1;                                           // return negative number if not found
}

// determines what happens for a cache miss during a store; returns number of cycles; assumes miss occurred
uint32_t Set::write_allocate(char* wa, uint32_t tag) {
    if (strcmp(wa, "write-allocate") == 0) { 
        return update_cache(tag);                        // bring block into cache
    } else if (strcmp(wa, "no-write-allocate") == 0){
        return 0;                                        // don't bring block into cache
    } else {
        std::cerr << "error in argument" << std::endl;
        exit(2);
    }
}

// decides what to do in event of store (hit or miss); returns nymber of cycles
uint32_t Set::write_through(char* wt, uint32_t tag) {
    int index = find_block(tag);
    if (strcmp(wt, "write-through") == 0) {
        if (index < 0) {                            // block is not in cache (no-write-allocate)
            return MAIN_MEM_CYCLES;
        } else {                                    // block is in cache, so have to write to both locations
            return MAIN_MEM_CYCLES + CACHE_CYCLES;
        }
    } else if (strcmp(wt, "write-back") == 0) {  
        if (index < 0) {                            // block is not in cache; makes no sense for this to ever happen
            std::cerr << "write-back and no-write-allocate cannot be used together" << std::endl;
            exit(2);
        }
        blocks[index].set_dirty();                  // mark block as dirty
        return CACHE_CYCLES;                        // only write to cache
    } else {
        std::cerr << "error in argument" << std::endl; // should never occur
        exit(2);
    }
}

// Handles all stores
uint32_t Set::store(uint32_t tag, char* wa, char* wt, char* lru) {
    int index = find_block(tag);

    uint32_t cycles = 0;
    if (index >= 0) {                               // Cache hit! Only have to worry about write-through
        cycles = write_through(wt, tag);
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);                   // Most recently used will be pushed to end of vector
        }
        return cycles;

    } else {                                        // Cache miss!
        cycles = write_allocate(wa, tag);           // Determine to bring into cache before store
        cycles = cycles + write_through(wt, tag);   // Store the value
        return cycles;
    }
}

// Handles all loads
uint32_t Set::load(uint32_t tag, char*lru) {
    int index = find_block(tag);

    uint32_t cycles = 0;
    if (index >= 0) {                               // Cache hit!
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);                   // Most recently used will be pushed to end of vector
        }
        return CACHE_CYCLES;                        // If fifo, don't have to update anything b/c order already self-managed
    } else {                                        // Cache miss!
        cycles = update_cache(tag);                 // Load block from main memory to cache
        return cycles;
    }       
}

// Ensure the most recently used block is at the end of the vector
void Set::lru_rearrange(int index) {
    Block block = blocks[index];
    blocks.erase(blocks.begin() + index);
    blocks.push_back(block);
}

// Decides to evict based on current size of cache; also adds a new block to end of vector; only for cache misses
uint32_t Set::update_cache(uint32_t tag) {
    uint32_t cycles = 0;
    if (blocks.size() < size) {                     // If cache not full, get block from main memory
        Block block(tag);
        blocks.push_back(block);                    // For either lru or fifo, just bring block into cache
        cycles = MAIN_MEM_CYCLES * block_size;      // Main memory access
        return cycles;
    } else {                                        // size is full! have to evict...
        if (blocks[0].is_dirty()) {                 // If the block is dirty, then have to store value to main memory
            cycles = MAIN_MEM_CYCLES * block_size;
        }
        blocks.erase(blocks.begin());               // Evict the least recently used block
        Block block(tag);
        blocks.push_back(block);
        cycles = cycles + MAIN_MEM_CYCLES * block_size; // Must bring new block from main memory
        return cycles;
    }
}

// Stores all dirty values to main memory
uint32_t Set::store_all_dirty() {
    uint32_t cycles = 0;
    for (uint32_t i = 0; i < size; i++) {
        if (blocks[i].is_dirty()) {
            cycles = cycles + MAIN_MEM_CYCLES * block_size;
        }
    }
    return cycles;
}