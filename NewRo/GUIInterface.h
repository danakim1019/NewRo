#pragma once

#include"Callback.h"

GLFWwindow* window;

#define OBJectStartNum 10

class GUIInterface :public Callback {
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
		static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

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
			strcpy(str0, selectedObj->mName.c_str());
			if (ImGui::InputText("Object Name", str0, 64)) {
				char* s = str0;
				win->setObjectName(s, selectedObjID);
			}

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Spacing();
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

				ImGui::InputFloat3("Position", worldTransform);
				ImGui::InputFloat3("Rotation", rotation);
				ImGui::InputFloat3("Scale", scale);

				selectedObj->setPosition(worldTransform[0], worldTransform[1], worldTransform[2]);
				selectedObj->setRotation(rotation[0], rotation[1], rotation[2]);
				selectedObj->setScale(scale[0], scale[1], scale[2]);
			}

			static int shadowType_current = 0;
			static bool isShadow = true;
			const char* items[] = { "HARD_SHADOW","INTERPOLED_SHADOW" ,"PCF_SHADOW" ,"INTERPOLED_PCF_SHADOW","VSM_SHADOW","FILTERED_VSM_SHADOW" };
			isShadow = win->getShadow(0)->bIsShadow;
			shadowType_current = win->getShadow(0)->mShadowType;
			if (selectedObj->mObjectType == "Light") {
				if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					ImGui::Checkbox("isShadow", &isShadow);
					ImGui::Combo("Shadow Type", &shadowType_current, items, IM_ARRAYSIZE(items));
					static ImVec4 light_color = ImVec4(1, 1, 1, 1);
					ImGui::ColorEdit4("Light Color", &light_color.x, ImGuiColorEditFlags_NoInputs);
				}
			}
			win->setShadow(isShadow, shadowType_current);

			if (selectedObj->mMat != NULL) {
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Spacing();
					static bool alpha_preview = true;
					static bool alpha_half_preview = false;
					static bool drag_and_drop = true;
					static bool options_menu = true;
					static bool hdr = false;
					ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
					static ImVec4 Ka_color = ImVec4(1, 1, 1, 1), Kd_color = ImVec4(1, 1, 1, 1), Ks_color = ImVec4(1, 1, 1, 1);

					float* valueM = selectedObj->mMat->getKa();
					ImVec4 _value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Ka_color = _value;

					valueM = selectedObj->mMat->getKd();
					_value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Kd_color = _value;

					valueM = selectedObj->mMat->getKs();
					_value = ImVec4(valueM[0], valueM[1], valueM[2], 1.0);
					Ks_color = _value;

					static float shiness;
					shiness = selectedObj->mMat->getShiness();

					ImGui::SliderFloat("Shiness", &shiness, 0, 32);
					ImGui::ColorEdit4("Ka", &Ka_color.x, ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
					ImGui::ColorEdit4("Kd", &Kd_color.x, ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
					ImGui::ColorEdit4("Ks", &Ks_color.x, ImGuiColorEditFlags_NoInputs);

					float* valueKa = new float[3] {Ka_color.x, Ka_color.y, Ka_color.z};
					float* valueKd = new float[3] {Kd_color.x, Kd_color.y, Kd_color.z};
					float* valueKs = new float[3] {Ks_color.x, Ks_color.y, Ks_color.z};
					selectedObj->mMat->setShaderValue(valueKa, valueKd, valueKs, shiness);

				}
			}
		}

		ImGui::End();
	}

	static void ShowProjectOverlay(bool* p_opent)
	{
		ImGui::SetNextWindowPos(ImVec2(0, windowHeight - projectWindowHeight));
		ImGui::SetNextWindowBgAlpha(0.5f);
		ImGui::SetNextWindowSize(ImVec2(windowWidth, projectWindowHeight));
		//if (!ImGui::Begin("Project", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		if (!ImGui::Begin("Project", NULL, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::End();
			return;
		}

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		//static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Project"))
			{
				ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Debug"))
			{
				ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Animation"))
			{
				ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::Separator();


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

	static void ShowBackstageOverlay(bool* p_open, float deltaTime)
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

		win->DrawBackstageWindow(screenWidth, screenHeight, selectedObjID, deltaTime);

		ImGui::End();
	}
};