module;

#include <memory>

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>
#include <glm/vec3.hpp>

export module DevTools.MaterialEditorTool;

import Core.ResourceHandle;

export namespace DevTools {

	class MaterialEditorTool {
	public:
		void setup(entt::registry& registry);

	private:
		void _printExportedMaterialJSON(entt::registry& registry);

		bool _hasValidMaterialResource(entt::registry& registry) const;

		entt::entity mMaterialRenderObjectEntity{ entt::null };
		std::shared_ptr<Core::ResourceHandle> mMaterialResource{};
		glm::vec3 mMaterialRenderPos{ 225.0f, 150.0f, 10.0f };
	};

} // namespace DevTools
