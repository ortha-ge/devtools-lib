module;

#include <format>
#include <memory>
#include <vector>

#include <imgui.h>
#include <imgui_stdlib.h>

module Ortha.DevTools.NodeTool;

import Ortha.RTTI.Any;
import Ortha.Core.EnTTNode;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.JsonTypeSaverAdapter;
import Ortha.Core.Log;
import Ortha.Core.Node;
import Ortha.Core.NodeHandle;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.ReflectionContextStack;
import Ortha.Core.Reflection.Node;
import Ortha.RTTI.TypeId;
import Ortha.Core.TypeLoader;
import Ortha.Core.TypeSaver;
import Ortha.DevTools.SelectedEntity;
import Ortha.DevTools.SelectedSceneRoot;
import Ortha.DevTools.Tool;
import entt;

namespace Ortha::DevTools::NodeToolInternal {

	constexpr const char* PrintExportFormatString = R"(Node JSON
=============
{})";

	void selectNodeEntity(entt::registry& registry, Core::Node::Ptr node) {
		using namespace Ortha::Core;
		using namespace RTTI;

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
		using namespace Ortha::Core;
		using namespace RTTI;

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
		using namespace RTTI;
		std::string exportedJSON{ Core::save(registry, Any{ *node.get() }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace Ortha::DevTools::NodeToolInternal

namespace Ortha::DevTools {

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
		using namespace Ortha::Core;
		using namespace NodeToolInternal;

		std::vector<Node::Ptr> rootNodes;
		registry.view<NodeHandle>()
			.each([&rootNodes](const NodeHandle& nodeHandle) {
				if (!nodeHandle->getParentNode()) {
					rootNodes.push_back(nodeHandle.getNode());
				}
			});



		const bool scrollingReturnsTrue = ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Button("New Root Node")) {
				createEnTTNode(registry, "New Node");
			}


			bool addNodePressed = ImGui::Button("Add Node from JSON");

			if (ImGui::BeginPopupModal("Node JSON")) {
				if (ImGui::InputTextMultiline("JSON", &mLoadNodeJsonString)) {

				}

				if (ImGui::Button("Load")) {
					auto node = std::make_shared<Node>();
					RTTI::Any nodeAny{ *node.get() };

					const auto& reflectionContext{ static_cast<RTTI::ReflectionContext&>(RTTI::getCurrentReflectionContext()) };
					load(registry, reflectionContext, mLoadNodeJsonString, nodeAny);

					const entt::entity nodeHandle = registry.create();
					registry.emplace<NodeHandle>(nodeHandle, node);

					mLoadNodeJsonString.clear();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (addNodePressed) {
				ImGui::OpenPopup("Node JSON");
			}

			ImGui::EndPopup();
		}

		if (scrollingReturnsTrue) {
			for (size_t i = 0; i < rootNodes.size(); ++i) {
				recurseNodeTree(registry, rootNodes[i], i);
			}
		}

		ImGui::EndChild();
	}

	void NodeTool::onClose(entt::registry&) {

	}

	void NodeTool::recurseNodeTree(entt::registry& registry, Core::Node::Ptr node, size_t idIndex) {
		using namespace Ortha::Core;
		using namespace NodeToolInternal;

		ImGui::PushID(std::format("{}_{}", node->getName(), idIndex).c_str());
		const bool isTreeOpen = ImGui::TreeNode(node->getName().c_str());

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::Button("Select")) {
				selectNodeEntity(registry, node);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Rename")) {

			}
			ImGui::OpenPopupOnItemClick(0, ImGuiPopupFlags_MouseButtonLeft);

			if (ImGui::BeginPopupModal("Rename")) {
				ImGui::InputText("Node Name", &mRenameNodeString);

				if (ImGui::Button("Rename")) {
					node->setName(mRenameNodeString);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button("Add New Child Node")) {
				createChildEnTTNode(registry, node, "New Node");
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
			for (auto it = children.begin(); it != children.end(); ++it) {
				recurseNodeTree(registry, (*it), std::distance(children.begin(), it));
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

} // namespace Ortha::DevTools
