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
    
    /*
     * Non-default constructor
     * 
     * Parameters: 
     *   arg1 - char* of the number of sets in the cache
     *   arg2 - char* of the number of blocks in each set
     *   arg3 - char* of the number of bytes in a block
     * 
     * Returns: N/A
     */
    Cache(char* arg1, char* arg2, char* arg3);

    /*
     * Getter for the cache's set
     * 
     * Parameters: 
     *   index - the the index of the set
     * 
     * Returns:
     *   the set object at the specified index
     */
    Set get_set(int index) { return sets[index]; }

    /*
     * Returns number of sets in the cache
     * 
     * Parameters: None  
     * 
     * Returns:
     *   the number of sets in the cache
     */
    uint32_t num_sets() { return sets.size(); }

    /*
     * Finds a specific block in the cache
     * 
     * Parameters: 
     *   tag - the tag of the block to be searched for
     *   index - the index of the set to which the block belongs  
     * 
     * Returns:
     *   1 if found; 0 if not found
     */
    bool find_block(uint32_t tag, uint32_t index);

    /*
     * Performs a store operation for the cache
     * 
     * Parameters: 
     *   tag - the tag of the block to be stored
     *   index - the index of the set to which the block should belong
     *   wa - char* indicating if the store is "write-allocate" or "no-write-allocate"
     *   wt - char* indicating if the store is "write-through" or "write-back"
     *   lru - char* indicating if the cache is "least recently used" or "first-in-first-out"
     * 
     * Returns:
     *   the number of cycles it took to complete the store operation
     */
    uint32_t store(uint32_t tag, uint32_t index, char* wa, char* wt, char* lru) {
        return sets[index].store(tag, wa, wt, lru);
    }

    /*
     * Performs a load operation for the cache
     * 
     * Parameters: 
     *   tag - the tag of the block to be loaded
     *   index - the index of the set to which the block should belong
     *   lru - char* indicating if the cache is "least recently used" or "first-in-first-out"
     * 
     * Returns:
     *   the number of cycles it took to complete the load operation
     */
    uint32_t load(uint32_t tag, uint32_t index, char* lru) {
        return sets[index].load(tag, lru);
    }

    /*
     * Stores all unstored, dirty cache blocks back to main memory
     * 
     * Parameters: None
     * 
     * Returns:
     *   the number of cycles it took to store all of the dirty blocks in cache to main memory
     */
    uint32_t store_all_dirty();

private:
    vector<Set> sets;
    uint32_t size;
};


#endif