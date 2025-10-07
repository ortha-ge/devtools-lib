module;

#include <rpp/rpp.hpp>

export module Ortha.DevTools.MaterialEditorTool;

import Ortha.Core.ResourceHandle;
import Ortha.Gfx.MaterialDescriptor;
import entt;
import glm;

export namespace Ortha::DevTools {

	class MaterialEditorTool {
	public:

		MaterialEditorTool(entt::registry&);

		void setupTool(entt::registry&);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();

		void setMaterialDescriptor(entt::registry&, const Gfx::MaterialDescriptor&);

		void _printExportedMaterialJSON(entt::registry&, const Gfx::MaterialDescriptor&);

		entt::entity mRenderTexture{ entt::null };
		entt::entity mMaterialRenderObjectRoot{ entt::null };
		rpp::subjects::behavior_subject<Gfx::MaterialDescriptor> mMaterialDescriptor;

		rpp::schedulers::run_loop mRunLoop;
		rpp::composite_disposable mSubscriptions;

		glm::vec2 mMaterialRenderPos;
		glm::vec2 mMaterialRenderScale;
	};

} // namespace Ortha::DevTools
