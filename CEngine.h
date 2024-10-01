#ifndef ENGINE_H
#define ENGINE_H

#include "Mesh/ObjMesh.h"
#include "Shader/Shader.h"
#include "glfw3.h"

namespace CEngine {
    class Engine {
    public:
        Engine();
        ~Engine();
        static GLFWframebuffersizefun frame_buffer_size_callback;
        GLFWwindow *NewWindow(int width, int height, const char *title);
        int Loop();
        void Process(double DeltaTime);
        void Render();
        void Exit(int code);
        static Engine *GetIns() { return Ins; }
        GLFWwindow *GetWindow() const { return window; }

    private:
        const static char *TAG;
        static Engine *Ins;
        GLFWwindow *window;
        int ExitCode = 0;

        ObjMesh *test_mesh;
        Shader *test_shader;
    };
}
#endif // !ENGINE_H
