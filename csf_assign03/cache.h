#ifndef CACHE_H 
#define CACHE_H

#include <vector>

using std::vector;

class Block {
public:
    Block(int block_bytes) : num_bytes(block_bytes) {}

private:
    int num_bytes;

};

class Set {
public:
    Set() {}

private:
    vector<Block> blocks;

};

class Cache { // Has a sequence of set objects; is a sequence of sets

public:
    Cache() {}

    Set getSet(int index) {
        return sets[index];
    }

    void setSet(int index, Set set) {
        sets[index] = set;
    }

private:
    vector<Set> sets;

};


#endif