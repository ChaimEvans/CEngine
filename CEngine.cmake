include_directories("CEngine/ThirdParty/include")

# glad库
include_directories("CEngine/ThirdParty/GLAD/include")
add_library(glad STATIC "CEngine/ThirdParty/GLAD/src/glad.c")

# GLFW库
set(glfw3 "glfw")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(glfw3 "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/lib/glfw3@3.4.lib")
endif ()

#assimp库
set(assimp "assimp")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(assimp "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/lib/assimp-vc143-mt@5.4.3.lib")
endif ()

# std模块（微软）
add_library(std_modules)
target_sources(
        std_modules
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/ThirdParty/modules/std/std.ixx
        CEngine/ThirdParty/modules/std/std.compat.ixx
)

# Base
add_library(Base)
target_sources(
        Base
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Base/Base.ixx
        CEngine/Base/Object.ixx
)
target_link_libraries(
        Base
        std_modules
)

# 日志类
add_library(Logger)
target_sources(
        Logger
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Utils/Logger.ixx
)
target_link_libraries(
        Logger
        std_modules
)

# 事件类
add_library(Event)
target_sources(
        Event
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Utils/Event.ixx
)
target_link_libraries(
        Event
        std_modules
        Base
)

# 渲染单位
add_library(Render)
target_sources(
        Render
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Render/Export.ixx
        CEngine/Render/GLSL.ixx
        CEngine/Render/ShaderProgram.ixx
        CEngine/Render/ShaderProgram_Presets.ixx
        CEngine/Render/Mesh.ixx
        CEngine/Render/Material.ixx
        CEngine/Render/Texture.ixx
)
target_link_libraries(
        Render
        std_modules
        Base
        Logger
        Utils
        Image
)

# 图像相关
add_library(Image)
target_sources(
        Image
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Image/Export.ixx
        CEngine/Image/Pixel.ixx
        CEngine/Image/ImageBuffer.ixx
        CEngine/Image/Image.ixx
)
target_link_libraries(
        Image
        std_modules
        glad
        Utils
        Logger
)

# Node系列
add_library(Node)
target_sources(
        Node
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Node/Export.ixx
        CEngine/Node/Node.ixx
        CEngine/Node/Node3D.ixx
        CEngine/Node/RenderUnit3D.ixx
        CEngine/Node/PBR3D.ixx
)
target_link_libraries(
        Node
        std_modules
        Base
        Logger
        Utils
        Render
)

# 模型导入器
add_library(ModelImporter)
target_sources(
        ModelImporter
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Utils/ModelImporter.ixx
)
target_link_libraries(
        ModelImporter
        Node
        Render
        ${assimp}
)

# 其他工具类
add_library(Utils)
target_sources(
        Utils
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Utils/Utils.ixx
)
target_link_libraries(
        Utils
        std_modules
)

# 引擎主模块
add_library(Engine)
target_sources(
        Engine
        PUBLIC
        FILE_SET CXX_MODULES FILES
        CEngine/Engine.ixx
)
target_link_libraries(
        Engine
        std_modules
        glad
        ${glfw3}
        Render
        Base
        Logger
        Event
        Node
)

# 构建后处理
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(TARGET Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/lib/glfw3@3.4.dll"
            "${CMAKE_BINARY_DIR}/glfw3.dll")

    add_custom_command(TARGET Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/lib/assimp-vc143-mt@5.4.3.dll"
            "${CMAKE_BINARY_DIR}/assimp-vc143-mt.dll")
endif ()

add_custom_command(TARGET Render POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/CEngine/Presets/Shader"
        "${CMAKE_BINARY_DIR}/Shader")
