#include "bitset.h"

// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size) {
    struct bitset * result;

    // allocate space for the stack header
    result = malloc(sizeof(struct bitset));
    result->universe_size = size;
    result->size_in_words = ceil(size/(sizeof(uint64_t)*8));
    result->bits = malloc(sizeof(uint64_t)*result->size_in_words);

    for(int i = 0; i < result->size_in_words; i++)
    {
        result->bits[i] = 0;
    }

    return result;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this) {
    return this->universe_size;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this){
    int n = this->size_in_words;
    uint64_t * words = this->bits;
    int count = 0;

    for(int i = 0; i < n; i++)
    {
        uint64_t mask = 1;
        uint64_t cur = words[i];
        for(int j = 0; j<sizeof(uint64_t)*8;j++)
        {
            if((cur&mask) != 0)
            {
                count++;
            }
            mask = mask<<1;
        }
    }

    return count;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item){
    int index = item/(sizeof(uint64_t)*8);
    if(index<this->size_in_words)
    {
        uint64_t elem = this->bits[index];
        int shift = item - index*sizeof(uint64_t)*8;
        return ((elem&(1ul<<shift)) !=0);
    }
    else
    {
        return 0;
    }
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
void bitset_add(struct bitset * this, int item) {
    int index = item/(sizeof(uint64_t)*8);
    if(index<this->size_in_words)
    {
        int shift = item - index*sizeof(uint64_t)*8;
        this->bits[index] = (this->bits[index]) | (1ul<<shift);
    }
}

// remove an item with number 'item' from the set
void bitset_remove(struct bitset * this, int item) {
    int index = item/(sizeof(uint64_t)*8);
    if(index<this->size_in_words)
    {
        int shift = item - index*sizeof(uint64_t)*8;
        uint64_t mask = ~(1ul<<shift);
        this->bits[index] = (this->bits[index] & mask);
    }
}

// place the union of src1 and src2 into dest;
// all of src1, src2, and dest must have the same size universe
void bitset_union(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
    if(dest != NULL && src1 != NULL && src2 != NULL && src1->universe_size == src2 ->universe_size) {
        int universe = dest->universe_size;

        for (int i = 0; i < universe; i++) {
            if (bitset_lookup(src1, i) || bitset_lookup(src2, i)) {
                bitset_add(dest,i);
            }
        }
    }
    else
    {
        dest = NULL;
    }
}

// place the intersection of src1 and src2 into dest
// all of src1, src2, and dest must have the same size universe
void bitset_intersect(struct bitset * dest, struct bitset * src1,
    struct bitset * src2) {
    if(dest != NULL && src1 != NULL && src2 != NULL && src1->universe_size == src2 ->universe_size) {
        int universe = dest->universe_size;

        for (int i = 0; i < universe; i++) {
            if (bitset_lookup(src1, i) && bitset_lookup(src2, i)) {
                bitset_add(dest, i);
            }
        }
    }
    else
    {
        dest = NULL;
    }



}
