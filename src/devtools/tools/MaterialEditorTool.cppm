module;

#include <memory>

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

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
	};

} // namespace DevTools
