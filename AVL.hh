/**
 * Licensed under Boost Software License - Version 1.0
 * @copyright REYNEP
 * 
 * Tree-AVL: Adelson-Velsky & Landis
 * 
 * @todo: Template if people/I need
 */
#include "stdint.h"
#include "stdlib.h"

struct AVL_Node {
    int32_t L = -1;
    int32_t R = -1;
    int32_t P = -1;
    int32_t H =  0;
    void *userD = nullptr;
};

class AVL_Intern {
  public:
    typedef float(*Func_GetKey)(void *userD);
    inline int32_t& L(int32_t node) {return _memory_[node].L;}
    inline int32_t& R(int32_t node) {return _memory_[node].R;}
    inline int32_t& P(int32_t node) {return _memory_[node].P;}
    inline int32_t& H(int32_t node) {return _memory_[node].H;}
    inline void*  & userD(int32_t node)  {return _memory_[node].userD;}

    /** @return int32_t: [index to new node] assumed that, you won't discard it. must use it */
    inline int32_t newNode(void *userD) {
        _memory_[neXt].L = -1;
        _memory_[neXt].R = -1;
        _memory_[neXt].H =  0;
        _memory_[neXt].userD = userD;
        return neXt++;
    }

    inline AVL_Node  *pNode(int32_t node){return _memory_ + node;}

  public:
    int32_t m_Root = -1;
    AVL_Node  *_memory_;     /* Set in CONstructor */
    int32_t   neXt =  0;     /*   memory tracker   */   

    /** @param     N: N*AVL_Node is Malloc-ed.... [Not like 'building a Tree' tho] */
    AVL_Intern(int32_t N = 16) {_memory_ = static_cast<AVL_Node *>( malloc(N * sizeof(AVL_Node)) );}

    
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

    /** 
     * @param Key: Okay
     * @param paramFunc_GetKey: [expected LAMBDA] takes a Parameter.... <BST_Node_IMPL.userD>
     * @return : nullptr / userData if found
     * 
     * @note This is for like, Internal uses.... You shouldn't make objects of AVL_Intern class....
     * @todo see is I can make this a PureVirtual/Abstract Class
     */
    constexpr int32_t _search(float Key, Func_GetKey F1) {
        int32_t x = m_Root;
        int32_t y = x;

        while(x) {
            AVL_Node &m = _memory_[x];  /** I guess this isn't gonna act better than Pointer-Dereferencing */

                     y = x;
            float    k = F1(m.userD);
            if      (k == Key) return x;
            else if (k  < Key) x = m.R;
            else               x = m.L;
        }
        return y;
    }

    /** 
     * @param userD: we calculate the 'key', using the 'F1' [Func_GerKey] that you gave
     * 
     * @return TRUE: If we got a match
     * 
     * @details [if (k == Key)]
     *  - Assuming that we got a match in the middle of the Tree smWhere
     *    - we could go left/right.
     *    - But because '_search()' only looks at 'RIGHT if BIGGER' only
     *    - we go left [from the Key that matched]
     *    - then we find Maximum(left)
     * 
     * @this Shit doesn't get inlined.... but we need it to.... for F1
     */
    constexpr bool Insert(void *userD, Func_GetKey F1) {
        float Key = F1(userD);
        int32_t x = m_Root;

        for(;;) {
            //AVL_Node *m = _memory_ +x;
            float     k = F1(_memory_[x].userD);

            if (k < Key) {
                if (!R(x)) {
                    R(x) = this->newNode(userD, x);
                    return Insert_FixUp(_memory_ + x, 1);
                }
                x = R(x);
            }
            else if (k > Key) {
                if (!L(x)) {
                    L(x) = this->newNode(userD, x);
                    return Insert_FixUp(_memory_ + x, 0);
                }
                x = L(x);
            }
            else {  /** (k == Key) */
                if (L(x)) {
                    x = Maximum(L(x));
                    R(x) = this->newNode(userD, x);
                    Insert_FixUp(_memory_ + x, 1);
                }
                else {
                    L(x) = this->newNode(userD, x);
                    Insert_FixUp(_memory_ + x, 0);
                }

                return true;
            }
        }
    }
};