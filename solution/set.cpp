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

int Set::find_block(uint32_t tag) {
    int num_blocks = blocks.size();
    // Iterate over all blocks in the set; if same tag as one provided, then cache hit
    for (int i = 0; i < num_blocks; i++) {
        if (blocks[i].get_tag() == tag) {
            return i;
        }   
    }
    return -1;
}

// determines what happens for a cache miss during a store; returns cycles
uint32_t Set::write_allocate(char* wa, uint32_t tag) {
    // If write-allocate is true, then first have to bring this into main memory
    if (strcmp(wa, "write-allocate") == 0) {
        return check_cache(tag);
    } else if (strcmp(wa, "no-write-allocate") == 0){
        return 0;
    } else {
        std::cerr << "error in argument" << std::endl;
        exit(2);
    }
}

// decides what to do in event of store (hit or miss)
uint32_t Set::write_through(char* wt, uint32_t tag) {
    int index = find_block(tag);
    // if write-through is true, then we have to access both the cache and main memory
    if (strcmp(wt, "write-through") == 0) {
        if (index < 0) { //block is not in cache
            return MAIN_MEM_CYCLES * block_size;
        } else { // block is in cache, so have to write to both locations
            return MAIN_MEM_CYCLES * block_size + CACHE_CYCLES;
        }
    } else if (strcmp(wt, "write-back") == 0) { // if write-back, then simply mark that block as dirty; if we access the same block + replace, will also have to mark as dirty
        if (index < 0) { // block is not in cache; makes no sense for this to ever happen
            std::cerr << "write-back and no-write-allocate cannot be used together" << std::endl;
            exit(2);
        }
        blocks[index].set_dirty();
        return CACHE_CYCLES;
    } else {
        std::cerr << "error in argument" << std::endl;
        exit(2);
    }
}

// Handles all stores
uint32_t Set::store(uint32_t tag, char* wa, char* wt, char* lru) {
    int index = find_block(tag);
    uint32_t cycles = 0;
    // Cache hit! Only have to worry about write-through
    if (index >= 0) {
        cycles = write_through(wt, tag);
        // Handle lru
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);
        }
        return cycles;

    } else { // Cache miss!
        // If lru or fifo, just add to end of vector
        cycles = write_allocate(wa, tag);
        cycles = cycles + write_through(wt, tag);
        return cycles;
    }
}

uint32_t Set::load(uint32_t tag, char*lru) {
    int index = find_block(tag);
    uint32_t cycles = 0;
    
    // Cache hit!
    if (index >= 0) {
        // If lru, update vector so most recent is block accessed
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);
        }
        // If fifo, don't have to update anything cuz order already self-managed
        return CACHE_CYCLES;
    } else { // Cache miss! Have to get from main
        cycles = check_cache(tag);
        return cycles;
    }       
}

void Set::lru_rearrange(int index) {
    Block block = blocks[index];
    blocks.erase(blocks.begin() + index);
    blocks.push_back(block);
}

// decides to evict based on current size of cache; also adds a new block to end of vector // only misses
uint32_t Set::check_cache(uint32_t tag) {
    uint32_t cycles = 0;
    // If cache not full, get from main memory and place into spot
    if (blocks.size() < size) {
        // Works for both lru and fifo
        Block block(tag);
        blocks.push_back(block);
        cycles = MAIN_MEM_CYCLES * block_size;
        return cycles;

    } else { // size is full! have to evict... remove from head! (works for both lru and fifo)
        if (blocks[0].is_dirty()) {
            cycles = MAIN_MEM_CYCLES * block_size;
        }
        blocks.erase(blocks.begin());
        Block block(tag);
        blocks.push_back(block);
        cycles = cycles + MAIN_MEM_CYCLES * block_size;
        return cycles;
    }
}

uint32_t Set::store_all_dirty() {
    uint32_t cycles = 0;
    for (uint32_t i = 0; i < size; i++) {
        if (blocks[i].is_dirty()) {
            cycles = cycles + MAIN_MEM_CYCLES * block_size;
        }
    }
    return cycles;
}