module;

#include <chrono>
#include <functional>
#include <utility>

#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <entt/entt.hpp>
#include <imgui.h>

#include <IconFontCppHeaders/IconsKenney.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h_fa-regular-400.ttf.h>
#include <IconFontCppHeaders/IconsKenney.h_kenney-icon-font.ttf.h>

#include <devtools/roboto_regular.ttf.h>
#include <devtools/robotomono_regular.ttf.h>

#include <devtools/fs_imgui_image.bin.h>
#include <devtools/fs_ocornut_imgui.bin.h>
#include <devtools/vs_imgui_image.bin.h>
#include <devtools/vs_ocornut_imgui.bin.h>

#define IMGUI_FLAGS_NONE        UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

module DevTools.Systems;

import DevTools.ImGuiContext;
import DevTools.Tool;
import Gfx.BGFXContext;
import Gfx.BGFXDrawCallback;
import Input.KeyboardState;
import Input.MouseState;

namespace DevTools {
	struct FontRangeMerge {
		const uint8_t *data{};
		size_t size{};
		ImWchar ranges[3]{};
	};

	static FontRangeMerge s_fontRangeMerge[] = {
		{s_kenney_icon_font_ttf, sizeof(s_kenney_icon_font_ttf), {ICON_MIN_KI, ICON_MAX_KI, 0}},
		{s_fa_regular_400_ttf, sizeof(s_fa_regular_400_ttf), {ICON_MIN_FA, ICON_MAX_FA, 0}},
	};

	static const bgfx::EmbeddedShader s_embeddedShaders[] = {
		BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
		BGFX_EMBEDDED_SHADER(vs_imgui_image),
		BGFX_EMBEDDED_SHADER(fs_imgui_image),

		BGFX_EMBEDDED_SHADER_END()
	};

