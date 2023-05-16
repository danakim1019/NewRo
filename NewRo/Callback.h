#pragma once

#include "ImGUI/imgui.h"
#include"ImGUI/imgui_internal.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include"ImGUI/ImGuizmo.h"

#include"BackstageWindow.h"

static int windowWidth = 1500;              //전체 창
static int windowHeight = 800;              //전체 창
static int screenWidth = 800;               //백스테이지 윈도우
static int screenHeight = 600;              //백스테이지 윈도우
static int inspectorWidth = 300;                //하이어라키 윈도우
static int hierachyWidth = 200;                 //하이어라키 윈도우
static int projectWindowHeight = 200;       //프로젝트 윈도우

static float worldTransform[3] = { 0.f, 0.f, 0.f };
static float rotation[3] = { 0.f, 0.f, 0.f };
static float scale[3] = { 0.f, 0.f, 0.f };

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

static double _frame_start_time;
static double _delta_time;
static double _animation_time;
static double _window_start_time;


static const char* objName;
static OBJect* selectedObj;
static unsigned int selectedObjID;
static char str0[32]{ " ", };

BackstageWindow* win;

class Callback {
public:

	Callback() {
		selectedObjID = -1;
		_frame_start_time = 0;
		_delta_time = 0;
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		if (ImGui::TempInputIsActive(ImGui::GetActiveID())) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				std::cout << "QQQQ" << std::endl;
				win->mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			}
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				win->mCurrentGizmoOperation = ImGuizmo::SCALE;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				if (selectedObj != NULL || win->mCurrentGizmoOperation != ImGuizmo::ROTATE)
				{
					win->mCurrentGizmoOperation = ImGuizmo::ROTATE;
					win->mCurrentGizmoMode = ImGuizmo::LOCAL;
				}
				else {
					win->cam.ProcessKeyboard(FORWARD, _delta_time);
				}
			}
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
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
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
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		//std::cout << "ImGui::GetItemFlags():"<<ImGui::GetItemFlags() << std::endl;

		if (action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

		}

		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				if ((xPos > hierachyWidth && xPos < hierachyWidth + screenWidth) && (yPos > 20 && yPos < screenHeight))
				{
					if (xPos < hierachyWidth + screenWidth - 128 && yPos>128)
					{
						unsigned int select = win->selectObject(xPos, yPos, selectedObjID);

						if ((ImGuizmo::IsOver() == true))
						{
							if (selectedObjID < 1 && select>0) {
								selectedObjID = select;
								selectedObj = win->getObject(selectedObjID);
							}
						}
						else {
							if (select > 0) {
								selectedObjID = select;
								selectedObj = win->getObject(selectedObjID);
							}
							else {
								selectedObjID = 0;
								selectedObj = NULL;
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
		//win->DrawBackstageWindow(screenWidth, screenHeight,selectedObjID);
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
		win->SetViewport(width, height);

	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
		hierachyWidth = windowHeight * 0.3f;
		screenWidth = windowWidth - inspectorWidth - hierachyWidth;
		screenHeight = windowHeight - projectWindowHeight;
		win->SetWindowSize(screenWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), hierachyWidth, projectWindowHeight, width, height);
	}

};