module;

#include <chrono>
#include <functional>
#include <utility>

#include <bimg/bimg.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <IconFontCppHeaders/IconsFontAwesome6.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h_fa-regular-400.ttf.h>
#include <IconFontCppHeaders/IconsKenney.h>
#include <IconFontCppHeaders/IconsKenney.h_kenney-icon-font.ttf.h>

#include <devtools/roboto_regular.ttf.h>
#include <devtools/robotomono_regular.ttf.h>

#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

module DevTools.Systems;

import Core.Any;
import Core.JsonTypeLoaderAdapter;
import Core.Log;
import Core.RawDataResource;
import Core.ResourceHandleUtils;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import DevTools.Tool;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Reflection.ShaderProgramDescriptor;
import Gfx.RenderCommand;
import Gfx.RenderState;
import Gfx.ShaderProgram;
import Gfx.ShaderProgramDescriptor;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import Input.KeyboardEvent;
import Input.KeyboardState;
import Input.MouseState;

namespace DevTools {
	struct FontRangeMerge {
		const uint8_t* data{};
		size_t size{};
		ImWchar ranges[3]{};
	};

	static FontRangeMerge s_fontRangeMerge[] = {
		{ s_kenney_icon_font_ttf, sizeof(s_kenney_icon_font_ttf), { ICON_MIN_KI, ICON_MAX_KI, 0 } },
		{ s_fa_regular_400_ttf, sizeof(s_fa_regular_400_ttf), { ICON_MIN_FA, ICON_MAX_FA, 0 } },
	};

