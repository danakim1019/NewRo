#pragma once

#include"Callback.h"

GLFWwindow* window;

#define OBJectStartNum 10

class GUIInterface :public Callback{
public:
    GUIInterface() {
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
                    if (ImGui::MenuItem("Import")) { win->createBuiltInOBJ(3); }
                    ImGui::Separator();
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

            //worldTransform
            glm::vec3 pos = selectedObj->getPositon();
            worldTransform[0] = pos.x;
            worldTransform[1] = pos.y;
            worldTransform[2] = pos.z;

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
            ImGui::InputFloat3("Position", worldTransform);
            ImGui::Separator();
            ImGui::InputFloat3("Rotation", rotation);
            ImGui::Separator();
            ImGui::InputFloat3("Scale", scale);


            selectedObj->setPosition(worldTransform[0], worldTransform[1], worldTransform[2]);
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
            for (int i = OBJectStartNum+1; i < win->getObjectNum() + 1; i++) {
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
        win->DrawBackstageWindow(screenWidth, screenHeight,selectedObjID);


        ImGui::End();
    }
};