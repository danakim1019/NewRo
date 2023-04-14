#pragma once

#include "ImGUI/imgui.h"
#include"ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include"BackstageWindow.h"

static int windowWidth = 1500;              //전체 창
static int windowHeight = 800;              //전체 창
static int screenWidth = 800;               //백스테이지 윈도우
static int screenHeight = 600;              //백스테이지 윈도우
static int inspectorWidth = 300;                //하이어라키 윈도우
static int hierachyWidth = 200;                 //하이어라키 윈도우
static int projectWindowHeight = 200;       //프로젝트 윈도우

double _frame_start_time;
double _delta_time;

static float transform[3] = { 0.f, 0.f, 0.f };
static float rotation[3] = { 0.f, 0.f, 0.f };
static float scale[3] = { 0.f, 0.f, 0.f };

const char* objName;
OBJect* selectedObj;
static unsigned int selectedObjID = 0;
static char str0[32]{ " ", };

bool isLeftMouseClicked = false;

bool isRightMouseClicked = false;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

bool isMiddleMouseClicked = false;
float lastPosX = 800.0f / 2.0;
float lastPosY = 600.0 / 2.0;

bool isEditingText = false;

int xPos;
int yPos;

GLFWwindow* window;
BackstageWindow* win;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    
    if (ImGui::TempInputIsActive(ImGui::GetActiveID())) {
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
    win->cam.ProcessMouseScroll(yoffset, _delta_time);
}

//cujrsor_pos_callback
static void cursor_pos_callback(GLFWwindow* window, double xposIn, double yposIn) {
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held

    ImGui::GetIO().MousePos.x = float(xposIn);
    ImGui::GetIO().MousePos.y = float(yposIn);

    xPos = static_cast<float>(xposIn);
    yPos = static_cast<float>(yposIn);

    if (isRightMouseClicked) {

        float xoffset = xPos - lastX;
        float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to to

        win->cam.ProcessMouseMovement(xoffset, yoffset, _delta_time);
    }
    if (isMiddleMouseClicked) {
        float xoffset = xPos - lastPosX;
        float yoffset = yPos - lastPosY; // reversed since y-coordinates go from bottom to top

        win->cam.cameraPositionMove(xoffset, yoffset, _delta_time);
    }
}

// glfw: whenever the mouse moves, this callback is called
// mouse right click : camera rotation
// -------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window,button,action,mods);

    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if ((xPos > hierachyWidth && xPos < hierachyWidth + screenWidth)
                && (yPos > 20 && yPos < screenHeight)) {
                if (!isLeftMouseClicked) {
                    unsigned int select = win->selectObject(xPos, yPos, selectedObjID);
                    if (select > 0) {
                        selectedObjID = select;
                        selectedObj = win->getObject(selectedObjID);
                    }
                    else {
                        selectedObjID = -1;
                        selectedObj = NULL;
                        printf("not picking");
                    }
                    isLeftMouseClicked = true;
                }
            }
        }
        else {
            isLeftMouseClicked = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            if (!isRightMouseClicked)
            {
                lastX = xPos;
                lastY = yPos;
                isRightMouseClicked = true;
            }
        }
        else {
            isRightMouseClicked = false;
        }
    }

    //마우스 휠 버튼 클릭 감지
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            //캠 위치 이동
            if (!isMiddleMouseClicked)
            {
                lastPosX = xPos;
                lastPosY = yPos;
                isMiddleMouseClicked = true;
            }
        }
        else {
            isMiddleMouseClicked = false;
        }
    }
}


//ImDrawCallback
static void backstage_draw_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
    win->DrawBackstageWindow(screenWidth, screenHeight);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    win->SetViewport(width, height);

}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    hierachyWidth = windowHeight * 0.3f;
    screenWidth = windowWidth - inspectorWidth - hierachyWidth;
    screenHeight = windowHeight - projectWindowHeight;
    win->SetWindowSize(screenWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), hierachyWidth, projectWindowHeight, width, height);
}

class GUIInterface {
public:
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
        ImGui::SetNextWindowSize(ImVec2(inspectorWidth, windowHeight - projectWindowHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18)));
        if (!ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::End();
            return;
        }

        if (selectedObj != NULL)
        {
            //name
            strcpy(str0, selectedObj->name.c_str());
            if (ImGui::InputText("Object Name", str0, 64)) {
                char* s = str0;
                win->setObjectName(s, selectedObjID);
            }

            //Transform
            glm::vec3 pos = selectedObj->getPositon();
            transform[0] = pos.x;
            transform[1] = pos.y;
            transform[2] = pos.z;

            glm::vec3 rot = selectedObj->getRotation();
            rotation[0] = rot.x;
            rotation[1] = rot.y;
            rotation[2] = rot.z;

            glm::vec3 sca = selectedObj->getScale();
            scale[0] = sca.x;
            scale[1] = sca.y;
            scale[2] = sca.z;

            ImGui::Separator();
            ImGui::Text("Transform");
            ImGui::Separator();
            ImGui::InputFloat3("Position", transform);
            ImGui::Separator();
            ImGui::InputFloat3("Rotation", rotation);
            ImGui::Separator();
            ImGui::InputFloat3("Scale", scale);


            selectedObj->setPosition(transform[0], transform[1], transform[2]);
            selectedObj->setRotation(rotation[0], rotation[1], rotation[2]);
            selectedObj->setScale(scale[0], scale[1], scale[2]);
        }

        ImGui::End();
    }

    static void ShowProjectOverlay(bool* p_opent)
    {
        ImGui::SetNextWindowPos(ImVec2(0, windowHeight - projectWindowHeight));
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
        ImGui::SetNextWindowSize(ImVec2(hierachyWidth, windowHeight - projectWindowHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18)));

        if (!ImGui::Begin("Hierachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
        {
            ImGui::End();
            return;
        }


        {
            char label[128];
            for (int i = 1; i < win->getObjectNum() + 1; i++) {
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
        win->DrawBackstageWindow(screenWidth, screenHeight);

        ImGui::End();
    }
};