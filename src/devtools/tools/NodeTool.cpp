module;

#include <entt/entt.hpp>
#include <imgui.h>

module DevTools.NodeTool;

import Core.EnTTNode;
import Core.Node;
import Core.TypeId;
import DevTools.SelectedEntity;
import DevTools.Tool;

namespace DevTools::NodeToolInternal {

	void selectNodeEntity(entt::registry& registry, Core::Node::Ptr node) {
		using namespace Core;

		if (node->getTypeId() != TypeId::get<EnTTNode>()) {
			return;
		}

		auto enttNode = std::static_pointer_cast<EnTTNode>(node);
		const entt::entity nodeEntity = enttNode->getEntity();

		registry.clear<SelectedEntity>();
		registry.emplace<SelectedEntity>(nodeEntity);
	}

	void recurseNodeTree(entt::registry& registry, Core::Node::Ptr node) {
		if (ImGui::TreeNode(node->getName().c_str())) {
			if (ImGui::Button("Select")) {
				selectNodeEntity(registry, node);
			}

			for (auto&& childNode : node->getChildren()) {
				recurseNodeTree(registry, childNode);
			}

			ImGui::TreePop();
		}
	}

} // namespace DevTools::NodeToolInternal

namespace DevTools {

	NodeTool::NodeTool(Core::EnTTRegistry& registry) {
		setupTool(registry);
	}

	void NodeTool::setupTool(entt::registry& registry) {
		const entt::entity toolEntity = registry.create();
		registry.emplace<Tool>(toolEntity, "Nodes",
			[this](entt::registry& registry, Tool&) { onOpen(registry); },
			[this](entt::registry& registry, Tool&) { onUpdate(registry); },
			[this](entt::registry& registry, Tool&) { onClose(registry); }
		);
	}

	void NodeTool::onOpen(entt::registry&) {

	}

	void NodeTool::onUpdate(entt::registry& registry) {
		using namespace Core;
		using namespace NodeToolInternal;

		std::vector<Node::Ptr> rootNodes;
		registry.view<NodeHandle>()
			.each([&rootNodes](const NodeHandle& nodeHandle) {
				if (!nodeHandle->getParentNode()) {
					rootNodes.push_back(nodeHandle.getNode());
				}
			});

		if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
			for (auto&& rootNode : rootNodes) {
				recurseNodeTree(registry, rootNode);
			}
		}

		ImGui::EndChild();
	}

	void NodeTool::onClose(entt::registry&) {

	}

} // namespace DevTools
