module;

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>

module DevTools.MaterialEditorTool;

import Core.Any;
import Core.JsonTypeLoaderAdapter;
import Core.JsonTypeSaverAdapter;
import Core.Log;
import Core.ResourceHandle;
import Core.ResourceHandleUtils;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import Core.TypeSaver;
import DevTools.Tool;
import Gfx.Camera;
import Gfx.RenderObject;
import Gfx.Sprite;
import Gfx.SpriteObject;
import Gfx.Material;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.Sprite;

namespace DevTools {

	constexpr const char* PrintExportFormatString = R"(Material JSON
=============
%s)";

	bool MaterialEditorTool::_hasValidMaterialResource(entt::registry& registry) const {
		if (!mMaterialResource) {
			return false;
		}

		const auto&& [resourceEntity, material] = Core::getResourceAndEntity<Gfx::Material>(registry, mMaterialResource);
		if (resourceEntity == entt::null) {
			return false;
		}

		if (!material) {
			return false;
		}

		return true;
	}

	void MaterialEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Material Editor", [this](Tool& tool, entt::registry& registry) {
			if (ImGui::Begin(tool.toolName.c_str(), &tool.isOpen)) {
				const bool hasValidMaterial = _hasValidMaterialResource(registry);

				if (mMaterialResource && (!tool.isOpen || !hasValidMaterial)) {
					mMaterialResource = {};
				}

				if (mMaterialRenderObjectEntity != entt::null && (!tool.isOpen || !hasValidMaterial)) {
					registry.destroy(mMaterialRenderObjectEntity);
					mMaterialRenderObjectEntity = entt::null;
				}

				if (tool.isOpen && !hasValidMaterial) {
					mMaterialResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
						registry, "assets/materials/cat.json",
						std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

					Gfx::Sprite spriteResource {};
					spriteResource.frames = {
						{ { 0.0f, 0.0f }, { 100.0f, 100.0f } }
					};

					mMaterialRenderObjectEntity = registry.create();
					registry.emplace<Core::Spatial>(mMaterialRenderObjectEntity, glm::vec3{ 50.0f, 50.0f, 50.0f });
					registry.emplace<Gfx::SpriteObject>(mMaterialRenderObjectEntity, spriteResource);
					registry.emplace<Gfx::RenderObject>(mMaterialRenderObjectEntity, mMaterialResource);
				}

				if (!mMaterialResource) {
					ImGui::End();
					return;
				}

				auto&& [materialEntity, materialDescriptor] = Core::getResourceAndEntity<Gfx::MaterialDescriptor>(registry, mMaterialResource);
				if (!materialDescriptor) {
					ImGui::End();
					return;
				}

				if (mMaterialRenderObjectEntity == entt::null || !registry.all_of<Gfx::RenderObject>(mMaterialRenderObjectEntity)) {
					ImGui::End();
					return;
				}

				auto cameraView = registry.view<Gfx::Camera>();
				if (cameraView.empty()) {
					return;
				}

				const auto& cameraEntity = cameraView.front();
				if (!registry.all_of<Core::Spatial>(cameraEntity)) {
					return;
				}

				const auto& cameraSpatial = registry.get<Core::Spatial>(cameraEntity);

				auto& renderObject{ registry.get<Gfx::RenderObject>(mMaterialRenderObjectEntity) };
				auto& renderObjectSpatial{ registry.get<Core::Spatial>(mMaterialRenderObjectEntity) };
				renderObjectSpatial.position = cameraSpatial.position + mMaterialRenderPos;
				bool changed{ false };

				if (ImGui::InputFloat3("Material Render Pos", &mMaterialRenderPos[0])) {

				}

				if (ImGui::InputText("Shader Program File-Path", &materialDescriptor->shaderProgramFilePath)) {
					changed = true;
				}

				if (ImGui::InputText("Texture Image File-Path", &materialDescriptor->textureImageFilePath)) {
					changed = true;
				}

				auto alphaColour{ materialDescriptor->alphaColour.value_or(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }) };
				if (ImGui::ColorPicker4("Transparency Colour", &alphaColour[0])) {
					materialDescriptor->alphaColour = alphaColour;
					changed = true;
				}

				if (ImGui::Button("Clear Transparency Colour")) {
					materialDescriptor->alphaColour = std::nullopt;
					changed = true;
				}

				if (ImGui::Button("Export JSON", ImVec2(100, 15))) {
					_printExportedMaterialJSON(registry);
				}

				if (changed && registry.all_of<Gfx::Material>(materialEntity)) {
					registry.remove<Gfx::Material>(materialEntity);
				}
			}

			ImGui::End();
		});
	}

	void MaterialEditorTool::_printExportedMaterialJSON(entt::registry& registry) {
		if (!mMaterialResource) {
			Core::logEntry(registry, "No Material Resource.");
			return;
		}

		const auto* materialDescriptor = Core::getResource<Gfx::MaterialDescriptor>(registry, mMaterialResource);
		if (!materialDescriptor) {
			Core::logEntry(registry, "Missing Material Descriptor.");
			return;
		}

		std::string exportedJSON{ Core::save(registry, Core::Any{ *materialDescriptor }) };

		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace DevTools
