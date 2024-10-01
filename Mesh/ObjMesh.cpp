#include "ObjMesh.h"
#include "glad/glad.h"
#include "Logger/Logger.h"
#include <fstream>
#include <sstream>

namespace CEngine {
    const char *ObjMesh::TAG = "ObjMesh";

    ObjMesh::ObjMesh(const char *file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            LogE << "无法打开文件: " << file_path;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                // 顶点位置
                glm::vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            } else if (prefix == "vn") {
                // 法线
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            } else if (prefix == "vt") {
                // 纹理坐标
                glm::vec2 texCoord;
                iss >> texCoord.x >> texCoord.y;
                texCoords.push_back(texCoord);
            }
        }
        file.close();
    }

    ObjMesh::~ObjMesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void ObjMesh::SendToGPU() {
        // 创建并绑定VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // 创建VBO并绑定
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // 将顶点数据上传到GPU
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
        // 解绑VAO和VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void ObjMesh::Render(const Shader *shader) const {
        glBindVertexArray(VAO);
        shader->Use();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        shader->UnUse();
        glBindVertexArray(0);
    }
}