	std::optional<ImGuiKey> getImGuiKey(Input::Key key) {
		switch (key) {
			case Input::Key::Space:
				return ImGuiKey_Space;
			case Input::Key::Apostrophe:
				return ImGuiKey_Apostrophe;
			case Input::Key::Comma:
				return ImGuiKey_Comma;
			case Input::Key::Minus:
				return ImGuiKey_Minus;
			case Input::Key::Period:
				return ImGuiKey_Period;
			case Input::Key::Slash:
				return ImGuiKey_Slash;
			case Input::Key::Number0:
				return ImGuiKey_0;
			case Input::Key::Number1:
				return ImGuiKey_1;
			case Input::Key::Number2:
				return ImGuiKey_2;
			case Input::Key::Number3:
				return ImGuiKey_3;
			case Input::Key::Number4:
				return ImGuiKey_4;
			case Input::Key::Number5:
				return ImGuiKey_5;
			case Input::Key::Number6:
				return ImGuiKey_6;
			case Input::Key::Number7:
				return ImGuiKey_7;
			case Input::Key::Number8:
				return ImGuiKey_8;
			case Input::Key::Number9:
				return ImGuiKey_9;
			case Input::Key::Semicolon:
				return ImGuiKey_Semicolon;
			case Input::Key::Equal:
				return ImGuiKey_Equal;
			case Input::Key::A:
				return ImGuiKey_A;
			case Input::Key::B:
				return ImGuiKey_B;
			case Input::Key::C:
				return ImGuiKey_C;
			case Input::Key::D:
				return ImGuiKey_D;
			case Input::Key::E:
				return ImGuiKey_E;
			case Input::Key::F:
				return ImGuiKey_F;
			case Input::Key::G:
				return ImGuiKey_G;
			case Input::Key::H:
				return ImGuiKey_H;
			case Input::Key::I:
				return ImGuiKey_I;
			case Input::Key::J:
				return ImGuiKey_J;
			case Input::Key::K:
				return ImGuiKey_K;
			case Input::Key::L:
				return ImGuiKey_L;
			case Input::Key::M:
				return ImGuiKey_M;
			case Input::Key::N:
				return ImGuiKey_N;
			case Input::Key::O:
				return ImGuiKey_O;
			case Input::Key::P:
				return ImGuiKey_P;
			case Input::Key::Q:
				return ImGuiKey_Q;
			case Input::Key::R:
				return ImGuiKey_R;
			case Input::Key::S:
				return ImGuiKey_S;
			case Input::Key::T:
				return ImGuiKey_T;
			case Input::Key::U:
				return ImGuiKey_U;
			case Input::Key::V:
				return ImGuiKey_V;
			case Input::Key::W:
				return ImGuiKey_W;
			case Input::Key::X:
				return ImGuiKey_X;
			case Input::Key::Y:
				return ImGuiKey_Y;
			case Input::Key::Z:
				return ImGuiKey_Z;
			case Input::Key::LeftBracket:
				return ImGuiKey_LeftBracket;
			case Input::Key::Backslash:
				return ImGuiKey_Backslash;
			case Input::Key::RightBracket:
				return ImGuiKey_RightBracket;
			case Input::Key::GraveAccent:
				return ImGuiKey_GraveAccent;
			case Input::Key::Escape:
				return ImGuiKey_Escape;
			case Input::Key::Enter:
				return ImGuiKey_Enter;
			case Input::Key::Tab:
				return ImGuiKey_Tab;
			case Input::Key::Backspace:
				return ImGuiKey_Backspace;
			case Input::Key::Insert:
				return ImGuiKey_Insert;
			case Input::Key::Delete:
				return ImGuiKey_Delete;
			case Input::Key::Right:
				return ImGuiKey_RightArrow;
			case Input::Key::Left:
				return ImGuiKey_LeftArrow;
			case Input::Key::Down:
				return ImGuiKey_DownArrow;
			case Input::Key::Up:
				return ImGuiKey_UpArrow;
			case Input::Key::PageUp:
				return ImGuiKey_PageUp;
			case Input::Key::PageDown:
				return ImGuiKey_PageDown;
			case Input::Key::Home:
				return ImGuiKey_Home;
			case Input::Key::End:
				return ImGuiKey_End;
			case Input::Key::CapsLock:
				return ImGuiKey_CapsLock;
			case Input::Key::ScrollLock:
				return ImGuiKey_ScrollLock;
			case Input::Key::NumLock:
				return ImGuiKey_NumLock;
			case Input::Key::PrintScreen:
				return ImGuiKey_PrintScreen;
			case Input::Key::Pause:
				return ImGuiKey_Pause;
			case Input::Key::F1:
				return ImGuiKey_F1;
			case Input::Key::F2:
				return ImGuiKey_F2;
			case Input::Key::F3:
				return ImGuiKey_F3;
			case Input::Key::F4:
				return ImGuiKey_F4;
			case Input::Key::F5:
				return ImGuiKey_F5;
			case Input::Key::F6:
				return ImGuiKey_F6;
			case Input::Key::F7:
				return ImGuiKey_F7;
			case Input::Key::F8:
				return ImGuiKey_F8;
			case Input::Key::F9:
				return ImGuiKey_F9;
			case Input::Key::F10:
				return ImGuiKey_F10;
			case Input::Key::F11:
				return ImGuiKey_F11;
			case Input::Key::F12:
				return ImGuiKey_F12;
			case Input::Key::F13:
				return ImGuiKey_F13;
			case Input::Key::F14:
				return ImGuiKey_F14;
			case Input::Key::F15:
				return ImGuiKey_F15;
			case Input::Key::F16:
				return ImGuiKey_F16;
			case Input::Key::F17:
				return ImGuiKey_F17;
			case Input::Key::F18:
				return ImGuiKey_F18;
			case Input::Key::F19:
				return ImGuiKey_F19;
			case Input::Key::F20:
				return ImGuiKey_F20;
			case Input::Key::F21:
				return ImGuiKey_F21;
			case Input::Key::F22:
				return ImGuiKey_F22;
			case Input::Key::F23:
				return ImGuiKey_F23;
			case Input::Key::F24:
				return ImGuiKey_F24;
			case Input::Key::Keypad0:
				return ImGuiKey_Keypad0;
			case Input::Key::Keypad1:
				return ImGuiKey_Keypad1;
			case Input::Key::Keypad2:
				return ImGuiKey_Keypad2;
			case Input::Key::Keypad3:
				return ImGuiKey_Keypad3;
			case Input::Key::Keypad4:
				return ImGuiKey_Keypad4;
			case Input::Key::Keypad5:
				return ImGuiKey_Keypad5;
			case Input::Key::Keypad6:
				return ImGuiKey_Keypad6;
			case Input::Key::Keypad7:
				return ImGuiKey_Keypad7;
			case Input::Key::Keypad8:
				return ImGuiKey_Keypad8;
			case Input::Key::Keypad9:
				return ImGuiKey_Keypad9;
			case Input::Key::KeypadDecimal:
				return ImGuiKey_KeypadDecimal;
			case Input::Key::KeypadDivide:
				return ImGuiKey_KeypadDivide;
			case Input::Key::KeypadMultiply:
				return ImGuiKey_KeypadMultiply;
			case Input::Key::KeypadSubtract:
				return ImGuiKey_KeypadSubtract;
			case Input::Key::KeypadAdd:
				return ImGuiKey_KeypadAdd;
			case Input::Key::KeypadEnter:
				return ImGuiKey_KeypadEnter;
			case Input::Key::KeypadEqual:
				return ImGuiKey_KeypadEqual;
			case Input::Key::LeftShift:
				return ImGuiKey_LeftShift;
			case Input::Key::LeftControl:
				return ImGuiKey_LeftCtrl;
			case Input::Key::LeftAlt:
				return ImGuiKey_LeftAlt;
			case Input::Key::LeftSuper:
				return ImGuiKey_LeftSuper;
			case Input::Key::RightShift:
				return ImGuiKey_RightShift;
			case Input::Key::RightControl:
				return ImGuiKey_RightCtrl;
			case Input::Key::RightAlt:
				return ImGuiKey_RightAlt;
			case Input::Key::RightSuper:
				return ImGuiKey_RightSuper;
			case Input::Key::Menu:
				return ImGuiKey_Menu;
			case Input::Key::World1:
			case Input::Key::World2:
			case Input::Key::F25:
			case Input::Key::Count:
				return std::nullopt;
		}

		return std::nullopt;
	}

