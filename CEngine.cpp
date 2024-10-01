#include <glad/glad.h>
#include "CEngine.h"
#include <iostream>
#include "Logger/Logger.h"

namespace CEngine {
    const char *Engine::TAG = "引擎";
    Engine *Engine::Ins = nullptr;

    GLFWframebuffersizefun Engine::frame_buffer_size_callback = [](GLFWwindow *win, int width, int height) {
        glViewport(0, 0, width, height);
        LogI << "设置Viewport: " << width << "x" << height;
    };

    Engine::Engine() {
        Ins = this;
        window = nullptr;
        glfwInit();
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        test_mesh = new ObjMesh(R"(D:\Code\CPP\OpenGL_Learning_CMake\Engine\Mesh\Presets\Cube.obj)");
        test_shader = new Shader(R"(D:\Code\CPP\OpenGL_Learning_CMake\Engine\Shader\Presets\Unlit\vertex.glsl)",
                                 R"(D:\Code\CPP\OpenGL_Learning_CMake\Engine\Shader\Presets\Unlit\fragment.glsl)");
    }

    Engine::~Engine() = default;

    GLFWwindow *Engine::NewWindow(const int width, const int height, const char *title) {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
            LogE << "创建窗口失败!";
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        LogS << "窗口创建成功.";

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            LogE << "GLAD加载失败!";
            glfwTerminate();
            return nullptr;
        }
        LogS << "GLAD加载成功.";
        glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
        return window;
    }

    int Engine::Loop() {
        double last_clock = clock();
        while (!glfwWindowShouldClose(window)) {
            const double now_clock = clock();
            Process((now_clock - last_clock) / CLOCKS_PER_SEC * 1000);
            last_clock = now_clock;
            Render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwTerminate();
        return ExitCode;
    }

    void Engine::Process(double DeltaTime) {
    }

    void Engine::Render() {
        // test_mesh->Render(test_shader);
    }

    void Engine::Exit(const int code) {
        ExitCode = code;
        glfwSetWindowShouldClose(window, true);
    }
}
