// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

//#include<GL/gl3w.h>
//#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <stdio.h>


#include"BackstageWindow.h"
#include"Camera.h"


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static int windowWidth = 1500;              //전체 창
static int windowHeight = 800;              //전체 창
static int screenWidth = 800;               //백스테이지 윈도우
static int screenHeight = 600;              //백스테이지 윈도우
static int inspectorWidth = 300;                //하이어라키 윈도우
static int hierachyWidth = 200;                 //하이어라키 윈도우
static int projectWindowHeight = 200;       //프로젝트 윈도우

GLFWwindow* window;

BackstageWindow* win;

double _frame_start_time;
double _delta_time;

static float transform[3] = { 0.f, 0.f, 0.f };
static float rotation[3] = { 0.f, 0.f, 0.f };
static float scale[3] = { 0.f, 0.f, 0.f };

const char* objName;
OBJect* selectedObj;
static int selectedObjID = 0;
static char str0[32]{ " ", };

bool isRightMouseClicked = false;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

bool isMiddleMouseClicked = false;
float lastPosX = 800.0f / 2.0;
float lastPosY = 600.0 / 2.0;

bool isEditingText = false;

static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (!isEditingText) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            win->cam.ProcessKeyboard(FORWARD, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            win->cam.ProcessKeyboard(BACKWARD, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            win->cam.ProcessKeyboard(LEFT, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            win->cam.ProcessKeyboard(RIGHT, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            win->cam.ProcessKeyboard(UPPER, _delta_time);
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            win->cam.ProcessKeyboard(DOWN, _delta_time);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    win->cam.ProcessMouseScroll(yoffset,_delta_time);
}

// glfw: whenever the mouse moves, this callback is called
// mouse right click : camera rotation
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held

    ImGui::GetIO().MousePos.x = float(xposIn);
    ImGui::GetIO().MousePos.y = float(yposIn);

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    //int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS) {
        if (!isRightMouseClicked)
        {
            //lastX = xpos;
            lastX = 0;
            //lastY = ypos;
            lastY = 0;
            isRightMouseClicked = true;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

     

        win->cam.ProcessMouseMovement(xoffset, yoffset,_delta_time);
    }
    else {
        isRightMouseClicked = true;
        lastX = xpos;
        lastY = ypos;
    }

    //마우스 휠 버튼 클릭 감지
    int button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (button == GLFW_PRESS) {
        //캠 위치 이동
        if (!isMiddleMouseClicked)
        {
            lastPosX = 0;
            lastPosY = 0;
            isMiddleMouseClicked = true;
        }

        float xoffset = xpos - lastPosX;
        float yoffset = ypos-lastPosY; // reversed since y-coordinates go from bottom to top

        win->cam.cameraPositionMove(xoffset, yoffset, _delta_time);
    }
    else {
        isMiddleMouseClicked = true;
        lastPosX = xpos;
        lastPosY = ypos;
    }
}


//ImDrawCallback
static void backstage_draw_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
    win->DrawBackstageWindow(window,screenWidth, screenHeight);
}


static void ShowMenuBarOverlay(bool* p_open) 
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //ShowExampleMenuFile();
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Object")) {
            if (ImGui::MenuItem("Create Empty")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) { win->createBuiltInOBJ(0); }
                if (ImGui::MenuItem("Sphere")) { win->createBuiltInOBJ(1); }
                if (ImGui::MenuItem("Cylinder")) { win->createBuiltInOBJ(2); }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::BeginMenu("Layout")) {
                if (ImGui::MenuItem("2 by 3")) {}
                if (ImGui::MenuItem("4 split")) {}
                if (ImGui::MenuItem("Default")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Extra")) {
            if (ImGui::MenuItem("Profile")) {
                //statUI(win);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void ShowInspectorOverlay(bool* p_open)
{
    ImGui::SetNextWindowPos(ImVec2(windowWidth - inspectorWidth, 22));
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::SetNextWindowSize(ImVec2(inspectorWidth, windowHeight-projectWindowHeight-((ImGui::GetStyle().FramePadding.y*2)+18)));
    if (!ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    if (selectedObj != NULL)
    {
        strcpy(str0, selectedObj->name.c_str());
        if (ImGui::InputText("Object Name", str0, 64)) {
            char* s = str0;
            win->setObjectName(s, selectedObjID);
        }
    }


    ImGui::Separator();
    ImGui::Text("Transform");
    ImGui::Separator();
    ImGui::InputFloat3("Position", transform);
    ImGui::Separator();
    ImGui::InputFloat3("Rotation", rotation);
    ImGui::Separator();
    ImGui::InputFloat3("Scale", scale);


    ImGui::End();
}

static void ShowProjectOverlay(bool* p_opent) 
{
    ImGui::SetNextWindowPos(ImVec2(0, windowHeight- projectWindowHeight));
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, projectWindowHeight));
    if (!ImGui::Begin("Project", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }

    ImGui::End();
}

/// <summary>
/// 하이어라키 화면
/// </summary>
/// <param name="p_open"></param>
static void ShowHierachyOverlay(bool* p_open)
{
    ImGui::SetNextWindowPos(ImVec2(0, 22));
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::SetNextWindowSize(ImVec2(hierachyWidth, windowHeight - projectWindowHeight -((ImGui::GetStyle().FramePadding.y * 2) + 18)));

    if (!ImGui::Begin("Hierachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }


    {
        char label[128];
        for (int i = 0; i < win->getObjectNum(); i++) {
            std::string str = win->getObjectName(i);
            objName = const_cast<char*>(str.c_str());
            sprintf_s(label, "%s", objName);
            if (ImGui::Selectable(label, selectedObjID == i)) {
                selectedObjID = win->getObjectID(i);
                selectedObj = win->getObject(i);
            }
        }
    }

    ImGui::End();
}

static void ShowBackstageOverlay(bool* p_open) 
{
    ImVec2 backstageSize = ImVec2(windowWidth - hierachyWidth - inspectorWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18));
    ImGui::SetNextWindowPos(ImVec2(hierachyWidth, 22));
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowSize(backstageSize);

    if (!ImGui::Begin("Backstage", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
    {

        ImGui::End();
        return;
    }
    /*ImDrawList* draw_list;
    draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCallback(backstage_draw_callback, NULL);*/
    win->DrawBackstageWindow(window, screenWidth, screenHeight);

    ImGui::End();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    hierachyWidth = windowHeight * 0.3f;
    screenWidth = windowWidth - inspectorWidth - hierachyWidth;
    screenHeight = windowHeight - projectWindowHeight;
    win->SetWindowSize(screenWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), hierachyWidth, projectWindowHeight,width,height);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    win->SetViewport(width, height);
   
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    bool init = false;

    if (init = ImGui_ImplOpenGL3_Init(glsl_version) == false) {
        printf("ImGui_ImplOpenGL3_Init return 0");
        return -1;
    }
        

    glfwSetKeyCallback(window, key_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_menubar_window = true;
    bool show_hierachy_window = true;
    bool show_inspector_window = true;
    bool show_project_window = true;
    bool show_backstage_window = true;
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

       /* if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);*/

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