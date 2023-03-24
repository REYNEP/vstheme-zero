#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

struct BST_Node_IMPL {
    int32_t L = -1; /* Left */
    int32_t R = -1; /* Right */
    int32_t P = -1; /* Parent */
    void *userD = nullptr;
};

/*
REY_BST_Intern::Func_GetKey WHATEVER = [](void *userD)->float {
    return *((float *)userD);
};
*/

/**
 * @brief 
 *  Treat this like a base class.... from where you make other kinds of BinarySearchTrees (BSTs)
 *  Thats why we didn't implement any INSERT/DELETE
 * 
 * .... .... .... Key/Values can be of diff. types, so we rather switched to LAMBDA Funcs, that you should provide.
 * 
 * @details
 * A List of what's implemented:
 *  - SEARCH / FIND
 *  - MINIMUM / MAXIMUM
 *  - PREV / NEXT
 * 
 * @todo we'd meed to templatize Search anyway.... to do that, class might need to be.... we could make BST_Node_IMPL templatized too!
 * 
 * @note see end of page for extended Details. [Includes L3 Cache misses Optimzation INFO]
 */
class REY_BST_Intern {
  public:
    typedef float(*Func_GetKey)(void *userD);
    inline int32_t &L(int32_t node) {return _memory_[node].L;}
    inline int32_t &R(int32_t node) {return _memory_[node].R;}
    inline int32_t &P(int32_t node) {return _memory_[node].P;}
    inline void* &userD(int32_t node) {return _memory_[node].userD;}

  public:
    int32_t Root = -1;
    BST_Node_IMPL *_memory_ = nullptr;

    /**
     * @param     N: We would just be occupying that amount of memory.... Not like 'building a Tree'
     * @param userD: The data stored at the Root Member....
     */
    REY_BST_Intern(int32_t N = 16, void *userD = nullptr) {
        _memory_ = static_cast<BST_Node_IMPL *>( malloc(N * sizeof(BST_Node_IMPL)) );
        Root = 0;
        _memory_[Root].userD = userD;
    }

    /** 
     * @param K: Key 
     * @param paramFunc_GetKey: takes a Parameter.... <BST_Node_IMPL.userD>
     * @return : nullptr / userData if found 
     */
    constexpr void *Search(float Key, Func_GetKey F1) {
        int32_t x = Root;
        while(x != -1) {
            BST_Node_IMPL &m = _memory_[x];  /** I guess this isn't gonna act better than Pointer-Dereferencing */

            float    k = F1(m.userD);
            if      (k == Key) return m.userD;
            else if (k  < Key) x = m.R;
            else               x = m.L;
        }
        return nullptr;
    }

    /** @return node if no furthur minimum */
    inline int32_t Minimum(int32_t node) {
        while (_memory_[node].L)
            node = _memory_[node].L;

        return node;
    }
    /** Compiler will Optimize it to the Most even at O2 */
    inline int32_t Maximum(int32_t node) {
        while (_memory_[node].R)
            node = _memory_[node].R;

        return node;
    }

    /** @return: Same Element, if No Previous element */
    int32_t Prev(int32_t node) {
        if (_memory_[node].L)
            return Maximum(_memory_[node].L);
        else {
            int32_t x = node;

            while (P(x)) {
                // if node == node.parent.right; -> we already found our previous element
                if (x == R(P(x)))return P(x);
                else x = P(x);
                // else lets go furthur upwards  -> we wanna check all the ancestors....
            }

            return node;
        }
    }
    /** heck this code is even more OPTIMIZED.... aha */
    int32_t Next(int32_t node) {
        if (_memory_[node].R)
            return Minimum(_memory_[node].R);
        else {
            int32_t x = node;

            while (P(x)) {
                if (x == L(P(x))) return P(x);
                else x = P(x);
            }

            return node;
        }
    }