	std::optional<ImGuiKey> getImGuiKey(Input::Key key) {
		static std::unordered_map<Input::Key, ImGuiKey> keyMapping = {
			{ Input::Key::Space, ImGuiKey_Space },
			{ Input::Key::Apostrophe, ImGuiKey_Apostrophe },
			{ Input::Key::Comma, ImGuiKey_Comma },
			{ Input::Key::Minus, ImGuiKey_Minus },
			{ Input::Key::Period, ImGuiKey_Period },
			{ Input::Key::Slash, ImGuiKey_Slash },
			{ Input::Key::Number0, ImGuiKey_0 },
			{ Input::Key::Number1, ImGuiKey_1 },
			{ Input::Key::Number2, ImGuiKey_2 },
			{ Input::Key::Number3, ImGuiKey_3 },
			{ Input::Key::Number4, ImGuiKey_4 },
			{ Input::Key::Number5, ImGuiKey_5 },
			{ Input::Key::Number6, ImGuiKey_6 },
			{ Input::Key::Number7, ImGuiKey_7 },
			{ Input::Key::Number8, ImGuiKey_8 },
			{ Input::Key::Number9, ImGuiKey_9 },
			{ Input::Key::Semicolon, ImGuiKey_Semicolon },
			{ Input::Key::Equal, ImGuiKey_Equal },
			{ Input::Key::A, ImGuiKey_A },
			{ Input::Key::B, ImGuiKey_B },
			{ Input::Key::C, ImGuiKey_C },
			{ Input::Key::D, ImGuiKey_D },
			{ Input::Key::E, ImGuiKey_E },
			{ Input::Key::F, ImGuiKey_F },
			{ Input::Key::G, ImGuiKey_G },
			{ Input::Key::H, ImGuiKey_H },
			{ Input::Key::I, ImGuiKey_I },
			{ Input::Key::J, ImGuiKey_J },
			{ Input::Key::K, ImGuiKey_K },
			{ Input::Key::L, ImGuiKey_L },
			{ Input::Key::M, ImGuiKey_M },
			{ Input::Key::N, ImGuiKey_N },
			{ Input::Key::O, ImGuiKey_O },
			{ Input::Key::P, ImGuiKey_P },
			{ Input::Key::Q, ImGuiKey_Q },
			{ Input::Key::R, ImGuiKey_R },
			{ Input::Key::S, ImGuiKey_S },
			{ Input::Key::T, ImGuiKey_T },
			{ Input::Key::U, ImGuiKey_U },
			{ Input::Key::V, ImGuiKey_V },
			{ Input::Key::W, ImGuiKey_W },
			{ Input::Key::X, ImGuiKey_X },
			{ Input::Key::Y, ImGuiKey_Y },
			{ Input::Key::Z, ImGuiKey_Z },
			{ Input::Key::LeftBracket, ImGuiKey_LeftBracket },
			{ Input::Key::Backslash, ImGuiKey_Backslash },
			{ Input::Key::RightBracket, ImGuiKey_RightBracket },
			{ Input::Key::GraveAccent, ImGuiKey_GraveAccent },
			// { Input::Key::World1, ImGuiKey_World1 },
			// { Input::Key::World2, ImGuiKey_World2 },
			{ Input::Key::Escape, ImGuiKey_Escape },
			{ Input::Key::Enter, ImGuiKey_Enter },
			{ Input::Key::Tab, ImGuiKey_Tab },
			{ Input::Key::Backspace, ImGuiKey_Backspace },
			{ Input::Key::Insert, ImGuiKey_Insert },
			{ Input::Key::Delete, ImGuiKey_Delete },
			{ Input::Key::Right, ImGuiKey_RightArrow },
			{ Input::Key::Left, ImGuiKey_LeftArrow },
			{ Input::Key::Down, ImGuiKey_DownArrow },
			{ Input::Key::Up, ImGuiKey_UpArrow },
			{ Input::Key::PageUp, ImGuiKey_PageUp },
			{ Input::Key::PageDown, ImGuiKey_PageDown },
			{ Input::Key::Home, ImGuiKey_Home },
			{ Input::Key::End, ImGuiKey_End },
			{ Input::Key::CapsLock, ImGuiKey_CapsLock },
			{ Input::Key::ScrollLock, ImGuiKey_ScrollLock },
			{ Input::Key::NumLock, ImGuiKey_NumLock },
			{ Input::Key::PrintScreen, ImGuiKey_PrintScreen },
			{ Input::Key::Pause, ImGuiKey_Pause },
			{ Input::Key::F1, ImGuiKey_F1 },
			{ Input::Key::F2, ImGuiKey_F2 },
			{ Input::Key::F3, ImGuiKey_F3 },
			{ Input::Key::F4, ImGuiKey_F4 },
			{ Input::Key::F5, ImGuiKey_F5 },
			{ Input::Key::F6, ImGuiKey_F6 },
			{ Input::Key::F7, ImGuiKey_F7 },
			{ Input::Key::F8, ImGuiKey_F8 },
			{ Input::Key::F9, ImGuiKey_F9 },
			{ Input::Key::F10, ImGuiKey_F10 },
			{ Input::Key::F11, ImGuiKey_F11 },
			{ Input::Key::F12, ImGuiKey_F12 },
			{ Input::Key::F13, ImGuiKey_F13 },
			{ Input::Key::F14, ImGuiKey_F14 },
			{ Input::Key::F15, ImGuiKey_F15 },
			{ Input::Key::F16, ImGuiKey_F16 },
			{ Input::Key::F17, ImGuiKey_F17 },
			{ Input::Key::F18, ImGuiKey_F18 },
			{ Input::Key::F19, ImGuiKey_F19 },
			{ Input::Key::F20, ImGuiKey_F20 },
			{ Input::Key::F21, ImGuiKey_F21 },
			{ Input::Key::F22, ImGuiKey_F22 },
			{ Input::Key::F23, ImGuiKey_F23 },
			{ Input::Key::F24, ImGuiKey_F24 },
			{ Input::Key::Keypad0, ImGuiKey_Keypad0 },
			{ Input::Key::Keypad1, ImGuiKey_Keypad1 },
			{ Input::Key::Keypad2, ImGuiKey_Keypad2 },
			{ Input::Key::Keypad3, ImGuiKey_Keypad3 },
			{ Input::Key::Keypad4, ImGuiKey_Keypad4 },
			{ Input::Key::Keypad5, ImGuiKey_Keypad5 },
			{ Input::Key::Keypad6, ImGuiKey_Keypad6 },
			{ Input::Key::Keypad7, ImGuiKey_Keypad7 },
			{ Input::Key::Keypad8, ImGuiKey_Keypad8 },
			{ Input::Key::Keypad9, ImGuiKey_Keypad9 },
			{ Input::Key::KeypadDecimal, ImGuiKey_KeypadDecimal },
			{ Input::Key::KeypadDivide, ImGuiKey_KeypadDivide },
			{ Input::Key::KeypadMultiply, ImGuiKey_KeypadMultiply },
			{ Input::Key::KeypadSubtract, ImGuiKey_KeypadSubtract },
			{ Input::Key::KeypadAdd, ImGuiKey_KeypadAdd },
			{ Input::Key::KeypadEnter, ImGuiKey_KeypadEnter },
			{ Input::Key::KeypadEqual, ImGuiKey_KeypadEqual },
			{ Input::Key::LeftShift, ImGuiKey_LeftShift },
			{ Input::Key::LeftControl, ImGuiKey_LeftCtrl },
			{ Input::Key::LeftAlt, ImGuiKey_LeftAlt },
			{ Input::Key::LeftSuper, ImGuiKey_LeftSuper },
			{ Input::Key::RightShift, ImGuiKey_RightShift },
			{ Input::Key::RightControl, ImGuiKey_RightCtrl },
			{ Input::Key::RightAlt, ImGuiKey_RightAlt },
			{ Input::Key::RightSuper, ImGuiKey_RightSuper },
			{ Input::Key::Menu, ImGuiKey_Menu }
		};

		if (auto it = keyMapping.find(key); it != keyMapping.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	DevToolsSystems::DevToolsSystems(Core::EnTTRegistry &registry, Core::Scheduler &scheduler, Gfx::GfxSystems&)
		: mRegistry{registry}
		, mScheduler{scheduler} {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO();

		ImFontConfig config;
		config.FontDataOwnedByAtlas = false;
		config.MergeMode = false;

		constexpr float fontSize = 18.0f;
		auto robotoRegularTtf = const_cast<uint8_t *>(s_robotoRegularTtf);
		auto robotoMonoRegularTtf = const_cast<uint8_t *>(s_robotoMonoRegularTtf);

		const ImWchar *ranges = io.Fonts->GetGlyphRangesCyrillic();
		auto *regularFont = io.Fonts->AddFontFromMemoryTTF(robotoRegularTtf, sizeof(s_robotoRegularTtf), fontSize,
															&config, ranges);
		io.Fonts->AddFontFromMemoryTTF(robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), fontSize - 3.0f, &config,
										ranges);

		config.MergeMode = true;
		config.DstFont = regularFont;

		for (const auto &frm: s_fontRangeMerge) {
			io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t *>(frm.data),
											static_cast<int>(frm.size),
											fontSize - 3.0f,
											&config,
											frm.ranges
			);
		}

		mTickHandle = mScheduler.schedule([this,
			lastTick = std::chrono::steady_clock::now()] mutable {
			using DeltaTimeCast = std::chrono::duration<float>;
			auto clockNow = std::chrono::steady_clock::now();
			auto deltaT = std::chrono::duration_cast<DeltaTimeCast>(clockNow - lastTick).count();
			lastTick = clockNow;

			ImGuiIO &io = ImGui::GetIO();
			io.DeltaTime = deltaT;

			entt::registry &registry(mRegistry);

			auto mouseStateView = registry.view<Input::MouseState>();
			mouseStateView.each([](const Input::MouseState &mouseState) {
				ImGuiIO &io = ImGui::GetIO();
				io.AddMousePosEvent(mouseState.x, mouseState.y);

				for (int i = 0; i < 3; ++i) {
					io.AddMouseButtonEvent(i, mouseState.buttons[i]);
				}
			});

			auto keyboardStateView = registry.view<Input::KeyboardState>();
			keyboardStateView.each([](Input::KeyboardState &keyState) {
				ImGuiIO &io = ImGui::GetIO();

				if (!keyState.charInput.empty()) {
					for (const auto &character: keyState.charInput) {
						io.AddInputCharacter(character);
					}
					keyState.charInput.clear();
				}

				for (uint8_t key = 0; key < Input::KeyCount; ++key) {
					if (auto mappedKey = getImGuiKey(static_cast<Input::Key>(key))) {
						io.AddKeyEvent(*mappedKey, keyState.keys[key]);
					}
				}
			});

			auto createImGuiContextView = registry.view<Gfx::BGFXContext>(entt::exclude<ImGuiContext>);
			createImGuiContextView.each([this, &registry](entt::entity entity, const Gfx::BGFXContext&) {
				bgfx::RendererType::Enum type = bgfx::getRendererType();
				bgfx::ProgramHandle imguiProgram = bgfx::createProgram(
					bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui"),
					bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui"),
					true
				);

				bgfx::UniformHandle imageLodEnabledUniform = bgfx::createUniform(
					"u_imageLodEnabled", bgfx::UniformType::Vec4);
				bgfx::ProgramHandle imguiImageProgram = bgfx::createProgram(
					bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image"),
					bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image"),
					true
				);

				bgfx::VertexLayout layout;
				layout.begin()
						.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
						.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
						.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
						.end();

				bgfx::UniformHandle textureSamplerUniform = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

				ImGuiIO &io = ImGui::GetIO();
				uint8_t *data{};
				int32_t width{};
				int32_t height{};

				io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

				bgfx::TextureHandle fontTextureHandle = bgfx::createTexture2D(
					(uint16_t) width,
					(uint16_t) height,
					false,
					1,
					bgfx::TextureFormat::BGRA8,
					0,
					bgfx::copy(data, width * height * 4)
				);

				registry.emplace<ImGuiContext>(entity,
												fontTextureHandle,
												imguiProgram,
												imguiImageProgram,
												imageLodEnabledUniform,
												textureSamplerUniform,
												layout
				);

				const entt::entity callbackEntity{registry.create()};
				registry.emplace<Gfx::BGFXDrawCallback>(callbackEntity, [this](entt::registry &registry) {
					_renderImGui(registry);
				});
			});

			auto toolUpdateView = registry.view<Tool>();

			auto updateImGuiContext = registry.view<ImGuiContext>();
			updateImGuiContext.each([&registry, &toolUpdateView](ImGuiContext &imguiContext) {
				ImGuiIO &io = ImGui::GetIO();
				io.DisplaySize.x = 1360;
				io.DisplaySize.y = 768;

				ImGui::NewFrame();

				bool open = true;
				if (ImGui::Begin("DevTools", &open, ImGuiWindowFlags_MenuBar)) {
					if (ImGui::BeginMenuBar()) {
						if (ImGui::BeginMenu("Tools")) {
							toolUpdateView.each([&registry](Tool &tool) {
								if (ImGui::MenuItem(tool.toolName.c_str())) {
									tool.isOpen = true;
								}
							});

							ImGui::EndMenu();
						}

						toolUpdateView.each([&registry](Tool &tool) {
							tool.updateFunction(tool, registry);
						});

						ImGui::EndMenuBar();
					}
				}

				ImGui::End();

				//ImGui::ShowDemoWindow();
				ImGui::Render();
			});
		});

		_setupTools(mRegistry);
	}

