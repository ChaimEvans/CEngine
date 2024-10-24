# CEngine

基于OpenGL的3D引擎（未完成）

C++
```c++
import CEngine.Engine;
import CEngine.UI.EditorUI;

int main(){
    const auto engine = CEngine::Engine::GetIns();
    engine->NewWindow(1920, 1080, "title");
    engine->LoadAllPresets();
    engine->setUI(new CEngine::EditorUI());
    engine->AddFlyCamera3DWhenReady = true;
    engine->Loop();
    return 0;
}
```
CMake
```cmake
cmake_minimum_required(VERSION 3.29)
project(xxx)
set(CMAKE_CXX_STANDARD 23)
include(CEngine/CEngine.cmake)
add_executable(xxx main.cpp)
target_link_libraries(
        xxx
        Engine
        EditorUI
)
```