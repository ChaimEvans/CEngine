/**
 * @file EditorUI.ixx
 * @brief 编辑器UI
 * @version 1.0
 * @author Chaim
 * @date 2024/10/16
 */

module;
#include <glfw3.h>
#include "imgui/imgui.h"
export module CEngine.UI.EditorUI;
import :FileBrowser;
import :SceneTreeBrowser;
import :Inspector;
import std;
import CEngine.UI;
import CEngine.Engine;
import CEngine.Logger;

namespace CEngine {
    export class EditorUI final : public UI {
    public:
        static const char *TAG;

        EditorUI(): UI(Engine::GetIns()->getWindow()) {
            file_browser.RefreshCache();
            scene_tree_browser.RefreshCache();
            Engine::GetIns()->Event_Process += [this](const double DeltaTime) {
                if (time_record > 1000) {
                    fps = frame_record / time_record * 1000;
                    frame_time = time_record / frame_record;
                    time_record = 0;
                    frame_record = 0;
                }
                time_record += DeltaTime;
                frame_record++;
            };
        }

        void DrawUI() override {
            int _window_width, _window_height;
            glfwGetWindowSize(Window, &_window_width, &_window_height);
            const auto window_width = static_cast<float>(_window_width);
            const auto window_height = static_cast<float>(_window_height);
            // Menu Bar
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    ImGui::MenuItem("New");
                    ImGui::MenuItem("Save");
                    ImGui::MenuItem("Open Directory");
                    if (ImGui::MenuItem("Refresh Directory"))
                        file_browser.RefreshCache();
                    ImGui::MenuItem("Exit");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit")) {
                    ImGui::MenuItem("Undo");
                    ImGui::MenuItem("Redo");
                    ImGui::MenuItem("Cut");
                    ImGui::MenuItem("Copy");
                    ImGui::MenuItem("Paste");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Scene")) {
                    if (ImGui::MenuItem("Refresh Scene"))
                        scene_tree_browser.RefreshCache();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Tools")) {
                    ImGui::MenuItem("Options");
                    ImGui::MenuItem("Settings");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("More")) {
                    ImGui::MenuItem("Help");
                    ImGui::MenuItem("About");
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            // 左Panel
            ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(window_width * 0.15f, window_height - 20 - 30), ImGuiCond_Once);
            if (ImGui::Begin("Left Panel", nullptr, ImGuiWindowFlags_NoMove)) {
                // 场景树
                if (ImGui::BeginChild("Scene Tree", ImVec2(0, ImGui::GetContentRegionAvail().y * 0.5f), ImGuiChildFlags_None)) {
                    if (ImGui::BeginTabBar("##tabs#Scene Tree")) {
                        if (ImGui::BeginTabItem("Scene Tree")) {
                            scene_tree_browser.DisplaySceneTreeBrowser();
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::EndChild();
                }

                // 文件浏览器
                if (ImGui::BeginChild("File Browser", ImVec2(0, 0), ImGuiChildFlags_None)) {
                    if (ImGui::BeginTabBar("##tabs#File Browser")) {
                        if (ImGui::BeginTabItem("File Browser")) {
                            file_browser.ShowDirectory();
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::EndChild();
                }
                ImGui::End();
            }

            // 右Panel
            ImGui::SetNextWindowPos(ImVec2(window_width * 0.85f, 20), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(window_width * 0.15f, window_height - 20 - 30), ImGuiCond_Once);
            if (ImGui::Begin("Right Panel", nullptr, ImGuiWindowFlags_NoMove)) {
                if (ImGui::BeginTabBar("##tabs#Inspector")) {
                    if (ImGui::BeginTabItem("Inspector")) {
                        DisplayInspector(scene_tree_browser);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::End();
            }

            // 底栏
            ImGui::SetNextWindowPos(ImVec2(0, window_height - 30), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(window_width, 30), ImGuiCond_Once);
            if (ImGui::Begin("Info Window", nullptr,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove)) {
                ImGui::Text(std::format(" FPS: {}  |  Render Time: {:.2f}ms", fps, frame_time).c_str());
                ImGui::End();
            }
        }

    private:
        int fps = 0, frame_record = 0;
        double frame_time = 0, time_record = 0;
        FileBrowser file_browser = FileBrowser(".");
        SceneTreeBrowser scene_tree_browser = SceneTreeBrowser();
    };

    const char *EditorUI::TAG = "EditorUI";
}
