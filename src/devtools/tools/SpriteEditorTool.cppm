module;

#include <string>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <glm/vec3.hpp>
#include <rpp/rpp.hpp>

export module DevTools.SpriteEditorTool;

import Core.EnTTRegistry;
import Core.ResourceHandle;
import Gfx.SpriteDescriptor;

export namespace DevTools {

	class SpriteEditorTool {
	public:

		SpriteEditorTool(Core::EnTTRegistry&);

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
		rpp::subjects::behavior_subject<Gfx::SpriteDescriptor> mSprite;

		rpp::schedulers::run_loop mRunLoop;
		rpp::composite_disposable mSubscriptions;

	};

} // namespace DevTools
