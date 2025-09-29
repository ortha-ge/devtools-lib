module;

#include <format>
#include <memory>
#include <vector>

#include <imgui.h>

module DevTools.NodeTool;

import Core.Any;
import Core.EnTTNode;
import Core.JsonTypeSaverAdapter;
import Core.Log;
import Core.Node;
import Core.NodeHandle;
import Core.TypeId;
import Core.TypeSaver;
import Core.Reflection.Node;
import DevTools.SelectedEntity;
import DevTools.SelectedSceneRoot;
import DevTools.Tool;
import entt;

namespace DevTools::NodeToolInternal {

	constexpr const char* PrintExportFormatString = R"(Node JSON
=============
{})";

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

	void exportNodeJSON(entt::registry& registry, Core::Node::Ptr node) {
		std::string exportedJSON{ Core::save(registry, Core::Any{ *node.get() }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

	void recurseNodeTree(entt::registry& registry, Core::Node::Ptr node, size_t idIndex = 0u) {
		ImGui::PushID(std::format("{}_{}", node->getName(), idIndex).c_str());
		const bool isTreeOpen = ImGui::TreeNode(node->getName().c_str());

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::Button("Select")) {
				selectNodeEntity(registry, node);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Set As Scene Root")) {
				selectSceneRootEntity(registry, node);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Export Node JSON")) {
				exportNodeJSON(registry, node);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (isTreeOpen) {
			const auto& children{ node->getChildren() };
			for (size_t i = 0; i < children.size(); ++i) {
				recurseNodeTree(registry, children[i], i);
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
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
