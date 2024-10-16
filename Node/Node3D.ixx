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

        virtual void SetWorldMatrix(const glm::mat4 &m) {
            ModelMatrix *= m * glm::inverse(GetWorldMatrix());
        }

        /// @property ModelMatrix
        virtual void SetModelMatrix(const glm::mat4 &m) {
            ModelMatrix = m;
        }

        /// @property ModelMatrix
        glm::mat4 GetModelMatrix() const {
            return ModelMatrix;
        }

        /**
         * 获取局部位置
         */
        glm::vec3 GetPosition() const {
            return Utils::Matrix4GetPosition(ModelMatrix);
        }

        void SetPosition(const glm::vec3 &pos) {
            ModelMatrix = glm::translate(ModelMatrix, pos - GetPosition());
        }

        /**
         * 获取世界位置
         */
        glm::vec3 GetWorldPosition() const {
            return Utils::Matrix4GetPosition(GetWorldMatrix());
        }

        glm::vec3 GetScale() const {
            return Utils::Matrix4GetScale(ModelMatrix);
        }

        void SetScale(const glm::vec3 &s) {
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f) + s - GetScale());
        }

        glm::vec3 GetWorldScale() const {
            return Utils::Matrix4GetScale(GetWorldMatrix());
        }

        glm::quat GetRotation() const {
            return Utils::Matrix4GetRotationAngles(ModelMatrix);
        }

        void SetRotation(const glm::vec3 &r) {
            const glm::vec3 raw_r = GetRotationAngles();
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians(r.x - raw_r.x), glm::vec3(1.0f, 0.0f, 0.0f));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians(r.y - raw_r.y), glm::vec3(0.0f, 1.0f, 0.0f));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians(r.z - raw_r.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        glm::quat GetWorldRotation() const {
            return Utils::Matrix4GetRotationAngles(GetWorldMatrix());
        }

        glm::vec3 GetRotationAngles() const {
            return Utils::Matrix4GetRotationAngles(ModelMatrix);
        }

        glm::vec3 GetWorldRotationAngles() const {
            return Utils::Matrix4GetRotationAngles(GetWorldMatrix());
        }

    protected:
        Node3D() = default;
        /// @brief 局部变换矩阵
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
    };
}
