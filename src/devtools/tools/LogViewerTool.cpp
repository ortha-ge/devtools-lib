module;

#include <string>

#include <entt/entt.hpp>
#include <imgui.h>

module DevTools.LogViewerTool;

import Core.Log;
import DevTools.Tool;

namespace DevTools {

	void LogViewerTool::setup(entt::registry& registry) {
		const entt::entity logViewerTool{ registry.create() };
		registry.emplace<Tool>(logViewerTool, "Log Viewer", [this](entt::registry& registry, Tool& tool) {
			if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
				registry.view<Core::Log>()
					.each([](const Core::Log& log) {
						for (auto&& logMessage : log.messages) {
							ImGui::TextUnformatted(logMessage.message.c_str());
						}
					});
			}

			ImGui::EndChild();
		});
	}

} // namespace DevTools
