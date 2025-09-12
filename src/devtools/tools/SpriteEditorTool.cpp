module;

#include <entt/entt.hpp>

module DevTools.SpriteEditorTool;

import DevTools.Tool;

namespace DevTools {

	void SpriteEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Sprite Editor", [this](entt::registry& registry, Tool& tool) {

		});
	}

} // namespace DevTools
