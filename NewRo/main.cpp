// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <stdio.h>


//#include"BackstageWindow.h"
#include"Camera.h"
#include"GUIInterface.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class Core : public GUIInterface
{
public:
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_menubar_window = true;
    bool show_hierachy_window = true;
    bool show_inspector_window = true;
    bool show_project_window = true;
    bool show_backstage_window = true;

    Core(){
        show_demo_window = true;
        show_another_window = false;
        show_menubar_window = true;
        show_hierachy_window = true;
        show_inspector_window = true;
        show_project_window = true;
        show_backstage_window = true;
    }
    void GUIRender() {
        ShowMenuBarOverlay(&show_menubar_window);

        if (show_hierachy_window) {
            ShowHierachyOverlay(&show_hierachy_window);
        }
        if (show_inspector_window) {
            ShowInspectorOverlay(&show_inspector_window);
        }
        if (show_project_window) {
            ShowProjectOverlay(&show_project_window);
        }

        if (show_backstage_window) {
            ShowBackstageOverlay(&show_backstage_window);
        }

        /*if (show_demo_window)
             ImGui::ShowDemoWindow(&show_demo_window);*/
    }
};



int main(int, char**)           
{
    Core core;

    // Setup window
    glfwSetErrorCallback(core.glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    
    // Create window with graphics context
    window = glfwCreateWindow(windowWidth, windowHeight, "NewRo Engine", NULL, NULL);

    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);  
    glfwSwapInterval(1); // Enable vsync


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    bool init = false;

    if (init = ImGui_ImplOpenGL3_Init(glsl_version) == false) {
        printf("ImGui_ImplOpenGL3_Init return 0");
        return -1;
    }       

    glfwSetKeyCallback(window, core.key_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, core.window_size_callback);
    glfwSetScrollCallback(window, core.scroll_callback);
    glfwSetCursorPosCallback(window, core.cursor_pos_callback);
    glfwSetMouseButtonCallback(window, core.mouse_button_callback);
    

    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImVec2 backstageSize = ImVec2(windowWidth - hierachyWidth - inspectorWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18));
    win = new BackstageWindow(backstageSize.x, backstageSize.y, windowWidth,windowHeight);
    screenWidth = windowWidth - inspectorWidth - hierachyWidth;
    screenHeight = windowHeight - projectWindowHeight;
    win->SetWindowSize(screenWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), hierachyWidth, projectWindowHeight, windowWidth, windowHeight);



    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        _delta_time = glfwGetTime() - _frame_start_time;
        _frame_start_time = glfwGetTime();
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        

        core.GUIRender();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}