# glad库
include_directories("CEngine/ThirdParty/GLAD/include")
add_library(glad STATIC "CEngine/ThirdParty/GLAD/src/glad.c")

# GLFW库
include_directories("CEngine/ThirdParty/GLFW/include")
set(glfw3 "glfw")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    #    set(glfw3 "${CMAKE_CURRENT_SOURCE_DIR}/Engine/ThirdParty/GLFW/lib-vc2022/glfw3.lib")
    set(glfw3 "${CMAKE_CURRENT_SOURCE_DIR}/CEngine/ThirdParty/GLFW/lib-mingw-w64/glfw3.dll")
endif ()

# 引擎各组件
include_directories("CEngine")
include_directories("CEngine/ThirdParty")
# 工具
add_library(Utils STATIC "CEngine/Utils/Utils.cpp" "CEngine/Utils/Event.cpp")
# 日志
add_library(Logger STATIC "CEngine/Logger/Logger.cpp")
# 图像
add_library(Image STATIC "CEngine/Image/Pixel.cpp" "CEngine/Image/Image.cpp" "CEngine/Image/ImageBuffer.cpp")
target_link_libraries(
        Image
        Logger
        Utils
)
# 纹理
add_library(Texture STATIC "CEngine/Texture/Texture.cpp")
target_link_libraries(
        Texture
        glad
        ${glfw3}
        Image
        Logger
)
# 着色器
add_library(Shader STATIC "CEngine/Shader/Shader.cpp")
target_link_libraries(
        Shader
        Engine
        glad
        ${glfw3}
        Logger
)
# 网格体
add_library(ObjMesh STATIC "CEngine/Mesh/ObjMesh.cpp")
target_link_libraries(
        ObjMesh
        glad
        Shader
        Logger
)
# 引擎
add_library(Engine STATIC "CEngine/CEngine.cpp")
target_link_libraries(
        Engine
        glad
        ${glfw3}
        Shader
        ObjMesh
        Texture
        Logger
        Utils
)