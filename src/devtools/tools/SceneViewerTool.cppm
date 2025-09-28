module;

#include <entt/entt.hpp>
#include <glm/mat4x4.hpp>

export module DevTools.SceneViewerTool;

import Core.EnTTRegistry;

export namespace DevTools {

	class SceneViewerTool {
	public:

		SceneViewerTool(Core::EnTTRegistry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

		entt::entity mRenderTexture{ entt::null };
		entt::entity mCameraEntity{ entt::null };

		glm::mat4 mGizmoTransform{ 1.0f };

	};

} // namespace DevTools
