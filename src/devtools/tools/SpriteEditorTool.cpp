module;

#include <imgui.h>
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

module DevTools.SpriteEditorTool;

import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import DevTools.Tool;
import Gfx.Camera;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Gfx.SpriteObject;
import Gfx.Reflection.MaterialDescriptor;

namespace DevTools {

	void SpriteEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Sprite Editor", [this](entt::registry& registry, Tool& tool) {
			if (mSpriteObjectRoot == entt::null) {
				mSpriteObjectRoot = registry.create();
				registry.emplace<Core::Spatial>(mSpriteObjectRoot, glm::vec3{}, glm::vec3(5.0f, 5.0f, 1.0f));
				registry.emplace<Gfx::RenderObject>(mSpriteObjectRoot);
				registry.emplace<Gfx::SpriteObject>(mSpriteObjectRoot, mSprite);
			}

			if (ImGui::InputText("Material Resource File Path", &mMaterialResourceFilePath)) {
				auto& renderObject{ registry.get<Gfx::RenderObject>(mSpriteObjectRoot) };
				renderObject.materialResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
					registry, mMaterialResourceFilePath,
					std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());
			}

			bool changed = false;
			if (ImGui::CollapsingHeader("Frames")) {
				for (size_t i = 0; i < mSprite.frames.size(); ++i) {
					if (ImGui::TreeNode(std::format("Frame {}", i).c_str())) {
						auto& frame{ mSprite.frames[i] };
						changed =
							ImGui::InputFloat2("Bottom Left", &frame.bottomLeft[0]) ||
							ImGui::InputFloat2("Top Right", &frame.topRight[0]);
						ImGui::TreePop();
					}
				}
			}

			if (ImGui::Button("Add Frame")) {
				mSprite.frames.emplace_back(glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 100.0f, 100.0f });
				changed = true;
			}

			if (changed) {
				registry.get<Gfx::SpriteObject>(mSpriteObjectRoot).spriteResource = mSprite;
			}

			// Sync with the camera position
			auto cameraView = registry.view<Gfx::Camera>();
			if (cameraView.empty()) {
				return;
			}

			const auto& cameraEntity = cameraView.front();
			if (!registry.all_of<Core::Spatial>(cameraEntity)) {
				return;
			}

			const auto& cameraSpatial = registry.get<Core::Spatial>(cameraEntity);

			auto& renderObjectSpatial{ registry.get<Core::Spatial>(mSpriteObjectRoot) };
			renderObjectSpatial.position = cameraSpatial.position + mSpriteRenderPos;

		}, [this](entt::registry& registry, Tool& tool) {
			if (mSpriteObjectRoot != entt::null) {
				registry.destroy(mSpriteObjectRoot);
				mSpriteObjectRoot = entt::null;
			}
		});
	}

} // namespace DevTools
