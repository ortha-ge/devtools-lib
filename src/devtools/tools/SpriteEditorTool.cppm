module;

#include <optional>
#include <string>
#include <utility>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <glm/vec3.hpp>
#include <rxcpp/rx.hpp>

export module DevTools.SpriteEditorTool;

import Core.ResourceHandle;
import Gfx.Sprite;

export namespace DevTools {

	class SpriteEditorTool {
	public:

		SpriteEditorTool();

		void setup(entt::registry&);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();

		rxcpp::observable<std::string> observeMaterialResourceFilePath() const;
		rxcpp::observable<Gfx::Sprite> observeSprite() const;

		void loadMaterialResource(entt::registry&, std::string) const;
		void setSprite(entt::registry&, Gfx::Sprite) const;

		entt::entity mSpriteObjectRoot{ entt::null };
		glm::vec3 mSpriteRenderPos{ 225.0f, 150.0f, 10.0f };
		uint32_t mSelectedFrame{ 0u };

		rxcpp::subjects::behavior<std::string> mMaterialResourceFilePath;
		rxcpp::subjects::behavior<Gfx::Sprite> mSprite;
		rxcpp::composite_subscription mSubscriptions;
		rxcpp::schedulers::run_loop mRunLoop;

	};

} // namespace DevTools
