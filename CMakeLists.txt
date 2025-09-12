
cmake_minimum_required(VERSION 3.31)

find_package(imgui CONFIG REQUIRED)

set (devtools-lib-src
        src/devtools/systems/DevToolsSystems.cpp
        src/devtools/tools/LogViewerTool.cpp
        src/devtools/tools/MaterialEditorTool.cpp
        src/devtools/tools/SpriteEditorTool.cpp
)

set (devtools-lib-modules
        src/devtools/ImGuiContext.cppm
        src/devtools/Tool.cppm
        src/devtools/services/DevToolsSystemService.cppm
        src/devtools/systems/DevToolsSystems.cppm
        src/devtools/tools/LogViewerTool.cppm
        src/devtools/tools/MaterialEditorTool.cppm
        src/devtools/tools/SpriteEditorTool.cppm
)

add_library(devtools-lib STATIC ${devtools-lib-src} ${devtools-lib-modules})

target_sources(devtools-lib PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${devtools-lib-modules})
target_compile_features(devtools-lib PUBLIC cxx_std_23)
target_link_libraries(devtools-lib PUBLIC core-lib gfx-lib input-lib audio-lib imgui::imgui)
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