    /**
     * @param L2R: Levitating Order (based on however tree is) 
     * @param EX1: Non-Recursive Algorithm
     * 
     * Assembly Analysis:
     *  https://www.section.io/engineering-education/assembly-part-2/
     *  https://home.adelphi.edu/~siegfried/cs174/174l4.pdf
     *  https://stackoverflow.com/questions/72492532/in-which-register-does-the-scanf-function-store-input-values
     *  https://stackoverflow.com/questions/56565510/x86-what-does-movsxd-rdx-edx-instruction-mean
     *  https://stackoverflow.com/questions/24206898/pointer-memory-arithmetic-in-assembly
     *  https://stackoverflow.com/questions/20510132/what-exactly-does-malloc-do-in-assembly
     *  https://stackoverflow.com/questions/43769467/x86-assembly-pointers
     *  https://stackoverflow.com/questions/38685058/assembly-register-arithmetic
     */
    void TraverseEX1(int32_t node, Func_GetKey F1, bool L2R = true) {
        if (L2R) {
            int32_t x = Minimum(node);
            printf("%f", F1(_memory_[x].userD));

            while (Next(x) != x) {
                x = Next(x);
                printf("%f", F1(_memory_[x].userD));
            }
        }
        else {
            int32_t x = Maximum(node);
            printf("%f", F1(_memory_[x].userD));

            while (Prev(x) != x) {
                x = Prev(x);
                printf("%f", F1(_memory_[x].userD));
            }
        }
    }

    Func_GetKey _F1 = nullptr;
    /**  @param _F1: [member variable] this must be set before */
    void TraverseEX0(int32_t node) {
        if (L(node))
            TraverseEX0(L(node));

        printf("%f", this->_F1(userD(node)));

        if (R(node))
            TraverseEX0(R(node));
    }
    /** 
     * Note that Code of the Recursive Algorithm [EX0] may be 2x or more bigger than EX1 
     * 
     * @note on Recursion: 
     *  https://stackoverflow.com/questions/30208196/maximum-recursive-function-calls-in-c-c-before-stack-is-full-and-gives-a-segme
     */
    void Traverse(int32_t node, Func_GetKey F1) {_F1 = F1; TraverseEX0(node);}
};



/**
 * 
 */
class REY_AVLTree {
    public:
    
    // BUILD? 
};

class REYs_OwnFkin_ShitTree {
    public:
    
};


/**
 * @note REY_BST_Intern: Extended DOCS
 * 
 * All of these Functions take 'LAMBDA' functions as params
 *      So you are supposed to pass in 'scheme on how to get/retrieve key' from <BST_Node_IMPL.userD>
 * In-Depth Beginners guide to LAMBDA function passing:
 *      https://www.youtube.com/watch?v=3jCOwajNch0
 * The-Cherno - Lambdas
 *      https://www.youtube.com/watch?v=mWgmBBz0y8c
 * 
 * 
 * ---------------------------------
 * Optimization #1
 *   Inline-ing Lambda's CONTENT....
 *      https://stackoverflow.com/questions/63530233/c-inline-lambda-argument/63530295#63530295
 *          - 2nd Answer
 *      - the Trick: Use <constexpr>   [works with template too]
 *          - https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170
 * ---------------------------------
 * 
 *  * Compact Nodes for lesser L3 Cache misses....
 *      - sizeof(BST_Node_IMPL) - 6*4 Bytes - 24B - 24KB for 1024Nodes, 24MB for 2²⁰ ....
 *  - 2/3x sized      [🤔 RedBlack/B-Tree might get this to 16MB for 2²⁰]
 *  - @todo we could do like, a Memory Sorting thingy, if someone wants smday, e.g. CACHE Misses could be improved, gotta make benchmark to make sure
 *      - IDEA: it would have to be like the first 2^19 elements [from top to bottom], I DONT 'LITERALLY MEAN SORTED' in a sense....
 *              all I mean is, first 19 level of all stuff can be in first 24MB.... you know.... first 18 level in 12MB....
 *              I Am gonna draw some stuff up, make a gist.... IG
 *      - DETE: 
 *          if you got 2^31 elements.... its likely that you are gonna have like 31 CacheMiss-es at most.... (🤔 maybe 31/2 ~ 15.5 at avg)
 *          Well anyway.... by what I said above.... it is gonna be guranteed that you won't have more than 1 cache miss, for 2^38 elements
 *          1 CacheMiss refers to:
 *              The need to Bring up Memory from RAM to L3Cache
 * 
 * - for Terms, refer to: [MIT 6.006 2020 OCW](https://www.youtube.com/playlist?list=PLUl4u3cNGP63EdVPNLG3ToM6LaEUuStEY)
 */






