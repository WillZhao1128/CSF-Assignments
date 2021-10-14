#ifndef CACHE_H 
#define CACHE_H

#include <vector>
#include <cstring>
#include <cctype>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;

class Block {
public:

    Block(uint32_t tag_add){
        this->tag = tag_add;
        this->dirty = 0;
        this->valid = 0;
    }

    uint32_t get_tag() { return this->tag; }

    void set_tag(uint32_t tag) {
        this->valid = 1;
        this->tag = tag;
    }

    void set_dirty() { this->dirty = 1; }

    bool is_dirty() {
        return dirty;
    }

private:
    uint32_t tag;
    bool dirty;
    bool valid;

};

class Set {
public:
    Set() {}

    // getter for a specific block
    Block get_block(int index) { return blocks[index]; }

    // setter for a specfici block
    void set_block(int index, Block block) { blocks[index] = block; }

    // called when creating the cache DELETE
    void create_block(Block block) { blocks.push_back(block); }

    // returns the number of blocks in set
    uint32_t num_blocks() { return blocks.size(); }

    // finds a block with a specific tag. returns -1 if not found.
    int find_block(uint32_t tag);

    // determines to write-allocate or no-write allocate
    uint32_t write_allocate(char* wa, int index, uint32_t tag);

    uint32_t store(uint32_t tag, char* wa, char* wt, char* lru) {
        int index = find_block(tag);
        uint32_t cycles = 0;
        // Cache hit
        if (index > 0) {
            cycles = write_through(wt, index, tag);
            Block block = blocks[index];
            blocks.erase(blocks.begin() + index);
            blocks.push_back(block);
            return cycles;

        } else { // Cache miss!
            evict(lru, tag);
            index = (blocks.size() - 1);
            cycles = write_allocate(wa, index, tag);
            cycles = cycles + write_through(wt, index, tag);
            return cycles;
        }
    }

    uint32_t load(uint32_t tag, char*lru) {
        int index = find_block(tag);
        uint32_t cycles = 0;
        // Cache hit!
        if (index > 0) {
            Block block = blocks[index];
            blocks.erase(blocks.begin() + index);
            blocks.push_back(block);
            return 1;
        } else { // Cache miss! Have to get from main
            cycles = evict(lru, tag);
            cycles = cycles + 100;
            return cycles;
        }       
    }

    // decides to evict and returns index of the new one
    uint32_t evict(char* evict_option, uint32_t tag) {
        // If not full, just pushback
        uint32_t cycles = 0;
        if (blocks.size() != size) {
            Block block(tag);
            blocks.push_back(block);
            return cycles;

        } else { // size is full! have to evict
            if (strcmp(evict_option, "lru")) {
                if (blocks[0].is_dirty()) {
                    cycles = 100;
                }
                blocks.erase(blocks.begin());
                return cycles;
            } else {
                return 0;
            }
        }

    }

    uint32_t write_through(char* wt, int index, uint32_t tag) {
        // if write-through is true, then we have to access both the cache and main memory
        if (strcmp(wt, "write-through")) {
            blocks[index].set_tag(tag);
            return 101;
        } else {
            blocks[index].set_dirty();
            return 1;
        }
    }


/*
    uint32_t calc_total_cycles(char* wa, char* wt, char* lfu) {
        uint32_t write_alloc = write_allocate(wa,)
    }
*/

private:
    vector<Block> blocks;
    uint32_t size;

}; 


class Cache { // Has a sequence of set objects;

public:
    Cache() {}
    
    // Creates a cache with specifications
    Cache(char* arg1);

    Set get_set(int index) {
        return sets[index];
    }

    void set_set(int index, Set set) {
        sets[index] = set;
    }

    void create_set(Set set) {
        sets.push_back(set);
    }

    uint32_t num_sets() {
        return sets.size();
    }

    bool find_block(uint32_t tag, uint32_t index);

    uint32_t store(uint32_t tag, uint32_t index, char* wa, char* wt, char* lru) {
        return sets[index].store(tag, wa, wt, lru);

    }

    uint32_t load(uint32_t tag, uint32_t index, char* lru) {
        return sets[index].load(tag, lru);
    }

private:
    vector<Set> sets;

};


#endif