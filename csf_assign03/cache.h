#ifndef CACHE_H 
#define CACHE_H

#include <vector>

using std::vector;

class Block {
public:
    Block() {}

    Block(uint32_t tag) : tag(tag){}

    Block(uint32_t tag, int data) : tag(tag), data(data) {}

    uint32_t get_tag() {
        return this->tag;
    }

    void set_tag(uint32_t tag) {
        this->tag = tag;
    }

    void set_dirty() {
        this->dirty = 1;
    }

private:
    int data;
    uint32_t tag;
    bool dirty;
    

};

class Set {
public:
    Set() {}

    Block get_block(int index) {
        return blocks[index];
    }

    void set_block(int index, Block block) {
        blocks[index] = block;
    }

    void create_block(Block block) {
        blocks.push_back(block);
    }

    uint32_t num_blocks() {
        return blocks.size();
    }

    bool find_block(uint32_t tag) {
        uint32_t num_blocks = blocks.size();
        for (int i = 0; i < num_blocks; i++) {
            if (blocks[i].get_tag() == tag) {
                return 1;
            }
        }
        return 0;
    }

private:
    vector<Block> blocks;

};

class Cache { // Has a sequence of set objects; is a sequence of sets

public:
    Cache() {}

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


    bool find_block(uint32_t tag, uint32_t index) {
        // If a tag at a specific set, or index, is found, then it was hit
        if (sets[index].find_block(tag) == 1) {
            return 1;
        } else { // if no tag was found, it was a miss. Depending on 

        }
        
        return 0;
    }

private:
    vector<Set> sets;

};


#endif