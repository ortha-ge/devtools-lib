
cmake_minimum_required(VERSION 3.31)

Include(FetchContent)

find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)

FetchContent_Declare(
        RPP
        GIT_REPOSITORY https://github.com/victimsnino/ReactivePlusPlus.git
        GIT_TAG        origin/v2
)

FetchContent_MakeAvailable(RPP)

set (ortha-devtools-src
        src/devtools/services/DevToolsSystemService.cpp
        src/devtools/systems/DevToolsSystems.cpp
        src/devtools/tools/EntityTool.cpp
        src/devtools/tools/LogViewerTool.cpp
        src/devtools/tools/MaterialEditorTool.cpp
        src/devtools/tools/NodeTool.cpp
        src/devtools/tools/ResourceCacheTool.cpp
        src/devtools/tools/SceneViewerTool.cpp
        src/devtools/tools/SpriteEditorTool.cpp
)

set (ortha-devtools-modules
        src/devtools/SelectedEntity.cppm
        src/devtools/SelectedSceneRoot.cppm
        src/devtools/Tool.cppm
        src/devtools/services/DevToolsSystemService.cppm
        src/devtools/services/EntityToolService.cppm
        src/devtools/services/LogViewerToolService.cppm
        src/devtools/services/NodeToolService.cppm
        src/devtools/services/MaterialEditorToolService.cppm
        src/devtools/services/ResourceCacheToolService.cppm
        src/devtools/services/SceneViewerToolService.cppm
        src/devtools/services/SpriteEditorToolService.cppm
        src/devtools/systems/DevToolsSystems.cppm
        src/devtools/tools/EntityTool.cppm
        src/devtools/tools/LogViewerTool.cppm
        src/devtools/tools/MaterialEditorTool.cppm
        src/devtools/tools/NodeTool.cppm
        src/devtools/tools/ResourceCacheTool.cppm
        src/devtools/tools/SceneViewerTool.cppm
        src/devtools/tools/SpriteEditorTool.cppm
)

add_library(ortha-devtools STATIC ${ortha-devtools-src} ${ortha-devtools-modules})

target_sources(ortha-devtools PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${ortha-devtools-modules})
target_compile_features(ortha-devtools PUBLIC cxx_std_23)
target_link_libraries(ortha-devtools PUBLIC ortha-core ortha-gfx ortha-input ortha-audio imgui::imgui imguizmo::imguizmo RPP::rpp)
target_include_directories(ortha-devtools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/deps/)

set (ortha-devtools-fonts-src
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/IconsFontAwesome6.h_fa-regular-400.ttf.h
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/IconsKenney.h_kenney-icon-font.ttf.h
)

add_custom_target(
        ortha-devtools-fonts ALL
        DEPENDS ${ortha-devtools-fonts-src}
        COMMENT "Building Fonts"
)

add_custom_command(
        OUTPUT ${ortha-devtools-fonts-src}
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/GenerateIconFontCppHeaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/
)

add_dependencies(ortha-devtools ortha-devtools-fonts)


#add_subdirectory(tests)
