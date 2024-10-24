include_directories("CEngine/ThirdParty/include")

# glad库
include_directories("CEngine/ThirdParty/glad/include")
file(GLOB glad_source "CEngine/ThirdParty/glad/src/*.c")
add_library(glad STATIC ${glad_source})

# GLFW库
include_directories("CEngine/ThirdParty/glfw3/include")
set(glfw3 "glfw")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(glfw3 "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/glfw3/lib/glfw3.lib")
endif ()

#assimp库
include_directories("CEngine/ThirdParty/assimp/include")
set(assimp "assimp")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(assimp "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/assimp/lib/assimp-vc143-mt.lib")
endif ()

# imgui库
include_directories("CEngine/ThirdParty/imgui/include")
file(GLOB glad_source "CEngine/ThirdParty/imgui/src/*.cpp")
add_library(imgui STATIC ${glad_source})

# std模块（微软）
add_library(std_modules)
file(GLOB std_modules_source "CEngine/ThirdParty/std/modules/*.ixx")
target_sources(std_modules PUBLIC FILE_SET CXX_MODULES FILES ${std_modules_source})

# Base
add_library(Base)
file(GLOB base_source "CEngine/Base/*.ixx")
target_sources(Base PUBLIC FILE_SET CXX_MODULES FILES ${base_source})
target_link_libraries(
        Base
        std_modules
)

# 日志类
add_library(Logger)
target_sources(Logger PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Utils/Logger.ixx")
target_link_libraries(
        Logger
        std_modules
)

# 事件类
add_library(Event)
target_sources(Event PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Utils/Event.ixx")
target_link_libraries(
        Event
        std_modules
        Base
)

# 渲染单位
add_library(Render)
file(GLOB render_sources "CEngine/Render/*.ixx")
target_sources(Render PUBLIC FILE_SET CXX_MODULES FILES ${render_sources})
target_link_libraries(
        Render
        std_modules
        Base
        Logger
        Utils
        Image
        Event
)

# 图像相关
add_library(Image)
file(GLOB image_sources "CEngine/Image/*.ixx")
target_sources(Image PUBLIC FILE_SET CXX_MODULES FILES ${image_sources})
target_link_libraries(
        Image
        std_modules
        glad
        Utils
        Logger
)

# Node系列
add_library(Node)
file(GLOB node_sources "CEngine/Node/*.ixx" "CEngine/Node/Behaviour/*.ixx")
target_sources(Node PUBLIC FILE_SET CXX_MODULES FILES ${node_sources})
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
target_sources(ModelImporter PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Utils/ModelImporter.ixx")
target_link_libraries(
        ModelImporter
        Node
        Render
        ${assimp}
)

# 其他工具类
add_library(Utils)
target_sources(Utils PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Utils/Utils.ixx")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    target_sources(Utils PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Utils/WindowsFileDialog.ixx")
endif ()
target_link_libraries(
        Utils
        std_modules
)

# UI
add_library(UI)
target_sources(UI PUBLIC FILE_SET CXX_MODULES FILES "CEngine/UI/UI.ixx")
target_link_libraries(
        UI
        ${glfw3}
        imgui
        Base
        Logger
        ModelImporter
        Render
)

#编辑器UI
add_library(EditorUI)
file(GLOB editor_ui_sources "CEngine/UI/EditorUI/*.ixx")
target_sources(EditorUI PUBLIC FILE_SET CXX_MODULES FILES ${editor_ui_sources})
target_link_libraries(
        EditorUI
        ${glfw3}
        imgui
        Engine
        UI
        Logger
)

# Behaviour预设
add_library(BehaviourPresets)
file(GLOB behaviour_presets_sources "CEngine/Presets/Behaviours/*.ixx")
target_sources(BehaviourPresets PUBLIC FILE_SET CXX_MODULES FILES ${behaviour_presets_sources})
target_link_libraries(
        BehaviourPresets
        std_modules
        Engine
        Node
)

# 预设加载器
add_library(PresetsLoader)
target_sources(PresetsLoader PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Presets/PresetsLoader.ixx")
target_link_libraries(
        PresetsLoader
        std_modules
        Base
        Render
        Node
        Logger
        Utils
        BehaviourPresets
        Engine
)

# 引擎主模块
add_library(Engine)
target_sources(Engine PUBLIC FILE_SET CXX_MODULES FILES "CEngine/Engine.ixx")
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
        UI
)

# 构建后处理
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    add_custom_command(TARGET Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/glfw3/lib/glfw3.dll"
            "${CMAKE_BINARY_DIR}/glfw3.dll")

    add_custom_command(TARGET Engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/CEngine/ThirdParty/assimp/lib/assimp-vc143-mt.dll"
            "${CMAKE_BINARY_DIR}/assimp-vc143-mt.dll")
endif ()

add_custom_command(TARGET PresetsLoader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/CEngine/Presets/Shader"
        "${CMAKE_BINARY_DIR}/Shader")