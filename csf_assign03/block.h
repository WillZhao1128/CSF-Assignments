/*
 * Declaration (and all simple implementations) of class functions for the blcok object
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>


class Block {
public:
    Block() {}

    // Non-default constructor
    Block(uint32_t tag_add){
        this->tag = tag_add;
        this->dirty = 0;
    }

    // Copy constructor
    Block(const Block& b) : tag(b.tag), dirty(b.dirty) {}

    // """"= Operator overload
    Block& operator=(const Block& b) {
        tag = b.tag;
        dirty = b.dirty;
        return *this;
    }

    /*
     * Getter for a block's tag
     * 
     * Parameters: None
     * 
     * Returns:
     *   Block's tag
     */
    uint32_t get_tag() { return this->tag; }

    // SHOULD WE EVER BE ABLE TO EDIT A BLOCK'S TAG???
    /*
     * Setter for a block's tag
     * 
     * Parameters:
     *   tag - the tag to replace the current block's tag
     * 
     * Returns: None
     */
    void set_tag(uint32_t tag) {
        this->tag = tag;
    }

    /*
     * Labels the block as dirty; only for use with "write-back"
     * 
     * Parameters: None
     * 
     * Returns: None
     */
    void set_dirty() { this->dirty = 1; }

   /*
    * Checks if a block is dirty or not; only for use with "write-back"
    *
    * Parameters: None
    * 
    * Returns: 
    *    1 if the block is dirty; 0 if the block is not dirty
    */
    bool is_dirty() {
        return dirty;
    }

private:
    uint32_t tag;
    bool dirty;
};

#endif