module;

#include <format>
#include <memory>
#include <vector>

#include <imgui.h>
#include <imgui_stdlib.h>

module DevTools.NodeTool;

import Ortha.RTTI.Any;
import Core.EnTTNode;
import Core.JsonTypeLoaderAdapter;
import Core.JsonTypeSaverAdapter;
import Core.Log;
import Core.Node;
import Core.NodeHandle;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.ReflectionContextStack;
import Core.Reflection.Node;
import Ortha.RTTI.TypeId;
import Core.TypeLoader;
import Core.TypeSaver;
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

		if (node->getTypeId() != Ortha::RTTI::TypeId::get<EnTTNode>()) {
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

		if (node->getTypeId() != Ortha::RTTI::TypeId::get<EnTTNode>()) {
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
		std::string exportedJSON{ Core::save(registry, Ortha::RTTI::Any{ *node.get() }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
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
					Ortha::RTTI::Any nodeAny{ *node.get() };

					const auto& reflectionContext{ static_cast<Ortha::RTTI::ReflectionContext&>(Ortha::RTTI::getCurrentReflectionContext()) };
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
		using namespace Core;
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
			for (size_t i = 0; i < children.size(); ++i) {
				recurseNodeTree(registry, children[i], i);
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

} // namespace DevTools
