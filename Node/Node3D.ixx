/**
* @file Node3D.ixx
 * @brief 3D节点类
 * @version 1.0
 * @author Chaim
 * @date 2024/10/07
 */

module;
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
export module CEngine.Node:Node3D;
import :Node;
import CEngine.Utils;

namespace CEngine {
    /**
     * @brief 3D节点类
     * @remark 仅实现3D变换功能
     */
    export class Node3D : public Node {
    public:
        static Node3D *Create() {
            return new Node3D();
        }

        const char *GetTypeName() override {
            return "Node3D";
        }

        Node3D &SetPosition(const glm::vec3 p, const bool updateM = true) {
            Position = p;
            if (updateM) UpdateModelMatrix();
            return *this;
        }

        Node3D &SetRotation(const EulerRotation e, const bool updateM = true) {
            Rotation = e;
            if (updateM) UpdateModelMatrix();
            return *this;
        }

        Node3D &SetScale(const glm::vec3 s, const bool updateM = true) {
            Scale = s;
            if (updateM) UpdateModelMatrix();
            return *this;
        }

        Node3D &UpdateModelMatrix() {
            // 平移矩阵 * 旋转矩阵 * 缩放矩阵
            ModelMatrix = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(Rotation.ToOrientation()) * glm::scale(glm::mat4(1.0f), Scale);
            return *this;
        }

        void SetModelMatrix(const glm::mat4 &matrix) {
            Position = glm::vec3(GetWorldMatrix()[3]);
            Scale = {
                glm::length(glm::vec3(matrix[0])),
                glm::length(glm::vec3(matrix[1])),
                glm::length(glm::vec3(matrix[2])),
            };
            glm::mat4 rotationM = matrix;
            rotationM[0] /= Scale.x;
            rotationM[1] /= Scale.y;
            rotationM[2] /= Scale.z;
            const glm::vec3 rotationEuler = glm::eulerAngles(glm::quat_cast(rotationM));
            Rotation = {rotationEuler.y, rotationEuler.x, rotationEuler.z};
            UpdateModelMatrix();
        }

        glm::vec3 GetPosition() const { return Position; }
        EulerRotation GetRotation() const { return Rotation; }
        glm::vec3 GetScale() const { return Scale; }
        glm::mat4 GetModelMatrix() const { return ModelMatrix; }

        /**
         * 获取世界变换矩阵
         * @return 世界变换矩阵
         */
        glm::mat4 GetWorldMatrix() const {
            auto matrix = ModelMatrix;
            auto parent = Parent;
            while (parent != nullptr) {
                if (const auto n3d = dynamic_cast<Node3D *>(parent); n3d != nullptr) {
                    matrix = n3d->GetWorldMatrix() * matrix;
                }
                parent = parent->getParent();
            }
            return matrix;
        }

        glm::vec3 GetWorldPosition() const {
            return glm::vec3(GetWorldMatrix()[3]);
        }

        EulerRotation GetWorldRotation() const {
            glm::mat4 matrix = GetWorldMatrix();
            const glm::vec3 scale = {
                glm::length(glm::vec3(matrix[0])),
                glm::length(glm::vec3(matrix[1])),
                glm::length(glm::vec3(matrix[2])),
            };
            glm::mat4 rotationM = matrix;
            rotationM[0] /= scale.x;
            rotationM[1] /= scale.y;
            rotationM[2] /= scale.z;
            const glm::vec3 rotationEuler = glm::eulerAngles(glm::quat_cast(rotationM));
            return {rotationEuler.y, rotationEuler.x, rotationEuler.z};
        }

        glm::vec3 GetWorldScale() const {
            glm::mat4 matrix = GetWorldMatrix();
            return {
                glm::length(glm::vec3(matrix[0])),
                glm::length(glm::vec3(matrix[1])),
                glm::length(glm::vec3(matrix[2])),
            };
        }

        glm::vec3 GetForward(const bool world = false) const {
            if (world)
                return glm::normalize(GetWorldRotation().ToOrientation() * glm::vec3(0.0f, 0.0f, 1.0f));
            return glm::normalize(Rotation.ToOrientation() * glm::vec3(0.0f, 0.0f, 1.0f));
        }

        glm::vec3 GetRight(const bool world = false) const {
            return glm::normalize(glm::cross(GetForward(world), glm::vec3(0.0f, 0.0f, 1.0f)));
        }

        glm::vec3 GetUp(const bool world = false) const {
            return glm::normalize(glm::cross(GetRight(world), GetForward(world)));
        }

    protected:
        Node3D() = default;
        /// 局部变换矩阵
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        /// 坐标
        glm::vec3 Position = glm::vec3(0.0f);
        /// 旋转值
        EulerRotation Rotation = EulerRotation(0.0f, 0.0f, 0.0f);
        /// 缩放值
        glm::vec3 Scale = glm::vec3(1.0f);
    };
}
