module;

#include <memory>

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>
#include <glm/vec3.hpp>
#include <rpp/rpp.hpp>

export module DevTools.MaterialEditorTool;

import Core.ResourceHandle;
import Gfx.MaterialDescriptor;

export namespace DevTools {

	class MaterialEditorTool {
	public:

		MaterialEditorTool();

		void setup(entt::registry& registry);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();

		void setMaterialDescriptor(entt::registry&, const Gfx::MaterialDescriptor&);

		void _printExportedMaterialJSON(entt::registry&, const Gfx::MaterialDescriptor&);

		entt::entity mMaterialRenderObjectRoot{ entt::null };
		rpp::subjects::behavior_subject<Gfx::MaterialDescriptor> mMaterialDescriptor;

		rpp::schedulers::run_loop mRunLoop;
		rpp::composite_disposable mSubscriptions;

		glm::vec3 mMaterialRenderPos;
	};

} // namespace DevTools
