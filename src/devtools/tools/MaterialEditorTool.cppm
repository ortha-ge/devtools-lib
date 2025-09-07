module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module DevTools.MaterialEditorTool;

export namespace DevTools {

	class MaterialEditorTool {
	public:
		void setup(entt::registry& registry);

	private:
		void _printExportedMaterialJSON(entt::registry& registry);

		bool _hasValidMaterialResource(entt::registry& registry) const;

		entt::entity mMaterialRenderObjectEntity{ entt::null };
		entt::entity mMaterialResourceEntity{ entt::null };
	};

} // namespace DevTools
