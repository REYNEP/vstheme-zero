#ifndef K391_INCLUDE_GUARD
#define K391_INCLUDE_GUARD
#include "amVK_INK.hh"
#include "amVK_WI.hh"

/** in Vulkan TOP-LEFT is (0, 0) */
typedef enum {
  amVK_LookingGlass_SplitTOP = 0,
  amVK_LookingGlass_SplitDOWN = 1,
  amVK_LookingGlass_SplitLEFT = 2,
  amVK_LookingGlass_SplitRIGHT = 4
} amVK_LookingGlass_Split;
typedef uint32_t amVK_TLookingGlass_Split;

/** HAHA */
struct FreeCityPlayer;
struct FreeCityNPC {
    //uint32_t _inUse_ = false;
   ///uint32_t  player = 0;       /** PLAYER-INDEX: 2^32 is max... also see FreeCity::N */
    //FreeCityNPC *NPC = nullptr;

    /** we can just do this */
    int32_t  player_i;             /** if negative, means not in use */
    uint32_t next_NPC_i;             /** we dont gotta set that to -1 either... cz we got a NPC_neXt variable... */
                                /** only indices smaller than NPC_neXt is/was used.... bigger indices have never been used before... no doubt */
                                /** on the contrary.... if  NPC_neXt  is of type UINT32_T ..... number of players can't be more than   NPC_neXt / 5 */
                                /** so    'int32_t player' suffices */
                                // SHOULD BE INDEX + 1
    /** 
     * now the thing is even if we do thi 
     * 
     * 1. if we had FreeCityNPC *NPC;    assembly would be like
     *      mov   NPC1.NPC    regx
     * 
     * 2. if we ave what we got now
     *      add   NPC1.next_NPC_i  regx       stores it into  'regx'
     * 
     * to assembly addition & moving variable is just 1 operation.... same thing
     */
};



struct FreeCityPlayer {
    VkViewport m_Viewport;      /** Not initialized / values are undefined */
    uint32_t _W, _A, _S, _D;    /**                ↑ ← ↓ →                 */

    /**
     * This is prolly the Most Optimized way to do this.... 
     *      You know not having the parameters in CONSTRUCTOR
     * 
     * Instead of UPDATE() you should just set the values in m_Viewport... anyway
     *      If anyone is like that ain't the standard way... we can add inline update functions with only DUOs of params
     */
    inline void Initialize(float x, float y, float width, float height, float minDepth = 0, float maxDepth = 1) {
        m_Viewport = {x, y, width, height, minDepth, maxDepth};
    }
    inline void Initialize(amVK_WI_MK2 *amVK_WI) {
        m_Viewport = {0, 0, static_cast<float>(amVK_WI->extent.width), static_cast<float>(amVK_WI->extent.height), 0, 1};
    }
    inline void NoConnectionSure(void) {
        _W = 0; _A = 0; _S = 0; _D = 0;
    }

    inline void Update(float x, float y, float width, float height) {
        m_Viewport.x = x;
        m_Viewport.y = y;
        m_Viewport.width = width;
        m_Viewport.height = height;
    }
    inline void Update(float width, float height) {
        m_Viewport.width = width;
        m_Viewport.height = height;
    }

    FreeCityPlayer() {}
   ~FreeCityPlayer() {}



