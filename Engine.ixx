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
        void Loop();
        /**
         * 每绘制一次调用(即Mesh.Render函数后)
         */
        void DrawCallEnd();
        /**
        * 退出引擎主循环
        * @remark 仅标记为退出，不会立即退出引擎
        */
        void Exit() const;

        /// @property RootNode
        Node3D *getRoot() const { return RootNode; }

        /// @property window
        GLFWwindow *getWindow() const { return window; }

        /// @property ui
        void setUI(UI *u) {
            if (!ui->IsValid())
                u->InitUI();
            ui = u;
        }

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
        /**
         *  @brief 事件：引擎退出时
         */
        Event<void()> Event_Destroy;

    private:
        Engine();
        /// @brief 引擎实例对象指针
        static Engine *Ins;
        /// @brief 窗口对象指针<code>GLFWwindow</code>
        GLFWwindow *window;
        /// @brief UI
        UI *ui;
        /// @brief 节点根目录
        Node3D *RootNode = Node3D::Create();

        /**
        * 当引擎准备就绪时
        */
        void Ready();
        /**
        * 引擎帧处理函数
        * @param DeltaTime 上一帧处理用时(ms)
        */
        void Process(double DeltaTime);
        /**
        * 当引擎退出时
        */
        void Destroy();
    };

    const char *Engine::TAG = "引擎";
    Engine *Engine::Ins = nullptr;

    Engine::Engine() {
        Ins = this;
        window = nullptr;
        glfwInit();
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);
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

        setUI(new UI(window));

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

    void Engine::Loop() {
        Ready();
        double last_clock = clock();
        while (!glfwWindowShouldClose(window)) {
            const double now_clock = clock();
            Process((now_clock - last_clock) / CLOCKS_PER_SEC * 1000);
            last_clock = now_clock;
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        Destroy();
    }

    void Engine::DrawCallEnd() {
        Texture::ResetTextureSlot();
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glUseProgram(0);
    }

    void Engine::Exit() const {
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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        Texture::ResetTextureSlot();
        std::stack<Node *> stack;
        stack.push(RootNode);
        while (!stack.empty()) {
            Node *node = stack.top();
            stack.pop();
            if (node->GetChildCount() > 0)
                for (const auto child: node->GetChildren())
                    stack.push(child);
            if (const auto ru3d = dynamic_cast<RenderUnit3D *>(node); ru3d != nullptr) {
                ru3d->Render();
                DrawCallEnd();
            }
        }
        ui->ProcessUI();
    }

    void Engine::Destroy() {
        Event_Destroy.Invoke();
        glfwDestroyWindow(window);
        glfwTerminate();
        delete RootNode;
        delete ui;
        UI::Destroy();
        delete this;
    }
}
