module;

#include <string>

#include <imgui.h>

module Ortha.DevTools.LogViewerTool;

import Ortha.Core.Log;
import Ortha.DevTools.Tool;
import entt;

namespace Ortha::DevTools {

	LogViewerTool::LogViewerTool(entt::registry& registry) {
		setupTool(registry);
	}

	void LogViewerTool::setupTool(entt::registry& registry) {
		const entt::entity logViewerTool{ registry.create() };
		registry.emplace<Tool>(logViewerTool, "Log Viewer",
			[this](entt::registry& registry, Tool&) { onOpen(registry); },
			[this](entt::registry& registry, Tool&) { onUpdate(registry); },
			[this](entt::registry& registry, Tool&) { onClose(registry); }
		);
	}

	void LogViewerTool::onOpen(entt::registry&) {

	}

	void LogViewerTool::onUpdate(entt::registry& registry) {
		if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
			registry.view<Core::Log>()
				.each([](const Core::Log& log) {
					for (auto&& logMessage : log.messages) {
						ImGui::TextUnformatted(logMessage.message.c_str());
					}
				});
		}

		ImGui::EndChild();
	}

	void LogViewerTool::onClose(entt::registry&) {

	}

} // namespace Ortha::DevTools
