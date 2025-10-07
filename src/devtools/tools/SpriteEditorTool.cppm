module;

#include <string>

#include <rpp/rpp.hpp>

export module Ortha.DevTools.SpriteEditorTool;

import Ortha.Core.ResourceHandle;
import Ortha.Gfx.SpriteDescriptor;
import entt;
import glm;

export namespace Ortha::DevTools {

	class SpriteEditorTool {
	public:

		SpriteEditorTool(entt::registry&);

		void setupTool(entt::registry&);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();

		void loadMaterialResource(entt::registry&, std::string) const;
		void setSprite(entt::registry&, Gfx::SpriteDescriptor) const;

		static void _printSavedSpriteJSON(entt::registry&, const Gfx::SpriteDescriptor&);

		entt::entity mSpriteObjectRoot{ entt::null };
		entt::entity mRenderTexture{ entt::null };

		glm::vec3 mSpriteRenderPos{ 225.0f, 150.0f, 10.0f };
		uint32_t mSelectedFrame{ 0u };

		rpp::subjects::behavior_subject<std::string> mMaterialResourceFilePath;
		rpp::subjects::behavior_subject<Ortha::Gfx::SpriteDescriptor> mSprite;

		rpp::schedulers::run_loop mRunLoop;
		rpp::composite_disposable mSubscriptions;

	};

} // namespace Ortha::DevTools