	DevToolsSystems::DevToolsSystems(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry{ registry }
		, mScheduler{ scheduler } {
		using namespace Core;
		using namespace Gfx;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImFontConfig config;
		config.FontDataOwnedByAtlas = false;
		config.MergeMode = false;

		constexpr float fontSize = 18.0f;
		auto robotoRegularTtf = const_cast<uint8_t*>(s_robotoRegularTtf);
		auto robotoMonoRegularTtf = const_cast<uint8_t*>(s_robotoMonoRegularTtf);

		const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
		auto* regularFont =
			io.Fonts->AddFontFromMemoryTTF(robotoRegularTtf, sizeof(s_robotoRegularTtf), fontSize, &config, ranges);
		io.Fonts->AddFontFromMemoryTTF(
			robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), fontSize - 3.0f, &config, ranges);

		config.MergeMode = true;
		config.DstFont = regularFont;

		for (const auto& frm : s_fontRangeMerge) {
			io.Fonts->AddFontFromMemoryTTF(
				const_cast<uint8_t*>(frm.data), static_cast<int>(frm.size), fontSize - 3.0f, &config, frm.ranges);
		}

		auto shaderProgramLoaderAdapter = std::make_shared<JsonTypeLoaderAdapter<ShaderProgramDescriptor>>();
		mShaderProgram = ResourceLoadRequest::create<TypeLoader>(registry, "assets/shaders/ocornut_imgui.json", shaderProgramLoaderAdapter);
		mImageShaderProgram = ResourceLoadRequest::create<TypeLoader>(registry, "assets/shaders/ocornut_imgui_image.json", shaderProgramLoaderAdapter);
		mFontImage = createFontTexture(registry);

		mTickHandle = mScheduler.schedule([this, lastTick = std::chrono::steady_clock::now()] mutable {
			using DeltaTimeCast = std::chrono::duration<float>;
			auto clockNow = std::chrono::steady_clock::now();
			auto deltaT = std::chrono::duration_cast<DeltaTimeCast>(clockNow - lastTick).count();
			lastTick = clockNow;

			ImGuiIO& io = ImGui::GetIO();
			io.DeltaTime = deltaT;

			entt::registry& registry(mRegistry);

			registry.view<Input::MouseState>().each([](const Input::MouseState& mouseState) {
				ImGuiIO& io = ImGui::GetIO();
				io.AddMousePosEvent(mouseState.x, mouseState.y);

				for (int i = 0; i < 3; ++i) {
					io.AddMouseButtonEvent(i, mouseState.buttons[i]);
				}
			});

			registry.view<Input::KeyboardState>().each([](Input::KeyboardState& keyState) {
				ImGuiIO& io = ImGui::GetIO();

				if (!keyState.charInput.empty()) {
					for (const auto& character : keyState.charInput) {
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

			registry.view<Input::KeyboardEvent>()
				.each([this](const Input::KeyboardEvent& keyboardEvent) {
					if (keyboardEvent.key == Input::Key::GraveAccent &&
						keyboardEvent.eventType == Input::InputEventType::Pressed) {
						mIsEnabled = !mIsEnabled;
					}
				});

			io.DisplaySize.x = 1360;
			io.DisplaySize.y = 768;

			ImGui::NewFrame();

			if (mIsEnabled) {
				drawDevToolsImGui(mRegistry);
			}

			ImGui::Render();
			_renderImGui(mRegistry);
		});
	}

	DevToolsSystems::~DevToolsSystems() {
		entt::registry& registry(mRegistry);

		if (mFontImage != entt::null) {
			registry.destroy(mFontImage);
			mFontImage = entt::null;
		}

		mScheduler.unschedule(std::move(mTickHandle));
		ImGui::DestroyContext();
	}

	entt::entity DevToolsSystems::createFontTexture(entt::registry& registry) {
		using namespace Gfx;

		uint8_t* data{};
		int32_t width{};
		int32_t height{};

		ImGuiIO& io{ ImGui::GetIO() };
		io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

		Image image;
		image.width = width;
		image.height = height;
		image.numMips = 0;
		image.numLayers = 1;
		image.format = bimg::TextureFormat::BGRA8;
		image.image.resize(width * height * 4);
		memcpy(image.image.data(), data, width * height * 4);

		const entt::entity entity{ registry.create() };
		registry.emplace<Image>(entity, std::move(image));
		return entity;
	}

	void DevToolsSystems::drawDevToolsImGui(entt::registry& registry) {
		auto toolUpdateView = registry.view<Tool>();
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Main")) {
				if (ImGui::MenuItem("Close")) {
					mIsEnabled = false;
				}

				if (ImGui::MenuItem("Reset Layout")) {
					ImGui::ClearIniSettings();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools")) {
				toolUpdateView.each([this](Tool& tool) {
					if (ImGui::MenuItem(tool.toolName.c_str())) {
						tool.isOpen = true;
						tool.isOpenFunctionCalled = false;
					}
				});

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		toolUpdateView.each([this](Tool& tool) {
			if (!tool.isOpen) {
				if (tool.isOpenFunctionCalled && tool.closeFunction) {
					tool.closeFunction(mRegistry, tool);
					tool.isOpenFunctionCalled = false;
				}
				return;
			}

			tool.isMinimized = !ImGui::Begin(tool.toolName.c_str(), &tool.isOpen);
			if (!tool.isMinimized && !tool.isOpenFunctionCalled && tool.openFunction) {
				tool.openFunction(mRegistry, tool);
				tool.isOpenFunctionCalled = true;
			}

			if (!tool.isMinimized && tool.updateFunction) {
				tool.updateFunction(mRegistry, tool);
			}

			if ((!tool.isOpen || tool.isMinimized) && tool.isOpenFunctionCalled && tool.closeFunction) {
				tool.closeFunction(mRegistry, tool);
				tool.isOpenFunctionCalled = false;
			}

			ImGui::End();
		});
	}

	void DevToolsSystems::_renderImGui(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		registry.view<Viewport>().each([this, &registry](const entt::entity viewportEntity, const Viewport& viewport) {
			auto&& [shaderProgramEntity, shaderProgram] = getResourceAndEntity<ShaderProgram>(registry, mShaderProgram);
			auto&& [imageShaderProgramEntity, imageShaderProgram] = getResourceAndEntity<ShaderProgram>(registry, mImageShaderProgram);
			if (!shaderProgram || !imageShaderProgram) {
				return;
			}

			// const bgfx::Caps* caps = bgfx::getCaps();
			const ImDrawData* drawData = ImGui::GetDrawData();
			if (!drawData) {
				return;
			}

			int32_t dispWidth = int32_t(drawData->DisplaySize.x * drawData->FramebufferScale.x);
			int32_t dispHeight = int32_t(drawData->DisplaySize.y * drawData->FramebufferScale.y);
			if (dispWidth <= 0 || dispHeight <= 0) {
				return;
			}

			const ImVec2 clipPos = drawData->DisplayPos; // (0,0) unless using multi-viewports
			const ImVec2 clipScale = drawData->FramebufferScale;
			// (1,1) unless using retina display which are often (2,2)

			int sortDepth = 0;
			for (int i = 0; i < drawData->CmdListsCount; ++i) {
				const ImDrawList* drawList = drawData->CmdLists[i];

				// Vertex Buffer
				const auto vertexCount = static_cast<uint32_t>(drawList->VtxBuffer.size());
				VertexBuffer vertexBuffer;
				vertexBuffer.vertexLayout = shaderProgramEntity;
				vertexBuffer.type = VertexBufferType::Transient;
				vertexBuffer.data.resize(sizeof(ImDrawVert) * vertexCount);

				auto* vertexHead = reinterpret_cast<ImDrawVert*>(vertexBuffer.data.data());
				memcpy(vertexHead, drawList->VtxBuffer.begin(), sizeof(ImDrawVert) * vertexCount);

				const entt::entity vertexBufferEntity = registry.create();
				registry.emplace<VertexBuffer>(vertexBufferEntity, std::move(vertexBuffer));

				// Index Buffer
				const auto indexCount = static_cast<uint32_t>(drawList->IdxBuffer.size());
				IndexBuffer indexBuffer;
				indexBuffer.type = IndexBufferType::Transient;
				indexBuffer.data.resize(sizeof(ImDrawIdx) * indexCount);

				auto* indexHead = reinterpret_cast<ImDrawIdx*>(indexBuffer.data.data());
				memcpy(indexHead, drawList->IdxBuffer.begin(), sizeof(ImDrawIdx) * indexCount);

				const entt::entity indexBufferEntity = registry.create();
				registry.emplace<IndexBuffer>(indexBufferEntity, std::move(indexBuffer));

				for (const ImDrawCmd *cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end();
					 cmd != cmdEnd; ++cmd) {
					if (cmd->ElemCount == 0) {
						continue;
					}

					RenderCommand renderCommand;
					renderCommand.vertexBuffer = vertexBufferEntity;
					renderCommand.indexBuffer = indexBufferEntity;
					renderCommand.vertexOffset = cmd->VtxOffset;
					renderCommand.vertexCount = vertexCount;
					renderCommand.indexOffset = cmd->IdxOffset;
					renderCommand.indexCount = cmd->ElemCount;

					RenderState renderState{};
					renderState.bufferWriting = BufferWriting::RGB | BufferWriting::Alpha;
					renderState.msaa = true;

					renderCommand.viewportEntity = viewportEntity;
					renderCommand.renderPass = 255;
					renderCommand.sortDepth = sortDepth++;

					renderCommand.shaderProgram = shaderProgramEntity;
					renderCommand.uniformData["s_texColor"] = Any(entt::entity{ mFontImage });

					if (ImTextureID{0} != cmd->TextureId) {
						const entt::entity textureEntity = static_cast<entt::entity>(cmd->TextureId);
						renderCommand.uniformData["s_texColor"] = Any(entt::entity{ textureEntity });

						bool lod = false;
						if (lod) {
							renderCommand.shaderProgram = imageShaderProgramEntity;
						}

						renderState.blendLhs = BlendOperand::SourceAlpha;
						renderState.blendOperator = BlendOperator::Add;
						renderState.blendRhs = BlendOperand::InverseSourceAlpha;
					} else {
						renderState.blendLhs = BlendOperand::SourceAlpha;
						renderState.blendOperator = BlendOperator::Add;
						renderState.blendRhs = BlendOperand::InverseSourceAlpha;
					}

					renderCommand.renderState = renderState;

					// Project scissor/clipping rectangles into framebuffer space
					ImVec4 clipRect;
					clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
					clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
					clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
					clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

					if (clipRect.x >= dispWidth || clipRect.y >= dispHeight || clipRect.z < 0.0f || clipRect.w < 0.0f) {
						continue;
					}

					const uint16_t clipX = uint16_t(std::max(clipRect.x, 0.0f));
					const uint16_t clipY = uint16_t(std::max(clipRect.y, 0.0f));

					renderCommand.scissorRect = {
						clipX,
						clipY,
						static_cast<uint16_t>(std::min(clipRect.z, 65535.0f) - clipX),
						static_cast<uint16_t>(std::min(clipRect.w, 65535.0f) - clipY)
					};

					const entt::entity renderCommandEntity = registry.create();
					registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
				}
			}
		});
	}
} // namespace DevTools
