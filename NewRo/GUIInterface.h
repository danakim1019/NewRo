// https://github.com/danakim1019/NewRo
// The MIT License(MIT)
//
//Copyright (C) 2023 by danakim1019
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 

#pragma once

#include"Callback.h"

GLFWwindow* window;

#define OBJectStartNum 10

class GUIInterface :public Callback {
public:

	GUIInterface() {}
	~GUIInterface(){}

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
					if (ImGui::MenuItem("Import Obj")) { backStageWin->CreateBuiltInOBJ(3); }
					if (ImGui::MenuItem("Import FBX")) { backStageWin->CreateBuiltInOBJ(4); }
					ImGui::Separator();
					if (ImGui::MenuItem("Cube")) { backStageWin->CreateBuiltInOBJ(0); }
					if (ImGui::MenuItem("Sphere")) { backStageWin->CreateBuiltInOBJ(1); }
					if (ImGui::MenuItem("Cylinder")) { backStageWin->CreateBuiltInOBJ(2); }
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
		static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

		ImGui::SetNextWindowPos(ImVec2(st_windowWidth - st_inspectorWidth, 22));
		ImGui::SetNextWindowBgAlpha(0.5f);
		ImGui::SetNextWindowSize(ImVec2(st_inspectorWidth, st_windowHeight - st_projectWindowHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18)));
		if (!ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}

		if (st_selectedObj != NULL)
		{
			//name
			strcpy(st_str0, st_selectedObj->mName.c_str());
			if (ImGui::InputText("Object Name", st_str0, 64)) {
				char* s = st_str0;
				backStageWin->setObjectName(s, st_selectedObjID);
			}

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Spacing();
				//worldTransform
				glm::vec3 pos = st_selectedObj->getPositon();
				st_worldTransform[0] = pos.x;
				st_worldTransform[1] = pos.y;
				st_worldTransform[2] = pos.z;

				glm::vec3 rot = st_selectedObj->getRotation();
				st_rotation[0] = rot.x;
				st_rotation[1] = rot.y;
				st_rotation[2] = rot.z;

				glm::vec3 sca = st_selectedObj->getScale();
				st_scale[0] = sca.x;
				st_scale[1] = sca.y;
				st_scale[2] = sca.z;

				ImGui::InputFloat3("Position", st_worldTransform);
				ImGui::InputFloat3("Rotation", st_rotation);
				ImGui::InputFloat3("Scale", st_scale);

				st_selectedObj->setPosition(st_worldTransform[0], st_worldTransform[1], st_worldTransform[2]);
				st_selectedObj->setRotation(st_rotation[0], st_rotation[1], st_rotation[2]);
				st_selectedObj->setScale(st_scale[0], st_scale[1], st_scale[2]);
			}

			static int shadowType_current = 0;
			static bool isShadow = true;
			const char* items[] = { "HARD_SHADOW","INTERPOLED_SHADOW" ,"PCF_SHADOW" ,"INTERPOLED_PCF_SHADOW","VSM_SHADOW","FILTERED_VSM_SHADOW" };
			isShadow = backStageWin->getShadow(0)->bIsShadow;
			shadowType_current = backStageWin->getShadow(0)->mShadowType;
			if (st_selectedObj->mObjectType == "Light") {
				if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					ImGui::Checkbox("isShadow", &isShadow);
					ImGui::Combo("Shadow Type", &shadowType_current, items, IM_ARRAYSIZE(items));
					static ImVec4 light_color = ImVec4(1, 1, 1, 1);
					ImGui::ColorEdit4("Light Color", &light_color.x, ImGuiColorEditFlags_NoInputs);
				}
			}
			backStageWin->setShadow(isShadow, shadowType_current);

			if (st_selectedObj->mMat != NULL) {
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					static bool alpha_preview = true;
					static bool alpha_half_preview = false;
					static bool drag_and_drop = true;
					static bool options_menu = true;
					static bool hdr = false;
					ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
					static ImVec4 Ka_color = ImVec4(1, 1, 1, 1), Kd_color = ImVec4(1, 1, 1, 1), Ks_color = ImVec4(1, 1, 1, 1);

					float* valueM = st_selectedObj->mMat->getKa();
					ImVec4 _value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Ka_color = _value;

					valueM = st_selectedObj->mMat->getKd();
					_value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Kd_color = _value;

					valueM = st_selectedObj->mMat->getKs();
					_value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Ks_color = _value;

					static float shiness;
					shiness = st_selectedObj->mMat->getShiness();

					ImGui::SliderFloat("Shiness", &shiness, 0, 32);
					ImGui::ColorEdit4("Ka", &Ka_color.x, ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
					ImGui::ColorEdit4("Kd", &Kd_color.x, ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
					ImGui::ColorEdit4("Ks", &Ks_color.x, ImGuiColorEditFlags_NoInputs);

					float* valueKa = new float[3] {Ka_color.x, Ka_color.y, Ka_color.z};
					float* valueKd = new float[3] {Kd_color.x, Kd_color.y, Kd_color.z};
					float* valueKs = new float[3] {Ks_color.x, Ks_color.y, Ks_color.z};
					st_selectedObj->mMat->setShaderValue(valueKa, valueKd, valueKs, shiness);

				}
			}
		}

		ImGui::End();
	}

	static void ShowProjectOverlay(bool* p_opent)
	{
		ImGui::SetNextWindowPos(ImVec2(0, st_windowHeight - st_projectWindowHeight));
		ImGui::SetNextWindowBgAlpha(0.5f);
		ImGui::SetNextWindowSize(ImVec2(st_windowWidth, st_projectWindowHeight));
		
		if (!ImGui::Begin("Project", NULL, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::End();
			return;
		}

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Project"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Debug"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animation"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();


		ImGui::End();
	}

	// Hiearchy Window
	static void ShowHierachyOverlay(bool* p_open)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 22));
		ImGui::SetNextWindowBgAlpha(0.5f);
		ImGui::SetNextWindowSize(ImVec2(st_hierachyWidth, st_windowHeight - st_projectWindowHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18)));

		if (!ImGui::Begin("Hierachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
			return;
		}


		{
			char label[128];
			for (int i = 1; i < backStageWin->getObjectNum() + 1; i++) {
				std::string str = backStageWin->getObjectName(i);
				st_objName = const_cast<char*>(str.c_str());
				sprintf_s(label, "%s", st_objName);
				if (ImGui::Selectable(label, st_selectedObjID == i)) {
					st_selectedObjID = backStageWin->getObjectID(i);
					st_selectedObj = backStageWin->getObject(i);

				}
			}
		}

		ImGui::End();
	}

	static void ShowBackstageOverlay(bool* p_open, float deltaTime)
	{
		ImVec2 backstageSize = ImVec2(st_windowWidth - st_hierachyWidth - st_inspectorWidth, st_screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18));
		ImGui::SetNextWindowPos(ImVec2(st_hierachyWidth, 22));
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::SetNextWindowSize(backstageSize);

		if (!ImGui::Begin("Backstage", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
			return;
		}

		backStageWin->DrawBackstageWindow(st_screenWidth, st_screenHeight, st_selectedObjID, deltaTime);

		ImGui::End();
	}
};