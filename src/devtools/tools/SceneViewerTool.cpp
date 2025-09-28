module;

#define GLM_ENABLE_EXPERIMENTAL

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

module DevTools.SceneViewerTool;

import Core.EnTTNode;
import Core.GlobalSpatial;
import Core.Spatial;
import DevTools.SelectedEntity;
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
		renderTexture.width = 1920;
		renderTexture.height = 1080;
		registry.emplace<RenderTexture>(mRenderTexture, renderTexture);

		// Camera + Viewport
		mCameraEntity = createEnTTNode(registry, "DevCamera");
		registry.emplace<Spatial>(mCameraEntity, glm::vec3{}, glm::vec3{ 1.0f, 1.0f,  1.0f });

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

		/* Camera Gizmo View/Model/Projection
		 * - View matrix shouldn't be scaled otherwise the gizmos get scaled.
		 * - Window position has to be add/subtracted from final gizmo position.
		 */
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetRect(0, 0, 1920.0f, 1080.0f);

		const ImVec2 cursorPos{ ImGui::GetCursorScreenPos() };
		//ImGui::SetNextItemAllowOverlap();
		ImGui::Image(static_cast<ImTextureID>(mRenderTexture), ImVec2{ 1920, 1080 });
		// ImGui::SetCursorScreenPos(cursorPos);
		// ImGui::InvisibleButton("SceneViewClickMask", ImVec2(1920.0f, 1080.0f));

		const ImVec2 windowPos{ ImGui::GetWindowPos() };
		glm::mat4 viewMatrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ windowPos.x, windowPos.y, 0.0f });

		glm::mat4 projectionMatrix{ glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 1000.0f) };


		auto selectedEntityView = registry.view<SelectedEntity>();
		if (!selectedEntityView.empty()) {
			const entt::entity selectedEntity = selectedEntityView.front();
			if (registry.all_of<GlobalSpatial>(selectedEntity)) {
				const auto& selectedEntityGlobalSpatial{ registry.get<GlobalSpatial>(selectedEntity) };
				glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
				glm::mat4 cameraScale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);

				glm::mat4 entityTranslation{ glm::translate(glm::mat4(1.0f), selectedEntityGlobalSpatial.position) };
				glm::mat4 entityScale{ glm::scale(glm::mat4(1.0f), selectedEntityGlobalSpatial.scale) };
				mGizmoTransform = glm::inverse(cameraTranslation * cameraScale) * entityTranslation;
				//mGizmoTransform = glm::inverse(translation) * mGizmoTransform;
				ImGuizmo::Manipulate(&viewMatrix[0][0], &projectionMatrix[0][0], ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, &mGizmoTransform[0][0]);

				mGizmoTransform = mGizmoTransform * (cameraTranslation * cameraScale);

				if (ImGuizmo::IsUsing()) {
					auto& selectedEntitySpatial{ registry.get<Spatial>(selectedEntity) };
					glm::quat rot;
					glm::vec3 scale;
					glm::vec3 sqew;
					glm::vec4 perspective;
					glm::decompose(mGizmoTransform, scale, rot, selectedEntitySpatial.position, sqew, perspective);
				}
			}
		}





	}

	void SceneViewerTool::onClose(entt::registry& registry) {

	}

} // namespace DevTools
