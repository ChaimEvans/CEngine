/**
 * @file Engine.ixx
 * @brief 引擎主模块
 * @version 1.0
 * @author Chaim
 * @date 2024/09/27
 */

module;
#include <ctime>
#include "glad/glad.h"
#include "glfw3.h"
export module CEngine.Engine;
import std;
import CEngine.Base;
import CEngine.Logger;
import CEngine.Event;
import CEngine.Node;
import CEngine.Render;
import CEngine.UI;

namespace CEngine {
    /**
    * @class Engine
    * @brief CEngine引擎类
    * @remark 单例！\n
    */
    export class Engine final : Object {
    public:
        const static char *TAG;
        /**
        * 获得引擎实例
        */
        static Engine *GetIns();
        /**
        * 创建窗口
        * @param width 窗口宽度
        * @param height 窗口高度
        * @param title 窗口标题
        * @return 窗口创建是否成功
        */
        bool NewWindow(int width, int height, const char *title);
        /**
        * 引擎主循环
        * @remark 堵塞型
        */
        int Loop();
        /**
        * 退出引擎主循环
        * @param code 退出代码，将会在<code>loop()</code>函数中返回
        * @remark 仅标记为退出，不会立即退出引擎
        */
        void Exit(int code);

        /// @property RootNode
        Node *GetRoot() const {
            return RootNode;
        }

        /// @property window
        GLFWwindow *GetWindow() const { return window; }

        /**
        * @brief 事件：窗口大小被改变
        * @param 0 GLFWwindow对象
        * @param 1 窗口宽度
        * @param 2 窗口高度
        */
        Event<void(GLFWwindow *, int, int)> Event_WindowResized;
        /**
         *  @brief 事件：引擎准备就绪
         */
        Event<void()> Event_Ready;
        /**
         * @brief 事件：引擎帧处理
         * @param 0 上一帧处理用时(ms)
        */
        Event<void(double)> Event_Process;

    private:
        Engine();
        /// @brief 引擎实例对象指针
        static Engine *Ins;
        /// @brief 窗口对象指针<code>GLFWwindow</code>
        GLFWwindow *window;
        /// @brief 节点根目录
        Node *RootNode = Node::Create();
        /// @brief 临时存储退出代码，用于<code>loop()</code>函数中返回
        int ExitCode = 0;

        /**
        * 当引擎准备就绪时
        */
        void Ready();
        /**
        * 引擎帧处理函数
        * @param DeltaTime 上一帧处理用时(ms)
        */
        void Process(double DeltaTime);
    };

    const char *Engine::TAG = "引擎";
    Engine *Engine::Ins = nullptr;

    Engine::Engine() {
        Ins = this;
        window = nullptr;
        glfwInit();
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);

        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        RootNode->setName("RootNode");
    }

    Engine *Engine::GetIns() {
        if (Ins->IsValid())
            return Ins;
        Ins = new Engine();
        return Ins;
    }

    bool Engine::NewWindow(const int width, const int height, const char *title) {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
            LogE(TAG) << "创建窗口失败!";
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0); // 关闭垂直同步
        LogS(TAG) << "窗口创建成功.";

        UI::InitUI(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            LogE(TAG) << "GLAD加载失败!";
            glfwTerminate();
            return false;
        }
        LogS(TAG) << "GLAD加载成功.";
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *_window, const int _width, int _height) {
            glViewport(0, 0, _width, _height);
            LogI(TAG) << "设置Viewport: " << _width << "x" << _height;
            GetIns()->Event_WindowResized.Invoke(_window, _width, _height);
        });
        return true;
    }

    int Engine::Loop() {
        Ready();
        double last_clock = clock();
        while (!glfwWindowShouldClose(window)) {
            const double now_clock = clock();
            Process((now_clock - last_clock) / CLOCKS_PER_SEC * 1000);
            last_clock = now_clock;
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwTerminate();
        delete this;
        return ExitCode;
    }

    void Engine::Exit(const int code) {
        ExitCode = code;
        glfwSetWindowShouldClose(window, true);
    }

    void Engine::Ready() {
        GLint maxUniformLocations;
        glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);
        LogI(TAG) << "当前设备最大Uniform数量: " << maxUniformLocations;
        glEnable(GL_CULL_FACE);
        ShaderProgram::LoadPresets();
        Event_Ready.Invoke();
    }

    void Engine::Process(const double DeltaTime) {
        Event_Process.Invoke(DeltaTime);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        std::stack<Node *> stack;
        stack.push(RootNode);
        while (!stack.empty()) {
            Node *node = stack.top();
            stack.pop();
            if (node->GetChildCount() > 0)
                for (const auto child: node->GetChildren())
                    stack.push(child);
            if (const auto ru3d = dynamic_cast<RenderUnit3D *>(node); ru3d != nullptr)
                ru3d->Render();
        }
        UI::ProcessUI();
    }
}