    /**
     *  in Vulkan TOP-LEFT is (0, 0)
     * This just sets the height & width.... maintanence is in FreeCity
     * 
     * \param Direction: if H0.6 W0.4   SplitTOP: create new [with H0.4] above THIS 
     * \param width_perc: Percentage Width of THIS one
     * \param height_perc: Percentage Height of THIS one
     * \param NEW: see 'FreeCity::get_NextSpot' or see 'FreeCity::SPLIT()
     */
    void SPLIT(amVK_TLookingGlass_Split Direction, float width_perc, float height_perc, FreeCityPlayer *NEW) {
        switch (Direction) 
        {
            /**
             * 1|2|3          1|2|3
             * 4|5|6    ->    4|0|6
             *          ->    4|5|6
             * 7|8|9          7|8|9
             */
            case amVK_LookingGlass_SplitTOP: {
                float _new_height_  = m_Viewport.height;
                m_Viewport.height  *= height_perc;
                      _new_height_ -= m_Viewport.height;

                NEW->Initialize(m_Viewport.x, m_Viewport.y, m_Viewport.width, _new_height_, 0, 1);
                m_Viewport.y      += _new_height_;
            }
            case amVK_LookingGlass_SplitDOWN: {
                float _new_height_  = m_Viewport.height;
                m_Viewport.height  *= height_perc;
                      _new_height_ -= m_Viewport.height;

                NEW->Initialize(m_Viewport.x, m_Viewport.y + m_Viewport.height, m_Viewport.width, _new_height_, 0, 1);
                // m_Viewport.y = m_Viewport.y
            }
            case amVK_LookingGlass_SplitLEFT: {             /** Copy of SplitTOP  'width' & 'height' swapped */
                float _new_width_  = m_Viewport.width;
                m_Viewport.width  *= width_perc;
                      _new_width_ -= m_Viewport.width;

                NEW->Initialize(m_Viewport.x, m_Viewport.y, _new_width_, m_Viewport.height, 0, 1);
                m_Viewport.x      += _new_width_;

            }
            case amVK_LookingGlass_SplitRIGHT: {            /** Copy of SplitDown  'width' & 'height' swapped */
                float _new_width  = m_Viewport.width;
                m_Viewport.width *= width_perc;
                      _new_width -= m_Viewport.width;

                NEW->Initialize(m_Viewport.x + m_Viewport.width, m_Viewport.y, _new_width, m_Viewport.height, 0, 1);
                // m_Viewport.x = m_Viewport.x
            }
            default:
                amVK_LOG_EX("[param] Direction: N/A  -> ", Direction);
        }
        NEW->NoConnectionSure();
    }

/**
 *   ╻ ╻   ╻ ╻╻┏ ┏━╸┏┳┓╺┳┓
 *   ╺╋╸   ┃┏┛┣┻┓┃  ┃┃┃ ┃┃
 *   ╹ ╹   ┗┛ ╹ ╹┗━╸╹ ╹╺┻┛
 */
    /** TODO: Support for MultiViewport:  https://www.saschawillems.de/blog/2018/06/08/multiview-rendering-in-vulkan-using-vk_khr_multiview/ */
    inline void vkCMD_SetViewport(VkCommandBuffer CMDBuf) {
        vkCmdSetViewport(CMDBuf, 0, 1, &m_Viewport);
    }
};








/** Shawn Levy Xoss.... xD */
/** 
 * This is a Feature provided by amVK.... [soon to be]
 * 
 * in Vulkan TOP-LEFT is (0, 0)
 * 
 * TODO: make all COMMAND Functions `vkCMD` compat & have them stuffed inside 1 single FUNCTION
 *       or 'amVKCMD_' if the parameter takes in amVK_CMDBUF
 * TODO: I might just like... make all the CONSTRUCTORs that there is in amVK... to be like INTIALIZE....
 * 
 * TODO: MEMORY_MANAGER
 */
class FreeCity {
    /** You can Find More variables below after CONSTRUCTOR section */
   public:
    FreeCityNPC   FreeGuy;              /** There must be 1 Default someone..... [This variable is just for fun... yk... not really that we need it ] */
    amVK_WI_MK2       *WI = nullptr;
    FreeCityPlayer  *data = nullptr;
    uint32_t            N;              /** DEFAULT: 4 | SET in CONSTRUCTOR */
    uint32_t         neXt = 0;

