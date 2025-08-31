module;

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

export module DevTools.MaterialResourceEditTool;

export namespace DevTools {

    class MaterialResourceEditTool {
    public:

        void setup(entt::registry& registry);

    private:

		void _printExportedMaterialJSON(entt::registry& registry);

        entt::entity mMaterialRenderObjectEntity{ entt::null };
        entt::entity mMaterialResourceEntity{ entt::null };

    };

} // DevTools
