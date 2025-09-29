module;

#include <memory>
#include <vector>

#include <imgui.h>

module DevTools.NodeTool;

import Core.EnTTNode;
import Core.Node;
import Core.TypeId;
import DevTools.SelectedEntity;
import DevTools.SelectedSceneRoot;
import DevTools.Tool;
import entt;

namespace DevTools::NodeToolInternal {

	void selectNodeEntity(entt::registry& registry, Core::Node::Ptr node) {
		using namespace Core;

		if (node->getTypeId() != TypeId::get<EnTTNode>()) {
			return;
		}

		auto enttNode = std::static_pointer_cast<EnTTNode>(node);
		const entt::entity nodeEntity = enttNode->getEntity();

		registry.clear<SelectedEntity>();
		if (!registry.valid(nodeEntity)) {
			return;
		}

		registry.emplace<SelectedEntity>(nodeEntity);
	}

	void selectSceneRootEntity(entt::registry& registry, Core::Node::Ptr node) {
		using namespace Core;

		if (node->getTypeId() != TypeId::get<EnTTNode>()) {
			return;
		}

		auto enttNode = std::static_pointer_cast<EnTTNode>(node);
		const entt::entity nodeEntity = enttNode->getEntity();

		registry.clear<SelectedSceneRoot>();
		if (!registry.valid(nodeEntity)) {
			return;
		}

		registry.emplace<SelectedSceneRoot>(nodeEntity);
	}

	void recurseNodeTree(entt::registry& registry, Core::Node::Ptr node) {
		if (ImGui::TreeNode(node->getName().c_str())) {
			if (ImGui::Button("Select")) {
				selectNodeEntity(registry, node);
			}

			if (ImGui::Button("Set As Scene Root")) {
				selectSceneRootEntity(registry, node);
			}

			for (auto&& childNode : node->getChildren()) {
				recurseNodeTree(registry, childNode);
			}

			ImGui::TreePop();
		}
	}

} // namespace DevTools::NodeToolInternal

namespace DevTools {

	NodeTool::NodeTool(entt::registry& registry) {
		setupTool(registry);
	}

	void NodeTool::setupTool(entt::registry& registry) {
		const entt::entity toolEntity = registry.create();
		registry.emplace<Tool>(toolEntity, Tool {
			.toolName = "Nodes",
			.openFunction = [this](entt::registry& registry, Tool&) { onOpen(registry); },
			.updateFunction = [this](entt::registry& registry, Tool&) { onUpdate(registry); },
			.closeFunction = [this](entt::registry& registry, Tool&) { onClose(registry); },
			.isOpen = true
		});
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
