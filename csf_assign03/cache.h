#ifndef CACHE_H 
#define CACHE_H

#include <vector>

using std::vector;

class Cache { // Has a sequence of set objects; is a sequence of sets

public:
    Cache() {}

    Set getSet(int index) {
        return sets[index];
    }

    void setSet(int index) {
        return sets[index];
    }

private:
    vector<Set> sets;

}


class Set {
public:
    Set(int num_blocks, int block_bytes) {

    }

private:
    vector<Block> blocks;

}

class Block {
public:
    Block(int block_bytes) : num_bytes(block_bytes) {}

private:
    int num_bytes;

}

#endif