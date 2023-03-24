/**
 * License: BSL 1.0 
 */

#include "stdint.h"

class REY_AVL_NODE {
    int32_t R = -1;
    int32_t L = -1;
    int32_t P = -1;
    uint8_t H =  0;
/** int24_t is gonna be taken anyway >_< ....to Fill up 16Bytes....
    compiler automatically fills it & can't leave it be at 13Bytes haha (TwT) */
    uint64_t pUserData = NULL;
/** even if we used 'uint32_t' compile would still take 24Bytes for this STRUCT
    So i decided to give the user all that I could anyway.... >_< 
now this 
    pUserData is:-
      1. a pointer to Data, Every NODE should have some data associated with it right 
      2. may/can be a INTEGER/FLOAT, [if you are trynna do INTEGER/FLOAT sorting] */
};

/**
 * TEMPLATE:
 *  - @param ByteOffset: from pUserData \see REY_AVL_NODE
 *  - @param KeyType: typeof_WhatIsAt( pUserData + ByteOffset ) [note: treat pUserData as 'INTEGER' not 'POINTER']
 *  
 * @details
 *  - I believe in EXPLICIT IS BETTER THAN IMPLICIT
 *  - So I hope that you use REY_AVL<12, float> smth like this
 *  - I dont trust in TypeHiding or 'auto' features.
 *  - Please don't think of using 'auto' with my code, if you have the intention to understand these things that I do
 *  - + using 'auto' instead of struct/class kinda, makes 10x times harder for Newbies understand the code
 *  - 
 * 
 * INSERT:
 *  - @param pUserData: [uint64_t] \see REY_AVL_NODE
 * 
 * @note: I made an Older version, where, I used the idea of 'Inline-ing LAMBDA' instead of using 'ByteOffset'
 *        you can find it down in the ARCHIVE Section at EOF....
 */
template<uint32_t ByteOffset, typename KeyType>
class REY_AVL {
  public:
    void search(KeyType Key) {
        func<KeyType>({});

        REY_AVL_NODE FUCK;
        REY_AVL_NODE FUCK2 = std::move(FUCK);
    }
    void insert(uint64_t pUserData)
};

REY_AVL<12, float *> FUCK;