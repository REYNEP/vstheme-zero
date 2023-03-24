#define WALLOWS
#define WALLOWS_V2

#include "amGHOST_V.hh"
#include "KeyMiceInput/EK.hh"

#include "amVK_Instance.hh"
#include "amVK_Device.hh"
#include "amVK_WI.hh"               // amVK_Surface, Needed to create amVK_Device
#include "amVK_RenderPass.hh"       // Needed to create amVK_WI later on

#include "amVK_Pipeline.hh"
#include "amVK_Descriptors.hh"

#include "agents/2D_Image.hh"
#include "amGHOST_Decode.hh"        // 👻 PNG-Decoding LIB, uses WUFFs by Google 😜
#include "amVK_ImgNBuf.hh"
#include "Enzo_Fernandez.hh"

#include "Lautaro_Martinez.hh"


/**
 * TODO: CMake: 
 *    1. USE CMake FindPackage, Check, Use & Log that if Found
 *    2. If not found, try to find it in extern amVK Folder...
 *    3.    People Can Specify variables like <amVK_EXT_glslang>
 * 
 *    4. Detect if its a SOURCE Dir without BINARIES & LIB
 *    5.    if so, use the GLSLang_CMakeLists.cmake file
 *    6. FurthurMore, we can detect the needed HEADER files....
 * 
 *    7. On the amVK Drive folder, We should only provide the BINARIES & HEADERS
 *    8.    That means Maintaining the GLSLang_CMakeLists.cmake, locally here at my PC
 *    9. We can Make DEBUG / RELEASE different builds & Upload to drive....
 *    
 *   10. If still not found, REPORT amVK ERROR Report... & Directions to solve
 *    
 * TODO:   AMG: amGHOST_V.hh includes every module.... add a way to include only one....
 *              I know... iknow... that ppl can just add their desired module... xD
 *              but everything in amGHOST seems to be connected.... like everything including one another
 *              I just wanna optimize that
 * 
 * TODO: SCons: Use that like Godot
 * TODO: meson: We are there... halfway
 * TODO: EVTNG: HANDLE Destruction
 * 
 * TODO:  amVK: Insntance::filter_SurfaceExts() improve.... support for xcb/xlib
 * TODO:  amVK: add 'konf' prefix to funcs thats called by konfigurieren()
 * TODO:  amVK: expose filter_SurfaceFormats... furthur through amVK_WI... + solve the TODOs
 * TODO:  amVK: activeDevice support
 * TODO:  amVK: ArrayDYN.... seperate into INITIALIZE()... whats inside CONSTRUCTOR....   wont hurt perf.. ig       [ANY NUMBER OF amVK_CommandBuffers inside amVK_CommandPool]
 * TODO:  amVK: Better way to create... DEADPOOL:sInfo
 * 
 * TODO: https://stackoverflow.com/questions/71626199/can-you-transfer-directly-to-an-image-without-using-an-intermediary-buffer
 * https://www.youtube.com/watch?v=Rol0iKEXk_8
 */
