#ifndef OBJMESH_H
#define OBJMESH_H
#include <vector>
#include <glm/glm.hpp>
#include "Shader/Shader.h"

namespace CEngine {
    class ObjMesh {
    public:
        // 构造函数
        explicit ObjMesh(const char *file_path);

        // 加载模型数据至GPU
        void SendToGPU();

        // 渲染函数
        void Render(const Shader *shader) const;

        // 析构函数，释放OpenGL资源
        ~ObjMesh();

    private:
        std::vector<glm::vec3> vertices; // 顶点位置
        std::vector<glm::vec3> normals; // 法线
        std::vector<glm::vec2> texCoords; // 纹理坐标

        unsigned int VAO = 0;
        unsigned int VBO = 0;

        const static char *TAG;
    };
}
#endif //OBJMESH_H