	DevToolsSystems::~DevToolsSystems() {
		entt::registry &registry(mRegistry);

		if (mRenderCallbackEntity != entt::null) {
			registry.destroy(mRenderCallbackEntity);
		}

		registry.view<ImGuiContext>().each([](ImGuiContext& imguiContext) {
			bgfx::destroy(imguiContext.fontTextureHandle);
			bgfx::destroy(imguiContext.textureSamplerUniform);
			bgfx::destroy(imguiContext.imageLodEnabledUniform);
			bgfx::destroy(imguiContext.imguiImageProgramHandle);
			bgfx::destroy(imguiContext.imguiProgramHandle);
			//bgfx::destroy(imguiContext.layout);
		});

		mScheduler.unschedule(std::move(mTickHandle));
		ImGui::DestroyContext();
	}

	void DevToolsSystems::_renderImGui(entt::registry &registry) {
		registry.view<ImGuiContext>().each([](const ImGuiContext &imGuiContext) {
			const bgfx::Caps *caps = bgfx::getCaps();
			const ImDrawData *drawData = ImGui::GetDrawData();

			int32_t dispWidth = int32_t(drawData->DisplaySize.x * drawData->FramebufferScale.x);
			int32_t dispHeight = int32_t(drawData->DisplaySize.y * drawData->FramebufferScale.y);
			if (dispWidth <= 0
				|| dispHeight <= 0) {
				return;
			}

			float ortho[16];
			float x = drawData->DisplayPos.x;
			float y = drawData->DisplayPos.y;
			float width = drawData->DisplaySize.x;
			float height = drawData->DisplaySize.y;

			bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(1, nullptr, ortho);
			bgfx::setViewRect(1, 0, 0, uint16_t(width), uint16_t(height));

			const ImVec2 clipPos = drawData->DisplayPos; // (0,0) unless using multi-viewports
			const ImVec2 clipScale = drawData->FramebufferScale;
			// (1,1) unless using retina display which are often (2,2)

			for (int i = 0; i < drawData->CmdListsCount; ++i) {
				bgfx::TransientVertexBuffer tvb;
				bgfx::TransientIndexBuffer tib;

				const ImDrawList *drawList = drawData->CmdLists[i];
				uint32_t numVertices = (uint32_t) drawList->VtxBuffer.size();
				uint32_t numIndices = (uint32_t) drawList->IdxBuffer.size();

				// if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices) )
				// {
				// 	// not enough space in transient buffer just quit drawing the rest...
				// 	break;
				// }
				if (bgfx::getAvailTransientVertexBuffer(numVertices, imGuiContext.layout) < numVertices ||
					bgfx::getAvailTransientIndexBuffer(numIndices, sizeof(ImDrawIdx) == 4) < numIndices) {
					break;
				}

				bgfx::allocTransientVertexBuffer(&tvb, numVertices, imGuiContext.layout);
				bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);

				ImDrawVert *verts = (ImDrawVert *) tvb.data;
				bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

				ImDrawIdx *indices = (ImDrawIdx *) tib.data;
				bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

				for (const ImDrawCmd *cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end();
					cmd != cmdEnd; ++cmd) {
					if (cmd->ElemCount == 0) {
						continue;
					}

					uint64_t state = 0
									| BGFX_STATE_WRITE_RGB
									| BGFX_STATE_WRITE_A
									| BGFX_STATE_MSAA;

					bgfx::TextureHandle texture = imGuiContext.fontTextureHandle;
					bgfx::ProgramHandle program = imGuiContext.imguiProgramHandle;

					if (ImU64(0) != cmd->TextureId) {
						union {
							ImTextureID ptr;

							struct {
								bgfx::TextureHandle handle;
								uint8_t flags;
								uint8_t mip;
							} s;
						} _texture = {cmd->TextureId};

						state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & _texture.s.flags)
									? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
									: BGFX_STATE_NONE;
						texture = _texture.s.handle;

						if (0 != _texture.s.mip) {
							const float lodEnabled[4] = {float(_texture.s.mip), 1.0f, 0.0f, 0.0f};
							bgfx::setUniform(imGuiContext.imageLodEnabledUniform, lodEnabled);
							program = imGuiContext.imguiImageProgramHandle;
						}
					} else {
						state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
					}

					// Project scissor/clipping rectangles into framebuffer space
					ImVec4 clipRect;
					clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
					clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
					clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
					clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

					if (clipRect.x >= dispWidth || clipRect.y >= dispHeight || clipRect.z < 0.0f || clipRect.w < 0.0f) {
						continue;
					}

					const uint16_t clipX = uint16_t(bx::max(clipRect.x, 0.0f));
					const uint16_t clipY = uint16_t(bx::max(clipRect.y, 0.0f));

					bgfx::setScissor(clipX, clipY,
									uint16_t(bx::min(clipRect.z, 65535.0f) - clipX),
									uint16_t(bx::min(clipRect.w, 65535.0f) - clipY)
					);

					bgfx::setState(state);
					bgfx::setTexture(0, imGuiContext.textureSamplerUniform, texture);
					bgfx::setVertexBuffer(0, &tvb, cmd->VtxOffset, numVertices);
					bgfx::setIndexBuffer(&tib, cmd->IdxOffset, cmd->ElemCount);
					bgfx::submit(1, program);
				}
			}
		});
	}

	void DevToolsSystems::_setupTools(entt::registry &registry) {
		mMaterialResourceEditTool.setup(registry);
	}
} // DevTools
