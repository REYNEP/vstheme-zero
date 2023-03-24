/**
 * License: BSL 1.0
 *          ©REYNEP
 * 
 * they prolly should have named it [AVYL] - AdelsonVelsky-YevgenyLandis
 *      - https://zbmath.org/authors/?q=ai:adelson-velsky.george-m
 *      - he made a cool CHESS Algorithms too ithink (just my intuition) 🤔 @see wikipedia
 *      - https://www.chessprogramming.org/Georgy_Adelson-Velsky
 *      - https://zbmath.org/authors/?q=ai:landis.evgenij-m
 * 
 * - I found this pretty nice
 *   https://medium.com/basecs/the-little-avl-tree-that-could-86a3cae410c7
 * - Tho Check out the MIT 6.006 OCW 2020, [nothing catches up to how awesome this actually is >_<]
 * 
 * StyleGuides I followed / breaked >_<
 *      - This is Prolly the first time I'm considering using 'reference' instead of 'pointers'
 *        I always used to think in terms of pointers.... haha
 *        https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable
 *      - Commenting Schemes.... For this file, there are no comments but lots of DOCs
 *        https://www.youtube.com/watch?v=Bf7vDBBOBUA
 */

#include "stdint.h"
#ifdef _MSC_VER
    #define _REY_NoInline_ __declspec(noinline)
    #define _REY_Inline_   __forceinline
#else
    #define _REY_NoInline_ __attribute__ ((noinline))
    #define _REY_Inline_   __attribute__((always_inline))
#endif

struct VelskyLandisNode {
    int32_t R;
    int32_t L;
    int32_t P;
    uint8_t H;
/** int24_t is gonna be taken anyway >_< ....to Fill up 16Bytes....
    compiler automatically fills it & can't leave it be at 13Bytes haha (TwT) 
    
    @todo We can pack this into 16Bytes.... instead of 'Height' we'd need to store the 'Skew' {-1, 0, 1} 
          the first 2Bytes of H would be used for that, string SKEW value 
          that leaves us with 30Bytes of data for storing the key */

    uint64_t pUserData;
/** fn.
      even if we used 'uint32_t' compile would still take 24Bytes for this STRUCT
      So i decided to give the user all that I could anyway.... >_< 
now this
    pUserData is:-
      1. a pointer to Data, Every NODE should have some data associated with it right?
      2. may/can be a INTEGER/FLOAT, [if you are trynna do INTEGER/FLOAT sorting] */

    /** We use 'new' keyword later on, on this struct, so default values are in this function */
    inline void DefaultValues(void) {
            R = L = P = -1;
        H = pUserData =  0;
    }
};



/**
 * @todo
 *  1. Don't Include anything .... >_< [JustForFun] 
 */
class VelskyLandis {
  public:
    public:
    typedef float(*Func_GetKey)(uint64_t pUserData);
    inline int32_t& L(int32_t node) {return _memory_[node].L;}
    inline int32_t& R(int32_t node) {return _memory_[node].R;}
    inline int32_t& P(int32_t node) {return _memory_[node].P;}
    inline uint8_t& H(int32_t node) {return _memory_[node].H;}
    inline uint64_t& pUserData(int32_t node) {return _memory_[node].pUserData;}

    /** @return int32_t: [index to new node] assumed that, you won't discard it. must use it */
    inline int32_t newNode(uint64_t pUserData) {
        _memory_[neXt].L = -1;
        _memory_[neXt].R = -1;
        _memory_[neXt].H =  0;
        _memory_[neXt].pUserData = pUserData;
        return neXt++;
    }

    _REY_NoInline_ bool Insert_FixUp(VelskyLandisNode &Parent) {
        return false;
    }

    inline VelskyLandisNode *pNode(int32_t node){return _memory_ + node;}

  public:
    int32_t        m_Root = -1;
    VelskyLandisNode *_memory_;     /* Set in CONstructor */
    int32_t          neXt =  0;     /*   memory tracker   */   

    /** @param     N: N*AVL_Node is Malloc-ed.... [Not like 'building a Tree' tho] */
    VelskyLandis(int32_t N = 16) {_memory_ = new VelskyLandisNode[N];}
   ~VelskyLandis() {}

    






    /** 
     * @param Key: [Float]
     * @param paramFunc_GetKey: [expected LAMBDA] takes a Parameter.... <VelskyLandisNode.pUserData>
     * 
     * @return TrySearch, [if not Found] returns bottom-most node, where the search reached
     * 
     *
     * @note This is for like, Internal uses.... You shouldn't make objects of AVL_Intern class....
     * @todo see is I can make this a PureVirtual/Abstract Class
     */
    constexpr int32_t _search(float Key, Func_GetKey F1) {
        int32_t x = m_Root;
        int32_t y = x;

        while(x) {
            VelskyLandisNode &m = _memory_[x];  /** I guess this isn't gonna act better than Pointer-Dereferencing */

                     y = x;
            float    k = F1(m.pUserData);
            if      (k == Key) return x;
            else if (k  < Key) x = m.R;
            else               x = m.L;
        }
        return y;
    }

    /** @return param:node if no furthur minimum */
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
     * @param pUserData: we calculate the 'key', using the 'F1' [Func_GerKey] that you gave
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
    constexpr bool _insert(uint64_t pUserData, Func_GetKey F1) {
        float Key = F1(pUserData);
        int32_t x = m_Root;
        int32_t NewNode = this->newNode(pUserData, m_Root);

        for(;;) {
            VelskyLandisNode &m = _memory_ [x];
            float             k = F1(m.pUserData);

            if (k < Key) {
                if (m.R > -1) x = m.R;
                else {
                    m.R = NewNode;
                    return Insert_FixUp(m);
                }
            }
            else if (k > Key) {
                if (m.L > -1) x = m.L;
                else {
                    m.L = NewNode;
                    return Insert_FixUp(m);
                }
            }
            else /** (k == Key) */ {  
                if (m.L > -1) {
                    L(x) = NewNode;
                    Insert_FixUp(m);
                }
                else {
                    x = Maximum(m.L);
                    R(x) = NewNode;
                    Insert_FixUp(m);
                }

                return true;
            }
        }
    }
};