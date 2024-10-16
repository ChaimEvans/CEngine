/**
 * @file Inspector.ixx
 * @brief 编辑器UI
 * @version 1.0
 * @author Chaim
 * @date 2024/10/16
 */

module;
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
export module CEngine.UI.EditorUI:Inspector;
import :SceneTreeBrowser;
import std;
import CEngine.Node;
import CEngine.Logger;
import CEngine.Utils;

namespace CEngine {
    export void DisplayInspector(const SceneTreeBrowser &scene_tree_browser) {
        auto node = scene_tree_browser.getSelectedNode();
        if (node == nullptr || !node->IsValid()) {
            ImGui::Text("Select A Node First");
            return;
        }
        if (ImGui::CollapsingHeader("Base", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::InputText("Type", const_cast<char *>(node->GetTypeName()), 16, ImGuiInputTextFlags_ReadOnly);
            ImGui::Separator();
            char Name[128];
            const auto name_str = node->getName();
            name_str.copy(Name, name_str.length());
            Name[name_str.length()] = '\0';
            if (ImGui::InputText("Name", Name, IM_ARRAYSIZE(Name), ImGuiInputTextFlags_EnterReturnsTrue))
                node->setName(Name);
        }
        auto node3d = dynamic_cast<Node3D *>(node);
        if (node3d == nullptr || !node3d->IsValid()) return;
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SeparatorText("Model");
            float pos_l[3];
            Utils::vec3_to_float3(node3d->GetPosition(), pos_l);
            if (ImGui::DragFloat3("Position (L)", pos_l, 0.01f)) {
                node3d->SetPosition(glm::vec3(pos_l[0], pos_l[1], pos_l[2]));
            }
            float rot_l[3];
            Utils::vec3_to_float3(node3d->GetRotationAngles(), rot_l);
            if (ImGui::DragFloat3("Rotation (L)", rot_l, 0.01f)) {
                node3d->SetRotation(glm::vec3(rot_l[0], rot_l[1], rot_l[2]));
            }
            float scale_l[3];
            Utils::vec3_to_float3(node3d->GetScale(), scale_l);
            if (ImGui::DragFloat3("Scale (L)", scale_l, 0.01f)) {
                node3d->SetScale(glm::vec3(scale_l[0], scale_l[1], scale_l[2]));
            }
            ImGui::SeparatorText("World");
            ImGui::DragFloat3("Position (W)", const_cast<float *>(glm::value_ptr(node3d->GetWorldPosition())));
            ImGui::DragFloat3("Rotation (W)", const_cast<float *>(glm::value_ptr(node3d->GetWorldRotationAngles())));
            ImGui::DragFloat3("Scale (W)", const_cast<float *>(glm::value_ptr(node3d->GetWorldScale())));
        }
    }
}
