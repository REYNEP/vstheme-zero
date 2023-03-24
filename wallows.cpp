/** \see wallowsV2.cpp .... DEPRECATED since JAN 11, 2023 - 1A.M. */

#define WALLOWS
#include "2D_Quad.hh"
#include "2D_Image.hh"

#include "amGHOST_V.hh"
#include "KeyMiceInput/EK.hh"

#include "amVK_Instance.hh"
#include "amVK_Device.hh"
#include "amVK_RenderPass.hh"
#include "amVK_WI.hh"
#include "amVK_Pipeline.hh"     // Tho already included in 2D_Quad.hh & 2D_Image.hh
#include "Enzo_Fernandez.hh"
#include "amVK_ImgNBuf.hh"
#include "amVK_Descriptors.hh"

#include "Lautaro_Martinez.hh"
#include "amGHOST_Decode.hh"


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
 * TODO: SCons: Use that like Godot
 * TODO: meson: We are there... halfway
 * 
 * TODO:  amVK: Insntance::filter_SurfaceExts() improve.... support for xcb/xlib
 * TODO:  amVK: add 'konf' prefix to funcs thats called by konfigurieren()
 * TODO:  amVK: expose filter_SurfaceFormats... furthur through amVK_WI... + solve the TODOs
 * TODO:  amVK: activeDevice support
 * TODO:  amVK: ArrayDYN.... seperate into INITIALIZE()... whats inside CONSTRUCTOR....   wont hurt perf.. ig       [ANY NUMBER OF amVK_CommandBuffers inside amVK_CommandPool]
 * 
 * TODO: https://stackoverflow.com/questions/71626199/can-you-transfer-directly-to-an-image-without-using-an-intermediary-buffer
 * https://www.youtube.com/watch?v=Rol0iKEXk_8
 * 
 * 
 * \see wallowsV2.cpp .... DEPRECATED since JAN 11, 2023 - 1A.M. 
 *      But we still keep this here as an memory....
 *      WHY V2?
 *          - IDK... i was feeling like this was getting pretty MESSI here.... Comment blocks not so cool....
 *      NOTE: to compile this/see syntax-errors in VSCode add 'coDe/wallows.cpp' in 'CMakeLists.txt::SRC' variable
 */
