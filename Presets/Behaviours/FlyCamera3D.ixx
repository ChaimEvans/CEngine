/**
 * @file FlyCamera3D.ixx
 * @brief 飞行相机
 * @version 1.0
 * @author Chaim
 * @date 2024/10/21
 */

module;
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
export module CEngine.Presets.Behaviours:FlyCamera3D;
import std;
import CEngine.Engine;
import CEngine.Node;

namespace CEngine {
    export class FlyCamera3D final : public Behaviour {
    public:
        FlyCamera3D() = default;

        void Ready() override {
            p3d = dynamic_cast<Camera3D *>(ParentNode);
            if (p3d == nullptr) {
                ParentNode->SetBehaviour(nullptr);
            }
        }

        void Update(const double DeltaTime) override {
            const auto window = Engine::GetIns()->getWindow();
            // Define the new float variable to hold the converted deltaTime value.
            const auto delta = static_cast<float>(DeltaTime);

            // Move the camera in the forward direction at a specified speed.
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() + p3d->GetForward() * delta * speed);
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() - p3d->GetForward() * delta * speed);
            }

            // Move the camera in the left direction at a specified speed.
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() + p3d->GetRight() * delta * speed);
            }
            // Move the camera in the right direction at a specified speed.
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() - p3d->GetRight() * delta * speed);
            }
            // Move the camera in the up direction at a specified speed.
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() + p3d->GetUp() * delta * speed);
            }
            // Move the camera in the down direction at a specified speed.
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                p3d->SetPosition(p3d->GetPosition() - p3d->GetUp() * delta * speed);
            }
            // Check if middle mouse button is pressed
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                // Get the current mouse cursor position
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                if (!(last_x == 0 && last_y == 0)) {
                    const auto [w, h] = Engine::GetIns()->GetScreenSize();
                    // Calculate the rotation angle based on the mouse movement, using width and height to normalize the delta_x value
                    const auto delta_x = static_cast<float>((x - last_x) * sensitivity);
                    const auto delta_y = static_cast<float>((last_y - y) * sensitivity); // Invert y-axis to match OpenGL conventions.
                    const auto rotation = p3d->GetRotationPtr();
                    rotation->Yaw += delta_x;
                    // Clamp the pitch between -90 and 90 degrees to prevent flipping.
                    rotation->Pitch = std::clamp(rotation->Pitch + delta_y, -glm::pi<float>() / 2.0f, glm::pi<float>() / 2.0f);
                    p3d->UpdateModelMatrix();
                }
                last_x = x;
                last_y = y;
            } else {
                last_x = last_y = 0; // Reset the mouse position when middle button is released to avoid sudden jump.
            }
        }

    private:
        Camera3D *p3d = nullptr;
        float speed = 0.001f;
        float sensitivity = 0.0005f;
        double last_x = 0, last_y = 0;
    };

    void Engine::AddFlyCamera3D() const {
        const auto camera = Camera3D::Create();
        camera->SetBehaviour(std::make_unique<FlyCamera3D>());
        ToolNode->AddChild(camera);
        camera->Active();
    }
}