   private:
    FreeCityNPC *NPC_data = nullptr;    /** the number of this should be    N * 5   at first... by default  */
    uint32_t     NPC_neXt = 0;          /** if this is filled... we check the entire NPC_data for 'NPC_data[i]._inUse_ == false' & */
    uint32_t     NPC_neXt_MAX = 0;      /** stores the maximum reached    'NPC_neXt' value.... */
    uint16_t _now = 5;
    uint16_t _old = 5;
    /**
     * 2|2|2|2|2
     * 4|8|1|3|7
     * 4|8|5|3|7
     * 6|6|6|6|7
     * 
     * In this case Total needed is 34
     * 
     * 1 → 4     * 2 → 5     * 3 → 5     * 4 → 3     
     * 5 → 4     * 6 → 5     * 7 → 3     * 8 → 5
     * 
     * We are doing this to thinking that we dont wanna do ReAlloc at all.... but we will check if NPC_data is filled or not
     * if   N * 5    cant hold     
     *                                  '_old = _now; _now++;    <MALLOC> stuff'
     * is done
     */

   public:
    /** Allocated memory. (N * (40+40)Bytes) */
    FreeCityPlayer *Initialize(amVK_WI_MK2 *amVK_WI) {
        data     = static_cast<  FreeCityPlayer *> (malloc((sizeof(FreeCityPlayer) * N)+ 
                                                           (sizeof(FreeCityNPC)    * N*5)));        // wE DONT NEED TO USE '_now' here
        NPC_data = reinterpret_cast<FreeCityNPC *> (data + (sizeof(FreeCityPlayer) * N));

        
        last_width  = static_cast<float>(amVK_WI->extent.width);
        last_height = static_cast<float>(amVK_WI->extent.height);
        FreeCityPlayer *Player1 = data;
        Player1->Initialize(0, 0, last_width, last_height, 0, 1);
        Player1->NoConnectionSure();
        WI = amVK_WI; FreeGuy = {0, 0}; neXt++; 
        return Player1;
    }

    /** Allocated memory. (X*N* (40+40)Bytes) + memcpy  (sooo kinda fast) */
    inline void resize(uint32_t X = 2) {
        N *= X;
        FreeCityPlayer  *data_new = static_cast<  FreeCityPlayer *> (malloc((sizeof(FreeCityPlayer) * N)+ 
                                                                            (sizeof(FreeCityNPC)    * N*_now)));
        FreeCityNPC *NPC_data_new = reinterpret_cast<FreeCityNPC *> (data + (sizeof(FreeCityPlayer) * N));

        memcpy(    data_new, data,     (sizeof(FreeCityPlayer) * neXt));
        memcpy(NPC_data_new, NPC_data, (sizeof(FreeCityNPC) * NPC_neXt_MAX * _now));
        
        free(data);
        data = data_new;
        NPC_data = NPC_data_new;
    }
    inline bool should_resize(void) {
        return (neXt < N) ? false : true;
    }
    inline FreeCityPlayer *get_NextSpot(void) {
        return &data[neXt++];
    }

    FreeCity(uint32_t N = 8) : N(N) {}
   ~FreeCity() {}

   protected:
    FreeCity(const FreeCity&) = delete;               //Brendan's Solution
    FreeCity& operator=(const FreeCity&) = delete;    //Brendan's Solution






    public:
    float last_width;
    float last_height;

    void UpdateCity(void) {
        float width  = static_cast<float>(WI->extent.width);
        float height = static_cast<float>(WI->extent.height);

        float width_perc  = width  / last_width;
        float height_perc = height / last_height;

        /** Update Players */
        for (int i = 0; i < neXt; i++) {
            data[i].m_Viewport.width  *= width_perc;
            data[i].m_Viewport.height *= height_perc;

            data[i].m_Viewport.x *= width_perc;
            data[i].m_Viewport.y *= height_perc;
        }

        last_width  = width;
        last_height = height;
    }

    void UpdateLonelyFreeGuy(void) {
        last_width  = static_cast<float>(WI->extent.width);
        last_height = static_cast<float>(WI->extent.height);
        data[0].m_Viewport.width  = last_width;
        data[0].m_Viewport.height = last_height;
    }




    /* Get Player Pointer from NPC Pointer */
    #define get_pPlayer(pNPC) (data + pNPC->player_i)
    #define get_pPlayer_index(pPlayer) ((pPlayer - data) / 8)

    #define FreeCityPlayer_GET_NPC_W(pPlayer) (&NPC_data[pPlayer->_W-1])

