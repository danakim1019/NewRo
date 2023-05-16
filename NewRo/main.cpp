// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <stdio.h>

#include"3DObjects/stb_image.h"

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
	bool bShowDemoWindow = true;
	bool bShowAnotherWindow = false;
	bool bShowMenubarWindow = true;
	bool bShowHierachyWindow = true;
	bool bShowInspectorWindow = true;
	bool bShowProjectWindow = true;
	bool bShowBackstageWindow = true;

	Core() {
		bShowDemoWindow = true;
		bShowAnotherWindow = false;
		bShowMenubarWindow = true;
		bShowHierachyWindow = true;
		bShowInspectorWindow = true;
		bShowProjectWindow = true;
		bShowBackstageWindow = true;
	}

	void GUIRender(float deltaTime) {
		ShowMenuBarOverlay(&bShowMenubarWindow);

		if (bShowHierachyWindow) {
			ShowHierachyOverlay(&bShowHierachyWindow);
		}
		if (bShowInspectorWindow) {
			ShowInspectorOverlay(&bShowInspectorWindow);
		}
		if (bShowProjectWindow) {
			ShowProjectOverlay(&bShowProjectWindow);
		}

		if (bShowBackstageWindow) {
			ShowBackstageOverlay(&bShowBackstageWindow, deltaTime);
		}

		//if (show_demo_window)
		//     ImGui::ShowDemoWindow(&show_demo_window);
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
	GLFWimage images[1];
	images[0].pixels = stbi_load("icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);

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
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->ItemSpacing = ImVec2(6, 8);

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
	win = new BackstageWindow(backstageSize.x, backstageSize.y, windowWidth, windowHeight);
	screenWidth = windowWidth - inspectorWidth - hierachyWidth;
	screenHeight = windowHeight - projectWindowHeight;
	win->SetWindowSize(screenWidth, screenHeight - ((ImGui::GetStyle().FramePadding.y * 2) + 18), hierachyWidth, projectWindowHeight, windowWidth, windowHeight);

	_window_start_time = glfwGetTime();
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		_delta_time = glfwGetTime() - _frame_start_time;
		_frame_start_time = glfwGetTime();
		_animation_time = (glfwGetTime() - _window_start_time) / 50.0f;
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


		core.GUIRender(_animation_time);

		// Rendering
		ImGui::Render();
		int displayWidth, displayHeight;
		glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

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