/**
 * @file SceneTreeBrowser.ixx
 * @brief 场景树浏览器
 * @version 1.0
 * @author Chaim
 * @date 2024/10/16
 */

module;
#include "imgui/imgui.h"
#include <vcruntime_typeinfo.h>
export module CEngine.UI.EditorUI:SceneTreeBrowser;
import std;
import CEngine.Engine;
import CEngine.Node;

namespace CEngine {
    export class SceneTreeBrowser {
    public:
        SceneTreeBrowser() = default;

        void RefreshCache() {
            NodeIsFolding.clear();
        }

        void DisplaySceneTreeBrowser() {
            std::stack<std::pair<Node *, int> > stack;
            stack.push({Engine::GetIns()->getRoot(), 0});
            while (!stack.empty()) {
                auto [node, tab] = stack.top();
                stack.pop();
                if (!NodeIsFolding.contains(node)) {
                    NodeIsFolding[node] = true;
                }
                std::string node_name;
                if (node->GetChildCount() > 0) {
                    node_name = std::format("{}> ({}) {}", std::string(tab, ' '), node->GetTypeName(), node->getName());
                    if (NodeIsFolding[node])
                        for (auto child: node->GetChildren())
                            stack.push({child, tab + 2});
                } else {
                    node_name = std::format("{}({}) {}", std::string(tab, ' '), node->GetTypeName(), node->getName());
                }
                if (ImGui::Selectable(node_name.c_str(), node == NodeSelected)) {
                    NodeSelected = node;
                    NodeIsFolding[node] = !NodeIsFolding[node];
                }
            }
        }

        Node *getSelectedNode() const {
            return NodeSelected;
        };

    private:
        Node *NodeSelected = nullptr;
        std::unordered_map<Node *, bool> NodeIsFolding;
    };
}
