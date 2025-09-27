module;

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <imgui.h>

module DevTools.SceneViewerTool;

import Core.Spatial;
import DevTools.SelectedSceneRoot;
import DevTools.Tool;
import Gfx.Camera;
import Gfx.RenderTexture;
import Gfx.Viewport;

namespace DevTools {

	SceneViewerTool::SceneViewerTool(Core::EnTTRegistry& registry) {
		setupTool(registry);
	}

	void SceneViewerTool::setupTool(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		const entt::entity toolEntity = registry.create();
		registry.emplace<Tool>(toolEntity, "Scene",
			[this](entt::registry& registry, Tool&) { onOpen(registry); },
			[this](entt::registry& registry, Tool&) { onUpdate(registry); },
			[this](entt::registry& registry, Tool&) { onClose(registry); }
		);

		// Render Texture
		mRenderTexture = registry.create();

		RenderTexture renderTexture;
		renderTexture.width = 512;
		renderTexture.height = 512;
		registry.emplace<RenderTexture>(mRenderTexture, renderTexture);

		// Camera + Viewport
		mCameraEntity = registry.create();
		registry.emplace<Spatial>(mCameraEntity, glm::vec3{}, glm::vec3{ 5.0f, 5.0f,  1.0f });

		Viewport viewport;
		viewport.renderTarget = mRenderTexture;
		viewport.offset = { 0.0f, 0.0f };
		viewport.dimensions = { 1.0f, 1.0f };
		registry.emplace<Viewport>(mCameraEntity, viewport);

		Camera camera;
		camera.viewport = mCameraEntity;
		registry.emplace<Camera>(mCameraEntity, camera);
	}

	void SceneViewerTool::onOpen(entt::registry& registry) {

	}

	void SceneViewerTool::onUpdate(entt::registry& registry) {
		using namespace Gfx;

		auto selectedSceneRootView = registry.view<SelectedSceneRoot>();
		if (selectedSceneRootView.empty()) {
			return;
		}

		if (!registry.all_of<Camera>(mCameraEntity)) {
			return;
		}

		registry.get<Camera>(mCameraEntity).sceneRoot = selectedSceneRootView.front();

		ImGui::Image(static_cast<ImTextureID>(mRenderTexture), ImVec2{ 1360, 768 });
	}

	void SceneViewerTool::onClose(entt::registry& registry) {

	}

} // namespace DevTools
