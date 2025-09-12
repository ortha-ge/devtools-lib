module;

#include <string>

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <glm/vec3.hpp>

export module DevTools.SpriteEditorTool;

import Core.ResourceHandle;
import Gfx.Sprite;

export namespace DevTools {

	class SpriteEditorTool {
	public:

		void setup(entt::registry&);

	private:

		std::string mMaterialResourceFilePath;
		Gfx::Sprite mSprite;

		entt::entity mSpriteObjectRoot{ entt::null };
		glm::vec3 mSpriteRenderPos{ 225.0f, 150.0f, 10.0f };
		uint32_t mSelectedFrame{ 0u };

	};

} // namespace DevTools
