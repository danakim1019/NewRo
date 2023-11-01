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

#include "ImGUI/imgui.h"
#include"ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include"ImGUI/ImGuizmo.h"

#include"BackstageWindow.h"

static int st_windowWidth = 1500;              //Entire Window
static int st_windowHeight = 800;              //Entire Window
static int st_screenWidth = 800;               //Backstage Window
static int st_screenHeight = 600;              //Backstage Window
static int st_inspectorWidth = 300;                //Hierarchy Window
static int st_hierachyWidth = 200;                 //Hierarchy Window
static int st_projectWindowHeight = 200;       //Project Window

static float st_worldTransform[3] = { 0.f, 0.f, 0.f };
static float st_rotation[3] = { 0.f, 0.f, 0.f };
static float st_scale[3] = { 0.f, 0.f, 0.f };

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

static double st_frame_start_time;
static double st_delta_time;
static double st_animation_time;
static double st_window_start_time;

static const char* st_objName;
static OBJect* st_selectedObj;
static unsigned int st_selectedObjID;
static char st_str0[32]{ " ", };

BackstageWindow* backStageWin;

class Callback {
public:

	Callback() {
		st_selectedObjID = -1;
		st_frame_start_time = 0;
		st_delta_time = 0;
	}
	~Callback(){}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		if (ImGui::TempInputIsActive(ImGui::GetActiveID())) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				backStageWin->mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				backStageWin->mCurrentGizmoOperation = ImGuizmo::SCALE;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				if (st_selectedObj != NULL || backStageWin->mCurrentGizmoOperation != ImGuizmo::ROTATE)
				{
					backStageWin->mCurrentGizmoOperation = ImGuizmo::ROTATE;
					backStageWin->mCurrentGizmoMode = ImGuizmo::LOCAL;
				}
				else {
					backStageWin->mCam.ProcessKeyboard(FORWARD, st_delta_time);
				}
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				backStageWin->mCam.ProcessKeyboard(BACKWARD, st_delta_time);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				backStageWin->mCam.ProcessKeyboard(LEFT, st_delta_time);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				backStageWin->mCam.ProcessKeyboard(RIGHT, st_delta_time);
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
				backStageWin->mCam.ProcessKeyboard(UPPER, st_delta_time);
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
				backStageWin->mCam.ProcessKeyboard(DOWN, st_delta_time);
		}
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		backStageWin->mCam.ProcessMouseScroll(yoffset, st_delta_time);
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

			backStageWin->mCam.ProcessMouseMovement(xoffset, yoffset, st_delta_time);
		}
		if (isMiddleMouseClicked) {
			float xoffset = xPos - lastPosX;
			float yoffset = yPos - lastPosY; // reversed since y-coordinates go from bottom to top

			backStageWin->mCam.cameraPositionMove(xoffset, yoffset, st_delta_time);
		}
	}

	// glfw: whenever the mouse moves, this callback is called
	// mouse right click : camera rotation
	// -------------------------------------------------------
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		if (action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				if ((xPos > st_hierachyWidth && xPos < st_hierachyWidth + st_screenWidth) && (yPos > 20 && yPos < st_screenHeight))
				{
					if (xPos < st_hierachyWidth + st_screenWidth - 128 && yPos>128)
					{
						unsigned int select = backStageWin->SelectObject(xPos, yPos, st_selectedObjID);

						if ((ImGuizmo::IsOver() == true))
						{
							if (st_selectedObjID < 1 && select>0) {
								st_selectedObjID = select;
								st_selectedObj = backStageWin->getObject(st_selectedObjID);
							}
						}
						else {
							if (select > 0) {
								st_selectedObjID = select;
								st_selectedObj = backStageWin->getObject(st_selectedObjID);
							}
							else {
								st_selectedObjID = 0;
								st_selectedObj = NULL;
								printf("not picking\n");
							}
						}
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

		//Catch Mouse Wheel
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (action == GLFW_PRESS) {
				//Move Cam position
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

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		backStageWin->SetViewport(width, height);
	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		st_windowWidth = width;
		st_windowHeight = height;
		st_hierachyWidth = st_windowHeight * 0.3f;
		st_screenWidth = st_windowWidth - st_inspectorWidth - st_hierachyWidth;
		st_screenHeight = st_windowHeight - st_projectWindowHeight;
		backStageWin->SetWindowSize(st_screenWidth, st_screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), st_hierachyWidth, st_projectWindowHeight, width, height);
	}

};