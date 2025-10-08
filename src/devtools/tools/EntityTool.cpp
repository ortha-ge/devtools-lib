module;

#include <imgui.h>
#include <imgui_stdlib.h>

module Ortha.DevTools.EntityTool;

import Ortha.Core.EnTTComponentAttribute;
import Ortha.Core.Log;
import Ortha.Core.Node;
import Ortha.Core.NodeHandle;
import Ortha.Core.Spatial;
import Ortha.DevTools.SelectedEntity;
import Ortha.DevTools.Tool;
import Ortha.RTTI.ReflectionContext;
import Ortha.RTTI.TypeId;
import Ortha.RTTI.TypeProperty;

namespace Ortha::DevTools::EntityToolInternal {

	template <typename T>
	void _renderComponentProperty(const char* propertyName, T& property) {
		ImGui::TextUnformatted(propertyName);
	}

	template <>
	void _renderComponentProperty(const char* propertyName, int& property) {
		ImGui::InputInt(propertyName, &property);
	}

	template <>
	void _renderComponentProperty(const char* propertyName, float& property) {
		ImGui::InputFloat(propertyName, &property);
	}

	template <>
	void _renderComponentProperty(const char* propertyName, bool& property) {
		ImGui::Checkbox(propertyName, &property);
	}

	template <>
	void _renderComponentProperty(const char* propertyName, std::string& property) {
		ImGui::InputText(propertyName, &property);
	}

	template <typename PropertyType>
	bool _renderBasicComponentProperty(const RTTI::TypeProperty& property, void* instance) {
		using namespace Ortha::Core;
		using namespace RTTI;
		if (property.getTypeId() == TypeId::get<PropertyType>()) {
			//_renderComponentProperty(property.getName().c_str(), property.apply<PropertyType>(instance));
			return true;
		}

		return false;
	}

	template <typename Tail>
	void _tryRenderBasicComponentProperty(const RTTI::TypeProperty& property, void* instance) {

	}

	template <typename Tail, typename PropertyType, typename... PropertyTypes>
	void _tryRenderBasicComponentProperty(const RTTI::TypeProperty& property, void* instance) {
		if (_renderBasicComponentProperty<PropertyType>(property, instance)) {
			return;
		}
		_tryRenderBasicComponentProperty<Tail, PropertyTypes...>(property, instance);
	}

	void tryRenderBasicComponentProperty(const RTTI::TypeProperty& property, void* instance) {
		using namespace Ortha::Core;

		_tryRenderBasicComponentProperty<void, float, int, bool, std::string>(property, instance);
	}

	void renderComponentProperty(const RTTI::ReflectionContext& reflectionContext, const RTTI::TypeProperty& property, void* instance) {
		// if (reflectionContext.hasClass(property.getTypeId())) {
		// 	ImGui::SeparatorText(property.getName().c_str());
		// 	ImGui::PushID(property.getName().c_str());
		// 	void* propertyInstance = property.getRawPointer(instance);
		//
		// 	const auto& classReflection{ reflectionContext.getClass(property.getTypeId()) };
		// 	for (auto&& childProperty : classReflection.getProperties()) {
		// 		renderComponentProperty(reflectionContext, childProperty, propertyInstance);
		// 	}
		// 	ImGui::PopID();
		// } else if (reflectionContext.hasEnum(property.getTypeId())) {
		//
		// } else if (reflectionContext.hasBasicType(property.getTypeId())){
		// 	tryRenderBasicComponentProperty(property, instance);
		// } else {
		// 	return;
		// }
	}

	void renderEntityTab(entt::registry& registry, const entt::entity entity) {
		using namespace Ortha::Core;

		std::string entityName{ "Unnamed Entity" };
		if (registry.all_of<NodeHandle>(entity)) {
			entityName = registry.get<NodeHandle>(entity)->getName();
		}

		if (ImGui::BeginTabItem(entityName.c_str())) {
			// const auto& reflectionContext{ getCurrentReflectionContext() };
			// reflectionContext.forEachClass([&reflectionContext, &registry, entity](const ClassReflection& classReflection) {
			// 	if (classReflection.hasAttribute<EnTTComponentAttribute>()) {
			// 		const auto& enttAnnotation{ classReflection.getAttribute<EnTTComponentAttribute>() };
			// 		if (void* componentInstance = enttAnnotation.getComponent(registry, entity)) {
			// 			if (ImGui::CollapsingHeader(classReflection.getName().c_str())) {
			// 				ImGui::PushID(classReflection.getName().c_str());
			// 				for (auto&& property : classReflection.getProperties()) {
			// 					renderComponentProperty(reflectionContext, property, componentInstance);
			// 				}
			// 				ImGui::PopID();
			// 			}
			// 		}
			// 	}
			// });

			ImGui::BeginDisabled();
			if (ImGui::Button("Add Component")) {

			}
			ImGui::EndDisabled();

			ImGui::EndTabItem();
		}
	}

} // namespace Ortha::DevTools::EntityToolInternal

namespace Ortha::DevTools {



	EntityTool::EntityTool(entt::registry& registry) {
		setupTool(registry);
	}

	void EntityTool::setupTool(entt::registry& registry) {
		const entt::entity toolEntity = registry.create();
		registry.emplace<Tool>(toolEntity, "Entity",
			[this](entt::registry& registry, Tool&) { onOpen(registry); },
			[this](entt::registry& registry, Tool&) { onUpdate(registry); },
			[this](entt::registry& registry, Tool&) { onClose(registry); }
		);
	}

	void EntityTool::onOpen(entt::registry&) {

	}

	void EntityTool::onUpdate(entt::registry& registry) {
		using namespace Ortha::Core;
		using namespace EntityToolInternal;

		auto selectedEntityView = registry.view<SelectedEntity>();
		if (selectedEntityView.size() > 1) {
			logEntry(registry, "Currently only a single selected entity is supported.");
		}

		if (ImGui::BeginTabBar("Entities")) {
			selectedEntityView.each([&registry](const entt::entity entity) {
				renderEntityTab(registry, entity);
			});

			ImGui::EndTabBar();
		}
	}

	void EntityTool::onClose(entt::registry&) {

	}

} // namespace Ortha::DevTools
