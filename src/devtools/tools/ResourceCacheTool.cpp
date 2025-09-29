module;

#include <chrono>

#include <imgui.h>

module DevTools.ResourceCacheTool;

import Core.ResourceCache;
import DevTools.Tool;
import entt;

namespace DevTools {

	ResourceCacheTool::ResourceCacheTool(entt::registry& registry) {
		setupTool(registry);
	}

	void ResourceCacheTool::setupTool(entt::registry& registry) {
		const entt::entity tool{ registry.create() };
		registry.emplace<Tool>(tool, "Resource Cache",
			[this](entt::registry& registry, Tool&) { open(registry); },
			[this](entt::registry& registry, Tool&) { update(registry); },
			[this](entt::registry& registry, Tool&) { destroy(registry); }
		);
	}

	void ResourceCacheTool::open(entt::registry&) {}
	void ResourceCacheTool::update(entt::registry& registry) {
		registry.view<Core::ResourceCache>()
			.each([](Core::ResourceCache& resourceCache) {
				int32_t cleanuptimeSecondsMax = 300;
				int32_t cleanupTimeSeconds{ static_cast<int32_t>(std::min<long>(cleanuptimeSecondsMax, resourceCache.getCleanupTriggerTimeSeconds().count())) };
				cleanupTimeSeconds = std::max(1, cleanupTimeSeconds);
				if (ImGui::SliderInt("Free timer", &cleanupTimeSeconds, 1, cleanuptimeSecondsMax)) {
					resourceCache.setCleanupTriggerTimeSeconds(std::chrono::seconds(cleanupTimeSeconds));
				}

				for (auto&& [resourceFilePath, resource] : resourceCache.getResourceLookup()) {
					if (ImGui::TreeNode(resourceFilePath.c_str())) {
						ImGui::Text("Use count: %ld", resource.use_count() - 1);
						ImGui::TreePop();
					}
				}
			});
	}
	void ResourceCacheTool::destroy(entt::registry&) {}
	void ResourceCacheTool::registerSubscriptions(entt::registry&) {}
	void ResourceCacheTool::releaseSubscriptions() {}
} // namespace DevTools
