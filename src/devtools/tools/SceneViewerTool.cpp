module;

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

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
		using namespace Core;
		using namespace Gfx;

		auto selectedSceneRootView = registry.view<SelectedSceneRoot>();
		if (selectedSceneRootView.empty()) {
			return;
		}

		if (!registry.all_of<Camera, Spatial>(mCameraEntity)) {
			return;
		}

		const auto& [cameraSpatial, camera] = registry.get<Spatial, Camera>(mCameraEntity);
		camera.sceneRoot = selectedSceneRootView.front();

		ImGui::Image(static_cast<ImTextureID>(mRenderTexture), ImVec2{ 1360, 768 });

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
		glm::mat4 rotation = glm::mat4_cast(cameraSpatial.rotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);
		glm::mat4 viewMatrix{ 1.0f };

		glm::mat4 projectionMatrix{ glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 1000.0f) };
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetRect(0, 0, 1920.0f, 1080.0f);

		auto& translationVec = mGizmoTransform[3];
		ImGui::DragFloat3("Gizmo Pos", &translationVec[0]);

		ImGuizmo::Manipulate(&viewMatrix[0][0], &projectionMatrix[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, &mGizmoTransform[0][0]);

		glm::mat4 cubeMatrix{ 1.0f };
		ImGuizmo::DrawCubes(&viewMatrix[0][0], &projectionMatrix[0][0], &cubeMatrix[0][0], 1);
	}

	void SceneViewerTool::onClose(entt::registry& registry) {

	}

} // namespace DevTools