/** The code that I wrote at first.... >_< I used to fear Pointer optimizations a lot.... but it seems that COMPILERS Areeeee actually really fkin smart 

#include "stdint.h"
#include "stdlib.h"

struct BST_Node_IMPL {
    int32_t L = -1;
    int32_t R = -1;
    int32_t P = -1;
    void *userD = nullptr;
};

class REY_BST_Intern {
  public:
    typedef float(*Func_GetKey) (void *userD);

  public:
    int32_t Root = -1;
    BST_Node_IMPL *_memory_ = nullptr;

    REY_BST_Intern(int32_t N = 16, void *userD = nullptr) {
        _memory_ = static_cast<BST_Node_IMPL *>( malloc(N * sizeof(BST_Node_IMPL)) );
        Root = 0;
        _memory_[Root].userD = userD;
    }

    constexpr void *Search(float Key, Func_GetKey F1) {
        int32_t x = Root;
        while(x != -1) {
            BST_Node_IMPL &m = _memory_[x];

            float    k = F1(m.userD);
            if      (k == Key) return m.userD;
            else if (k  < Key) x = m.R;
            else               x = m.L;
        }
        return nullptr;
    }

    inline int32_t Minimum(int32_t node) {
        int32_t x = _memory_[node].L;

        while(x != -1) {
            node = x;
            x = _memory_[x].L;
        }

        return node;
    }
    inline int32_t Maximum(int32_t node) {
        int32_t x = _memory_[node].R;

        while(x != -1) {
            node = x;
            x = _memory_[x].R;
        }

        return node;
    }

    int32_t Prev(int32_t node) {
        int32_t x = _memory_[node].L;

        if (x != -1)
            return Maximum(x);
        else {
            int32_t p = _memory_[node].P;

            while(p != -1) {
                if (x == _memory_[p].R)
                    return p;

                x = p;
                p = _memory_[x].P;
            }
            
            return node;
        }
    }
    int32_t Next(int32_t node) {
        int32_t x = _memory_[node].R;

        if (x != -1)
            return Minimum(x);
        else {
            int32_t p = _memory_[node].P;

            while(p != -1) {
                if (x == _memory_[p].L)
                    return p;

                x = p;
                p = _memory_[x].P;
            }
            
            return node;
        }
    }
};

REY_BST_Intern::Func_GetKey WHATEVER = [](void *userD)->float {
    return *((float *)userD);
};

#include <iostream>

int main(void) {
    REY_BST_Intern whatever(16, nullptr);

    int x;
    scanf("%d",&x);
    printf("%i", whatever.Prev(x));

    return 0;
}

*/




        constexpr bool Insert(void *userD, Func_GetKey F1) {
        float Key = F1(userD);
        int32_t x = m_Root;

        for(;;) {
            AVL_Node *m = _memory_ +x;
            float     k = F1(m->userD);

            if (k < Key) {
                if (m->R)
                    x = m->R;
                else {
                    R(x) = this->newNode(userD, x);
                    return Insert_FixUp(R(x));
                }
            }
            else if (k > Key) {
                if (m->L)
                    x = m->L;
                else {
                    L(x) = this->newNode(userD, x);
                    return Insert_FixUp(L(x));
                }
            }
            else {  /** (k == Key) */
                if (m->L) {
                    x = Maximum(m->L);
                    R(x) = this->newNode(userD, x);
                    Insert_FixUp(R(x));
                }
                else {
                    L(x) = this->newNode(userD, x);
                    Insert_FixUp(L(x));
                }

                return true;
            }
        }
    }