int main(void) {
        /**     GHOST - Generic Handy Operating System Toolkit 👻      */
    amGHOST_Window *wOne = MARADONA->create_window(":𝐫𝐞", 0, 0, 600, 600);
        /** [EK.hh]    EK   - Event Konsument. xD (German) 👀
            Your own Function to process amGHOST Event (OS Events)     */
    MARADONA->add_EventKonsument(&EK);




        /** true: Debug Extensions                                     
            some EXTs like VK_KHR_SURFACE & VK_KHR_[win32/xcb/x11/wayland/macos]_surface are enabled */
    amVK_InstanceMK2      *amVK = new amVK_InstanceMK2(true);
    amVK->Create_VkInstance();
        /**         INSTANCE-EXTS: amVK->add_InstanceExt('VK_EXT_swapchain_colorspace')
                      DEVICE-EXTS: amVK_ARRAY_PUSH_BACK(amVK_D.exts) = "VK_KHR_VIDEO_DECODE";        */
    amVK_SurfaceMK2     *amVK_S = new amVK_SurfaceMK2(wOne->create_vulkan_surface(amVK->s_Instance));                     // GHOST 👻 Creates VkSurface
    amVK_DeviceMK2      *amVK_D = new amVK_DeviceMK2(amVK_S->select_DisplayDevice(), 1, 0, amVK_DP_GRAPHICS);
    amVK_D->Create_VkDevice();
    amVK_D->Activate_Device();
        /** Now, it'll be okay if you dont pass amVK_D everywhere      */




        /** Why the RenderPass separated?
         *  To become a standard for ColorSpace infos 🎨 */
    amVK_RenderPassMK2 *amVK_RP = new amVK_RenderPassMK2(amVK_S, amVK_D);
    amVK_RP->Create_RenderPass();

        /**   WI: Window Interface     [highly modifiable]      \see class file, Lots of features... 
         *            CALL: These 3 & \fn 'CleanUp_Swapchain()'   when ReCreating Swapchain          */
    amVK_WI_MK2        *amVK_WI = new amVK_WI_MK2(":𝐫𝐞", amVK_S, amVK_RP, amVK_D);                      
    amVK_WI->Create_Swapchain();
    amVK_WI->create_Attachments();
    amVK_WI->create_Framebuffers();                                                         // Cool enough implementation for now [amVK_Array Used]  🙃
    wOne->window_userData = amVK_WI;
        /**     The purpose of amVK should be.... like we don't gotta create a 're_Window' thingy    */




        /**           This's like a 'Pipe'Store...🏪      where you build🏗️  PipeLines from         
            konfigurieren: Basic Configuration.... Still Modifiable.... 😉                           */
    amVK_GraphicsPipes *amVK_PS = new amVK_GraphicsPipes(amVK_RP, amVK_D);
    amVK_PS->konfigurieren();
    amVK_DeadPool     *DeadPool = new amVK_DeadPool(amVK_D);                                                              // 🏊 DescriptorPool🎱   [+++]
    DeadPool->Create();












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

    REY_AVL_NODE FUCK;
    REY_AVL_NODE FUCK2 = std::move(FUCK);


    Agent_Image2D::pipeline.Initialize(amVK_PS);
    Agent_Image2D aQuad = Agent_Image2D();
    REY_InYourMind::INITIALIZE_WUFFs();
    REY_InYourMind REYNEP("amGHOST/reynep.png");

        /** 
         * For now BufferMK2 & ImageMK2 is in beta stage... sooo this is how we set this... 
         * TODO: For Now we don't wanna use TRANSFER_SRC (STORAGE_TEXEL vs UNIFORM_TEXEL)
         *       Just wanna put in inside a CPU_WRITABLE but GPU_MEMORY
         */
    amVK_ImgNBuf_Kernal::Set_Device(amVK_D);
    amVK_ImageMK2  I = {};
    amVK_BufferMK2 B = {};
    B.Create(REYNEP.m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);                                                            // Allocates VkMemory 🧠
        /** vkMap, memcpy, vkUnmap       (instead of memcpy, directly decode image into it)    + Free INYOURMIND */
    B.CopyFrom(REYNEP.m_ptr);
    I.amvkCreateImage(VK_FORMAT_R8G8B8A8_SRGB, REYNEP.m_width, REYNEP.m_height);             // vkCreateImage, vkBindImageMemory(B), vkCreateImageView
    aQuad.ReadyDS(DeadPool, B, I);                                                           // Alloc DescriptorSet, CreateSampler, WriteDescriptorSet
    /** 
     * UNTILL FURTHUR NOTICE... If I really dont need to put these things inside ANOTHER Wrapper/Function, I wouldn't and just keep adding features here 
     * UP NEXT:
     *  1. AREA & Non-Stretched Image Patch
     *  2. VideoDecode
     *      2.0 Remove amVK_Printers
     *      2.1 VideoDecode
     *      2.2 Optimize Vulkan... [Not the LOOKs]
     *  3. Temperature, Tint, Exposure, Contrast    use KEY-BINDS [Q,W,E,R] to activate + ARROW keys to Increase....
     *  4. Highlights, Shadow, Whites, Blacks                     [U,I,O,P]
     *  5. Vibrance, Satutaion                                    [T,Y]
     **/

        /**     1 CommandBuffer .... also uses qFamily for Barrier     */
    Enzo_Fernandez *Enzo = new Enzo_Fernandez(amVK_D, 1);
    Enzo->Enzo_ThePlan(amVK_D->m_graphics_qFAM, 0);
    Enzo_Upload:
    {
        Enzo->Begin_CMDBuf(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
            Enzo->CopyBufferToImage(I.IMG, B.BUFFER, REYNEP.m_width, REYNEP.m_height);
        Enzo->End_CMDBuf();

        Enzo->QSubmit(amVK_D->get_graphics_queue());
        Enzo->CMDPool.Reset();                                                               // that will free/deallocate CommandBuffers too!!!
    }











        /**                         Area (K391)                        */
    //FreeCity C1(8);
    //FreeCityPlayer *FreeGuy = C1.Initialize(amVK_WI);
    //DayGlow_Screen S1(8);
    //DayGlow_Area *MainArea = S1.Initialize(amVK_WI);




        /**                         RD: Render                         */
    Lautaro_Martinez RD(amVK_D);
    RD.Lautaro_ThePlan(amVK_D->get_graphics_queue());
    RD.Enzo_ThePlan(
        amVK_D->m_graphics_qFAM, 
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VK_FENCE_CREATE_SIGNALED_BIT
    );
        /**  Soon, you will prolly need to rewrite this class LAUTARO  */
        /** Its a LOREEEEE .... Lautaro needs to be switched time2time */


                                                                                                                                                                                                                                                                                    
    while(true) {
            /**     amGHOST processed all events and puts in QUEUE     */
        amG_SYS->process_events(true);
        //S1.UpdateALL();
            /**       Calls that 'EK' from line 2 for every event      
             *                   Your EK can return '-1'
             *    If it does so... dispatch_events() return false 😄   */
        if (amG_SYS->dispatch_events() == false)
            break;

            /**          You should check this out yourself 😉         */
        RD.Render_BeginRecord(amVK_WI);
            /**              FOR non TRANSFER_SRC solution
              if (i == 0) vkCmdCopyBufferToImage(RD.m_cmdBuf.BUF, B.BUFFER, I.IMG, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,) */

        //vkCmdSetViewport(RD.ActiveCMDBuf, 0, 1, &xD);
        //MainArea->vkCMD_SetViewport(RD.ActiveCMDBuf);
        aQuad.Draw(RD.ActiveCMDBuf);
            /** 
             * YOU CAN THINK OF THIS LIKE: RenderKonsument....
             * 
             * You can think of this DRAW or COMMANDBUFFER thingy from vulkan.... 
             *        Just like how you seen EK: (EventKonsument) to work....
             *  Like it isn't getting drawn RN
             *        Rather, Its gonna be DRAWN.... by the GPU....
             *        after you submit the QUEUE.... to the GPU....
             *  Everything COMMAND: (vkCmd***) that was recorded
             *        those will get executed/done.. by the GPU.... in ORDER
            */
        
            /**      You should check this one out yourself TOOO 😉     */
        RD.EndRecord_N_Submit();
    }



    /** delete wOne, won't work. */
    amG_SYS->destroy_window(wOne);
    amG_SYS->dispose_system();                                                                                                           // CLean Up 😝

    return 0;
}