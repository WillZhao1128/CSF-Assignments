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
    for (int i = 0; i < num_blocks; i++) {
        if (blocks[i].get_tag() == tag) {
            return i;
        }   
    }
    return -1;
}

// determines what happens for a cache miss during a store; returns cycles
uint32_t Set::write_allocate(char* wa, uint32_t tag) {
    if (strcmp(wa, "write-allocate") == 0) {
        check_cache(tag);
        return MAIN_MEM_CYCLES * block_size;
    } else if (strcmp(wa, "no-write-allocate") == 0){
        return MAIN_MEM_CYCLES * block_size;
    } else {
        std::cerr << "error in argument" << std::endl;
        exit(2);
    }
}

// decides what to do in event of store (hit or miss)
uint32_t Set::write_through(char* wt, int index) {
    // if write-through is true, then we have to access both the cache and main memory
    if (strcmp(wt, "write-through") == 0) {
        return MAIN_MEM_CYCLES * block_size + CACHE_CYCLES;
    } else if (strcmp(wt, "write-back") == 0) { // if write-back, then simply mark that block as dirty; if we access the same block + replace, will also have to mark as dirty
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
        cycles = write_through(wt, index);
        // Handle lru
        if (strcmp(lru, "lru") == 0) {
            lru_rearrange(index);
        }
        return cycles;

    } else { // Cache miss!
        // If lru or fifo, just add to end of vector
        index = (blocks.size() - 1);
        cycles = write_allocate(wa, tag);
        index = (blocks.size() - 1);
        cycles = cycles + write_through(wt, index);
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