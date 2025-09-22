
cmake_minimum_required(VERSION 3.31)

Include(FetchContent)

find_package(imgui CONFIG REQUIRED)

FetchContent_Declare(
        RPP
        GIT_REPOSITORY https://github.com/victimsnino/ReactivePlusPlus.git
        GIT_TAG        origin/v2
)

FetchContent_MakeAvailable(RPP)

set (devtools-lib-src
        src/devtools/services/DevToolsSystemService.cpp
        src/devtools/systems/DevToolsSystems.cpp
        src/devtools/tools/LogViewerTool.cpp
        src/devtools/tools/MaterialEditorTool.cpp
        src/devtools/tools/NodeTool.cpp
        src/devtools/tools/ResourceCacheTool.cpp
        src/devtools/tools/SpriteEditorTool.cpp
)

set (devtools-lib-modules
        src/devtools/ImGuiContext.cppm
        src/devtools/Tool.cppm
        src/devtools/services/DevToolsSystemService.cppm
        src/devtools/services/LogViewerToolService.cppm
        src/devtools/services/NodeToolService.cppm
        src/devtools/services/MaterialEditorToolService.cppm
        src/devtools/services/ResourceCacheToolService.cppm
        src/devtools/services/SpriteEditorToolService.cppm
        src/devtools/systems/DevToolsSystems.cppm
        src/devtools/tools/LogViewerTool.cppm
        src/devtools/tools/MaterialEditorTool.cppm
        src/devtools/tools/NodeTool.cppm
        src/devtools/tools/ResourceCacheTool.cppm
        src/devtools/tools/SpriteEditorTool.cppm
)

add_library(devtools-lib STATIC ${devtools-lib-src} ${devtools-lib-modules})

target_sources(devtools-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${devtools-lib-modules})
target_compile_features(devtools-lib PUBLIC cxx_std_23)
target_link_libraries(devtools-lib PUBLIC core-lib gfx-lib input-lib audio-lib imgui::imgui RPP::rpp)
target_include_directories(devtools-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/deps/)

set (devtools-lib-fonts-src
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/IconsFontAwesome6.h_fa-regular-400.ttf.h
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/IconsKenney.h_kenney-icon-font.ttf.h
)

add_custom_target(
        devtools-lib-fonts ALL
        DEPENDS ${devtools-lib-fonts-src}
        COMMENT "Building Fonts"
)

add_custom_command(
        OUTPUT ${devtools-lib-fonts-src}
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/GenerateIconFontCppHeaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/IconFontCppHeaders/
)

add_dependencies(devtools-lib devtools-lib-fonts)


#add_subdirectory(tests)