    #define get_pNPC_W(pPlayer) (NPC_data + pPlayer->_W-1)
    #define get_pNPC_A(pPlayer) (NPC_data + pPlayer->_A-1)
    #define get_pNPC_S(pPlayer) (NPC_data + pPlayer->_S-1)
    #define get_pNPC_D(pPlayer) (NPC_data + pPlayer->_D-1)

    /* Get NPC from uint32_t a.k.a NPC_index */
    #define get_pNPC(NPC_ind) (NPC_data + NPC_ind-1)

    /** swap player entry in a FreeCityNPC */
    uint32_t swapPlayerEntry(uint32_t k, FreeCityPlayer *before, FreeCityPlayer *after) {
        while(k) {
            FreeCityNPC *NPC = get_pNPC(k);

            if (get_pPlayer(NPC) == before) {
                NPC->player_i = get_pPlayer_index(after);
                break;
            }
            k = NPC->player_i;
        }
    }


    inline void SPLIT(uint32_t N, amVK_TLookingGlass_Split Direction, float width_perc, float height_perc) {
        this->SPLIT(data + N, Direction, width_perc, height_perc);
    }
    void SPLIT(FreeCityPlayer *ThisOne, amVK_TLookingGlass_Split Direction, float width_perc, float height_perc) {
        ThisOne->SPLIT(Direction, width_perc, height_perc, &data[neXt++]);

        FreeCityPlayer *Spawned = data + neXt;
      //FreeCityPlayer *ThisOne = data + N;

        switch (Direction)
        {
            /**
             * 1|2|3          1|2|3
             * 4|5|6    ->    4|0|6
             *          ->    4|5|6
             * 7|8|9          7|8|9
             */
            case amVK_LookingGlass_SplitTOP: {
                // ---------- TOP ----------
                // TOP Linkings are gonna be now LINKED/attacged to 'NEW"
                Spawned->_W = ThisOne->_W;
                /** 
                 * Now we gotta do WASD, linking support 
                 * we are assuming that W.player > -1       [inUse]    if (this->_W)
                 */
                const FreeCityNPC  *W;
                FreeCityNPC *S;                                     /** W->S */
                uint32_t k;
                while (ThisOne->_W) {
                        W = get_pNPC(ThisOne->_W);                  /** FreeCityNPC      */

                        k = get_pPlayer(W)->_S;                     /** uint32_t         */
                    while (k) {
                        S = get_pNPC(k);                            /** FreeCityNPC      */
                        
                        if (get_pPlayer(S) == ThisOne) {            /** FreeCityPlayer   */
                            S->player_i = get_pPlayer_index(Spawned);     /** new Player Index */
                            break;
                        }
                        k = S->player_i;
                    }

                    ThisOne->_W = W->next_NPC_i;
                }


                // ---------- DOWN ----------
                // DOWN Linkings are gonna stay the same
              //ThisOne->_S = ThisOne->_S;
                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(ThisOne)), 0};
                Spawned->_S = NPC_neXt++; //This is      gonna be a new NPC occupied in NPC_data

                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned)), 0};
                ThisOne->_W = 0;          //This is also gonna be a new NPC occupied in NPC_data




                // ---------- LEFT ----------
                uint32_t *Spawned_last = &Spawned->_A;
                uint32_t *ThisOne_last = &ThisOne->_A;

                FreeCityNPC    *A_NPC;
                FreeCityPlayer *A;
                k = ThisOne->_A;
                
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    A_NPC = get_pNPC(k);
                    A = get_pPlayer(A_NPC);

                    float tmp1 = A->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** A starts   above ThisOne */
                        if ((A->m_Viewport.height + tmp1) < tmp2) { /** A finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = A_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &A_NPC->next_NPC_i;
                             k = A_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *A_NPC->next_NPC_i = 0

                            swapPlayerEntry(A->_D, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &A_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(A_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), A->_D)};
                             A->_D = NPC_neXt++;

                            k = A_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** A starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &A_NPC->next_NPC_i;
                        k = A_NPC->next_NPC_i;
                    }
                    
                    
                }




                // ---------- RIGHT ----------
                uint32_t *Spawned_last = &Spawned->_D;
                uint32_t *ThisOne_last = &ThisOne->_D;

                FreeCityNPC    *D_NPC;
                FreeCityPlayer *D;
                k = ThisOne->_D;
                // ---------- LEFT ----------
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    D_NPC = get_pNPC(k);
                    D = get_pPlayer(D_NPC);

                    float tmp1 = D->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** D starts   above ThisOne */
                        if ((D->m_Viewport.height + tmp1) < tmp2) { /** D finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = D_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &D_NPC->next_NPC_i;
                             k = D_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *D_NPC->next_NPC_i = 0

                            swapPlayerEntry(D->_A, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &D_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(D_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), D->_A)};
                             D->_A = NPC_neXt++;

                            k = D_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** D starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &D_NPC->next_NPC_i;
                        k = D_NPC->next_NPC_i;
                    }
                    
                    
                }
            }
            case amVK_LookingGlass_SplitDOWN: {
                // ---------- TOP ----------
                // TOP Linkings are gonna be now LINKED/attacged to 'NEW"
                Spawned->_W = ThisOne->_W;
                /** 
                 * Now we gotta do WASD, linking support 
                 * we are assuming that W.player > -1       [inUse]    if (this->_W)
                 */
                const FreeCityNPC  *W;
                FreeCityNPC *S;                                     /** W->S */
                uint32_t k;
                while (ThisOne->_W) {
                        W = get_pNPC(ThisOne->_W);                  /** FreeCityNPC      */

                        k = get_pPlayer(W)->_S;                     /** uint32_t         */
                    while (k) {
                        S = get_pNPC(k);                            /** FreeCityNPC      */
                        
                        if (get_pPlayer(S) == ThisOne) {            /** FreeCityPlayer   */
                            S->player_i = get_pPlayer_index(Spawned);     /** new Player Index */
                            break;
                        }
                        k = S->player_i;
                    }

                    ThisOne->_W = W->next_NPC_i;
                }


                // ---------- DOWN ----------
                // DOWN Linkings are gonna stay the same
              //ThisOne->_S = ThisOne->_S;
                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(ThisOne)), 0};
                Spawned->_S = NPC_neXt++; //This is      gonna be a new NPC occupied in NPC_data

                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned)), 0};
                ThisOne->_W = 0;          //This is also gonna be a new NPC occupied in NPC_data




                // ---------- LEFT ----------
                uint32_t *Spawned_last = &Spawned->_A;
                uint32_t *ThisOne_last = &ThisOne->_A;

                FreeCityNPC    *A_NPC;
                FreeCityPlayer *A;
                k = ThisOne->_A;
                
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    A_NPC = get_pNPC(k);
                    A = get_pPlayer(A_NPC);

                    float tmp1 = A->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** A starts   above ThisOne */
                        if ((A->m_Viewport.height + tmp1) < tmp2) { /** A finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = A_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &A_NPC->next_NPC_i;
                             k = A_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *A_NPC->next_NPC_i = 0

                            swapPlayerEntry(A->_D, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &A_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(A_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), A->_D)};
                             A->_D = NPC_neXt++;

                            k = A_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** A starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &A_NPC->next_NPC_i;
                        k = A_NPC->next_NPC_i;
                    }
                    
                    
                }




                // ---------- RIGHT ----------
                uint32_t *Spawned_last = &Spawned->_D;
                uint32_t *ThisOne_last = &ThisOne->_D;

                FreeCityNPC    *D_NPC;
                FreeCityPlayer *D;
                k = ThisOne->_D;
                // ---------- LEFT ----------
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    D_NPC = get_pNPC(k);
                    D = get_pPlayer(D_NPC);

                    float tmp1 = D->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** D starts   above ThisOne */
                        if ((D->m_Viewport.height + tmp1) < tmp2) { /** D finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = D_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &D_NPC->next_NPC_i;
                             k = D_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *D_NPC->next_NPC_i = 0

                            swapPlayerEntry(D->_A, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &D_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(D_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), D->_A)};
                             D->_A = NPC_neXt++;

                            k = D_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** D starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &D_NPC->next_NPC_i;
                        k = D_NPC->next_NPC_i;
                    }
                    
                    
                }
            }
            case amVK_LookingGlass_SplitLEFT: {
                // ---------- TOP ----------
                // TOP Linkings are gonna be now LINKED/attacged to 'NEW"
                Spawned->_W = ThisOne->_W;
                /** 
                 * Now we gotta do WASD, linking support 
                 * we are assuming that W.player > -1       [inUse]    if (this->_W)
                 */
                const FreeCityNPC  *W;
                FreeCityNPC *S;                                     /** W->S */
                uint32_t k;
                while (ThisOne->_W) {
                        W = get_pNPC(ThisOne->_W);                  /** FreeCityNPC      */

                        k = get_pPlayer(W)->_S;                     /** uint32_t         */
                    while (k) {
                        S = get_pNPC(k);                            /** FreeCityNPC      */
                        
                        if (get_pPlayer(S) == ThisOne) {            /** FreeCityPlayer   */
                            S->player_i = get_pPlayer_index(Spawned);     /** new Player Index */
                            break;
                        }
                        k = S->player_i;
                    }

                    ThisOne->_W = W->next_NPC_i;
                }


                // ---------- DOWN ----------
                // DOWN Linkings are gonna stay the same
              //ThisOne->_S = ThisOne->_S;
                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(ThisOne)), 0};
                Spawned->_S = NPC_neXt++; //This is      gonna be a new NPC occupied in NPC_data

                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned)), 0};
                ThisOne->_W = 0;          //This is also gonna be a new NPC occupied in NPC_data




                // ---------- LEFT ----------
                uint32_t *Spawned_last = &Spawned->_A;
                uint32_t *ThisOne_last = &ThisOne->_A;

                FreeCityNPC    *A_NPC;
                FreeCityPlayer *A;
                k = ThisOne->_A;
                
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    A_NPC = get_pNPC(k);
                    A = get_pPlayer(A_NPC);

                    float tmp1 = A->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** A starts   above ThisOne */
                        if ((A->m_Viewport.height + tmp1) < tmp2) { /** A finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = A_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &A_NPC->next_NPC_i;
                             k = A_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *A_NPC->next_NPC_i = 0

                            swapPlayerEntry(A->_D, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &A_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(A_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), A->_D)};
                             A->_D = NPC_neXt++;

                            k = A_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** A starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &A_NPC->next_NPC_i;
                        k = A_NPC->next_NPC_i;
                    }
                    
                    
                }




                // ---------- RIGHT ----------
                uint32_t *Spawned_last = &Spawned->_D;
                uint32_t *ThisOne_last = &ThisOne->_D;

                FreeCityNPC    *D_NPC;
                FreeCityPlayer *D;
                k = ThisOne->_D;
                // ---------- LEFT ----------
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    D_NPC = get_pNPC(k);
                    D = get_pPlayer(D_NPC);

                    float tmp1 = D->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** D starts   above ThisOne */
                        if ((D->m_Viewport.height + tmp1) < tmp2) { /** D finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = D_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &D_NPC->next_NPC_i;
                             k = D_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *D_NPC->next_NPC_i = 0

                            swapPlayerEntry(D->_A, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &D_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(D_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), D->_A)};
                             D->_A = NPC_neXt++;

                            k = D_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** D starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &D_NPC->next_NPC_i;
                        k = D_NPC->next_NPC_i;
                    }
                    
                    
                }
            }
            case amVK_LookingGlass_SplitRIGHT: {
                // ---------- TOP ----------
                // TOP Linkings are gonna be now LINKED/attacged to 'NEW"
                Spawned->_W = ThisOne->_W;
                /** 
                 * Now we gotta do WASD, linking support 
                 * we are assuming that W.player > -1       [inUse]    if (this->_W)
                 */
                const FreeCityNPC  *W;
                FreeCityNPC *S;                                     /** W->S */
                uint32_t k;
                while (ThisOne->_W) {
                        W = get_pNPC(ThisOne->_W);                  /** FreeCityNPC      */

                        k = get_pPlayer(W)->_S;                     /** uint32_t         */
                    while (k) {
                        S = get_pNPC(k);                            /** FreeCityNPC      */
                        
                        if (get_pPlayer(S) == ThisOne) {            /** FreeCityPlayer   */
                            S->player_i = get_pPlayer_index(Spawned);     /** new Player Index */
                            break;
                        }
                        k = S->player_i;
                    }

                    ThisOne->_W = W->next_NPC_i;
                }


                // ---------- DOWN ----------
                // DOWN Linkings are gonna stay the same
              //ThisOne->_S = ThisOne->_S;
                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(ThisOne)), 0};
                Spawned->_S = NPC_neXt++; //This is      gonna be a new NPC occupied in NPC_data

                NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned)), 0};
                ThisOne->_W = 0;          //This is also gonna be a new NPC occupied in NPC_data




                // ---------- LEFT ----------
                uint32_t *Spawned_last = &Spawned->_A;
                uint32_t *ThisOne_last = &ThisOne->_A;

                FreeCityNPC    *A_NPC;
                FreeCityPlayer *A;
                k = ThisOne->_A;
                
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    A_NPC = get_pNPC(k);
                    A = get_pPlayer(A_NPC);

                    float tmp1 = A->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** A starts   above ThisOne */
                        if ((A->m_Viewport.height + tmp1) < tmp2) { /** A finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = A_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &A_NPC->next_NPC_i;
                             k = A_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *A_NPC->next_NPC_i = 0

                            swapPlayerEntry(A->_D, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &A_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(A_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), A->_D)};
                             A->_D = NPC_neXt++;

                            k = A_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** A starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &A_NPC->next_NPC_i;
                        k = A_NPC->next_NPC_i;
                    }
                    
                    
                }




                // ---------- RIGHT ----------
                uint32_t *Spawned_last = &Spawned->_D;
                uint32_t *ThisOne_last = &ThisOne->_D;

                FreeCityNPC    *D_NPC;
                FreeCityPlayer *D;
                k = ThisOne->_D;
                // ---------- LEFT ----------
                // All LEFT Linkings need to be checked.... height & y
                while(k) {
                    D_NPC = get_pNPC(k);
                    D = get_pPlayer(D_NPC);

                    float tmp1 = D->m_Viewport.y;
                    float tmp2 = ThisOne->m_Viewport.y;
                    if (tmp1 < tmp2) {                              /** D starts   above ThisOne */
                        if ((D->m_Viewport.height + tmp1) < tmp2) { /** D finishes above ThisOne */
                            //binds to Only Spawned._A enrtry

                            *ThisOne_last = D_NPC->next_NPC_i;      /** */
                            *Spawned_last = k;
                             Spawned_last = &D_NPC->next_NPC_i;
                             k = D_NPC->next_NPC_i;
                            *Spawned_last = 0;      // *D_NPC->next_NPC_i = 0

                            swapPlayerEntry(D->_A, ThisOne, Spawned);
                        }
                        else {
                            //binds to Both Spawned._A & ThisOne._A enrtry
                             ThisOne_last = &D_NPC->next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(D_NPC->player_i), 0};
                            *Spawned_last = NPC_neXt++;
                             Spawned_last = &NPC_data[NPC_neXt].next_NPC_i;

                             NPC_data[NPC_neXt] = {static_cast<int>(get_pPlayer_index(Spawned), D->_A)};
                             D->_A = NPC_neXt++;

                            k = D_NPC->next_NPC_i;
                        }
                    }
                    else {                                          /** D starts  within ThisOne */
                        //binds to Only ThisOne._A enrtry
                        ThisOne_last = &D_NPC->next_NPC_i;
                        k = D_NPC->next_NPC_i;
                    }
                    
                    
                }
            }
        }
    }
};


#endif