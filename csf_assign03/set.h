/*
 * Declaration of class functions for the cache object
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#ifndef SET_H 
#define SET_H

#include "block.h"
#include <vector>
#include <cstdint>

using std::vector;

#define MAIN_MEM_CYCLES 100
#define CACHE_CYCLES 1

class Set {
public:

    // Non-default constructor
    Set(uint32_t set_size, uint32_t bpb) {
        this->size = set_size;
        this->block_size = bpb;
    }

    /*
     * Getter for a set's block
     * 
     * Parameters: 
     *   index - the the index of the block
     * 
     * Returns:
     *   Block at specified index
     */
    Block get_block(int index) { return blocks[index]; }

    /*
     * Setter for a set's block
     * 
     * Parameters: 
     *   index - the the index of the block
     *   block - the block to replace the current block with
     * 
     * Returns: None
     */
    void set_block(int index, Block block) { blocks[index] = block; }

    /*
     * Gets the number of blocks per set
     * 
     * Parameters: None
     * 
     * Returns: 
     *    Number of blocks in the set as a uint32_t
     */
    uint32_t num_blocks() { return blocks.size(); }

    /*
     * Checks to see if the block with a specific tag is in the set
     * 
     * Parameters: 
     *    Tag - the tag of a specified block
     * 
     * Returns: 
     *    The index of the block in the set. Returns -1 if block wasn't found in the set.
     */
    int find_block(uint32_t tag);

    /*
     * Decides what to do in case of a store and miss.
     * "write-allocate" means a store first loads the address from main memory,
     *                  prior to a store: therefore, will access main mem
     * "no-write-allocate" means a store will directly store to main memory
     *                  and NOT MODIFY CACHE AT ALL (if wt and no-w-alloc, then just
     *                  modifies main memory)
     * Parameters: 
     *    wa = string that's either "write-allocate" or "no-write-allocate"
     *    tag = the tag of the block in main memory
     * Returns: 
     *    The number of cycles from accessing memory (cache or main)
     */
    uint32_t write_allocate(char* wa, uint32_t tag);
    
    /*
     * Decides what to do in case of a store (hit or miss).
     * "write-through" means a store will write to both main memory and the cache
     * "write-back"    means a store will mark the cache as dirty (cache access)
     *                 and have it write to the main memory when evicted (at the end of
     *                 the program, all dirty blocks will have to be written to main memory)
     * 
     * Parameters: 
     *    wt = string that's either "write-through" or "write-back"
     *    tag = the address of the block to be written to
     *    
     * Returns: 
     *    The number of cycles from accessing memory (cache or main)
     */
    uint32_t write_through(char* wt, uint32_t tag);

    /*
     * Decides what to do in case of a store (hit or miss)
     * 
     * Parameters: 
     *    tag = the tag of the object to be stored
     *    wa = either "write-allocate" or "no-write-allocate"
     *    wt = either "write-through" or "write-back"
     *    lru = either "lru" (least-recently-used) or "fifo" (first-in first-out)
     * 
     * Returns: 
     *    The number of cycles total from a store event
     */
    uint32_t store(uint32_t tag, char* wa, char* wt, char* lru);

    /*
     * Decides what to do in case of a load (hit or miss)
     * 
     * Parameters: 
     *    tag = the tag of the object to be stored
     *    lru = either "lru" (least-recently-used) or "fifo" (first-in first-out)
     * 
     * Returns: 
     *    The number of cycles total from a store event
     */
    uint32_t load(uint32_t tag, char*lru);

    /*
     * Rearranges the vector of sets s.t. the head will be the least-recently used
     * 
     * Parameters: 
     *    index = the location of the recently used block
     * 
     * Returns: None
     */
    void lru_rearrange(int index);

    /*
     * Decides whether to evicts a block from the cache or not
     * 
     * Parameters: 
     *    tag = the tag of the block to be stored in cache
     * 
     * Returns: None
     */
    uint32_t update_cache(uint32_t tag);

    /*
     * Extra functionality: stores all dirty blocks too
     * 
     * Parameters: None
     * 
     * Returns: None
     */
    uint32_t store_all_dirty();

private:
    vector<Block> blocks;
    uint32_t size;
    uint32_t block_size;
}; 


#endif