/**
* @file Node3D.ixx
 * @brief 3D节点类
 * @version 1.0
 * @author Chaim
 * @date 2024/10/07
 */

module;
#include <glm/glm.hpp>
export module CEngine.Node:Node3D;
import :Node;

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

        virtual void setWorldMatrix(const glm::mat4 &m) {
            ModelMatrix *= m * glm::inverse(getWorldMatrix());
        }

        /**
         * 获取世界变换矩阵
         * @return 世界变换矩阵
         */
        glm::mat4 getWorldMatrix() const {
            auto matrix = ModelMatrix;
            auto parent = Parent;
            while (parent != nullptr) {
                if (const auto n3d = dynamic_cast<Node3D *>(parent); n3d != nullptr) {
                    matrix = n3d->getWorldMatrix() * matrix;
                }
                parent = parent->getParent();
            }
            return matrix;
        }

        /// @property ModelMatrix
        virtual void setModelMatrix(const glm::mat4 &m) {
            ModelMatrix = m;
        }

        /// @property ModelMatrix
        glm::mat4 getModelMatrix() const {
            return ModelMatrix;
        }

    protected:
        Node3D() = default;
        /// @brief 局部变换矩阵
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
    };
}
