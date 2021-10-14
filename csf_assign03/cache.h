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

    // decides what to do in event of store (hit or miss)
    uint32_t write_through(char* wt, int index, uint32_t tag);

    // Handles all stores
    uint32_t store(uint32_t tag, char* wa, char* wt, char* lru);

    uint32_t load(uint32_t tag, char*lru);

    void lru_rearrange(int index);

    // decides to evict or not; 
    uint32_t evict(uint32_t tag);

private:
    vector<Block> blocks;
    uint32_t size;

}; 


class Cache { // Has a sequence of set objects;

public:
    Cache() {}
    
    // Creates a cache with specifications
    Cache(char* arg1);

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

private:
    vector<Set> sets;
};


#endif