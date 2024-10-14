/**
 * @file UI.ixx
 * @brief UI总模块
 * @version 1.0
 * @author Chaim
 * @date 2024/10/14
 */

module;
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
export module CEngine.UI;
import CEngine.Logger;


namespace CEngine::UI {
    auto TAG = "UI";
    /**
     * 初始化窗口（在创建GLFWwindow之后）
     * @param window 窗口句柄
     */
    export void InitUI(GLFWwindow* window) {
        LogI(TAG) << "初始化UI...";
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    bool show_demo_window = true;

    /**
     * 刷新显示UI（在Process中）
     */
    export void ProcessUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