int main(void) {
    amGHOST_Window        *wOne = amG_SYS->create_window(":𝐫𝐞", 0, 0, 600, 600);                        /**     GHOST - Generic Handy Operating System Toolkit 👻      */
    amG_SYS->add_EventKonsument(&EK);                                                                   /** [EK.hh]    EK   - Event Konsument. xD (German) 👀
                                                                                                            Your own Function to process amGHOST Event (OS Events)     */
    amVK_InstanceMK2      *amVK = new amVK_InstanceMK2(true);                                           /** true: Debug Extensions                                     */
    amVK->Create_VkInstance();
    // add_InstanceExt('VK_EXT_swapchain_colorspace')                                                   /** some EXTs like VK_KHR_SURFACE & VK_KHR_win32/xcb/x11/wayland/macos_surface are enabled */

    amVK_SurfaceMK2     *amVK_S = new amVK_SurfaceMK2(wOne->create_vulkan_surface(amVK->s_Instance));                                           // GHOST 👻 Creates VkSurface
    amVK_DeviceMK2      *amVK_D = new amVK_DeviceMK2(amVK_S->select_DisplayDevice(), 1, 0, amVK_DP_GRAPHICS);
    amVK_D->Create_VkDevice();                                                                          /** amVK_ARRAY_PUSH_BACK(amVK_D.exts) = "VK_KHR_VIDEO_DECODE"; */
    amVK_D->Activate_Device();                                                                          /** Now, it'll be okay if you dont pass amVK_D everywhere      */

    

    amVK_RenderPassMK2 *amVK_RP = new amVK_RenderPassMK2(amVK_S, amVK_D);                               // Why the RenderPass separated?
    amVK_RP->Create_RenderPass();                                                                       // To become a standard for ColorSpace infos 🎨

    amVK_WI_MK2        *amVK_WI = new amVK_WI_MK2(":𝐫𝐞", amVK_S, amVK_RP, amVK_D);                      /** WI: Window Interface     [highly modifiable]      \see class file, Lots of features... */
    amVK_WI->Create_Swapchain();                                                                        /**          CALL: These 3 & \fn 'CleanUp_Swapchain()'   when ReCreating Swapchain         */
    amVK_WI->create_Attachments();
    amVK_WI->create_Framebuffers();                                                                     /**                Cool enough implementation for now [amVK_Array Used]  🙃                */
    wOne->window_userData = amVK_WI;                                                                    /**    The purpose of amVK should be.... like we don't gotta create a 're_Window' thingy   */



    amVK_GraphicsPipes *amVK_PS = new amVK_GraphicsPipes(amVK_RP, amVK_D);                              // This is like a 'Pipe'Store... where you build PipeLines from
    amVK_PS->konfigurieren();                                                                           // Basic Configuration.... Still Modifiable.... 😉
    amVK_DeadPool     *DeadPool = new amVK_DeadPool(amVK_D);                                            // DescriptorPool   [+++]
    DeadPool->Create();                                                                                 // TODO: A Better way to create... sInfo

    Agent_Image2D::pipeline.Initialize(amVK_PS);                                                              // TODO: HANDLE Destruction
    Agent_Image2D aQuad = Agent_Image2D();
    REY_InYourMind REYNEP("amGHOST/reynep.png", 1);                                                     // uses WUFFs by google 🙃

    amVK_ImgNBuf_Kernal::Set_Device(amVK_D);                                                            // For now BufferMK2 & ImageMK2 is in beta stage... sooo this is how we set this...
    amVK_ImageMK2  I = {};
    amVK_BufferMK2 B = {};                                                                              // For Now we don't wanna use TRANSFER_SRC (STORAGE_TEXEL vs UNIFORM_TEXEL)
    B.Create(REYNEP.m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);                                          // Allocates VkMemory
    B.CopyFrom(REYNEP.m_ptr);                                                                           // vkMap, memcpy, vkUnmap       (instead of memcpy, directly decode image into it)    + Free INYOURMIND
    I.amvkCreateImage(VK_FORMAT_R8G8B8A8_SRGB, REYNEP.m_width, REYNEP.m_height);                        // vkCreateImage, vkBindImageMemory(B), vkCreateImageView
    aQuad.ReadyDS(DeadPool, B, I);                                                                      // Alloc DescriptorSet, CreateSampler, WriteDescriptorSet
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


    Enzo_Fernandez *Enzo = new Enzo_Fernandez(amVK_D, 1);                                               // 1 CommandBuffer
    Enzo->Enzo_ThePlan(amVK_D->m_graphics_qFAM, 0);                                                     // also uses qFamily for Barrier
    Enzo_Upload:
    {
        Enzo->Begin_CMDBuf(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
            Enzo->CopyBufferToImage(I.IMG, B.BUFFER, REYNEP.m_width, REYNEP.m_height);
        Enzo->End_CMDBuf();

        Enzo->QSubmit(amVK_D->get_graphics_queue());
        Enzo->CMDPool.Reset();                                                                          // that will free/deallocate CommandBuffers too!!!
    }



    Lautaro_Martinez RD(amVK_D);                                                                        // RD: Render
    RD.Lautaro_ThePlan(amVK_D->get_graphics_queue());
    RD.Enzo_ThePlan(
        amVK_D->m_graphics_qFAM, 
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VK_FENCE_CREATE_SIGNALED_BIT
    );                                                                                                  // Soon, you will prolly need to rewrite this class
    VkViewport xD = {0, 0, amVK_WI->extent.width, amVK_WI->extent.height, 0, 1};
    VkRect2D HOGA = {{0, 0}, {amVK_WI->extent.width, amVK_WI->extent.height}};

    while(true) {
        amG_SYS->process_events(true);                                                                  // amGHOST processed all events and puts in QUEUE
        xD.width = amVK_WI->extent.width;
        xD.height = amVK_WI->extent.height;
        HOGA.extent = {amVK_WI->extent.width, amVK_WI->extent.height};
        if (amG_SYS->dispatch_events() == false) {                                                      // Calls that 'EK' from line 2 for every event
            break;                                                                                      // Your EK can return '-1'
        }                                                                                               // If it does so... dispatch_events() return false

        RD.Render_BeginRecord(amVK_WI);                                                                 // 😄 You should check this out yourself 😉

        //if (i == 0) vkCmdCopyBufferToImage(RD.m_cmdBuf.BUF, B.BUFFER, I.IMG, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, )

        vkCmdSetViewport(RD.ActiveCMDBuf, 0, 1, &xD);
        vkCmdSetScissor(RD.ActiveCMDBuf, 0, 1, &HOGA);
        aQuad.Draw(RD.ActiveCMDBuf);                                                                    // This is more Like RenderKonsument....
                                                                                                        // Like it isn't getting drawn RN
        
        RD.EndRecord_N_Submit();                                                                        // This Too 😊
    }

    /** delete wOne, won't work. */
    amG_SYS->destroy_window(wOne);
    amG_SYS->dispose_system();                                                                          // CLean Up 😝

    return 0;
}
