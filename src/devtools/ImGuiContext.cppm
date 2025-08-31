module;

#include <bgfx/bgfx.h>

export module DevTools.ImGuiContext;

export namespace DevTools {

    struct ImGuiContext {
        bgfx::TextureHandle fontTextureHandle{};
        bgfx::ProgramHandle imguiProgramHandle{};
        bgfx::ProgramHandle imguiImageProgramHandle{};
        bgfx::UniformHandle imageLodEnabledUniform{};
        bgfx::UniformHandle textureSamplerUniform{};
        bgfx::VertexLayout layout{};
    };

} // DevTools
