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
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import Core.TypeSaver;
import DevTools.Tool;
import Gfx.Colour;
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
					mMaterialResource = Core::ResourceHandle::create<Core::TypeLoader>(
							"assets/materials/cat.json",
							std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());

					auto loadRequest = registry.create();
					registry.emplace<Core::ResourceLoadRequest>(loadRequest, mMaterialResource);

					auto spriteResource = Core::ResourceHandle::create<Core::TypeLoader>(
												"assets/sprites/cat.json",
												std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::SpriteDescriptor>>());

					loadRequest = registry.create();
					registry.emplace<Core::ResourceLoadRequest>(loadRequest, spriteResource);

					mMaterialRenderObjectEntity = registry.create();
					registry.emplace<Core::Spatial>(mMaterialRenderObjectEntity, glm::vec3{ 500.0f, 500.0f, 10.0f }, glm::vec3{ 20.0f, 20.0f, 1.0f });
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

				auto& renderObject{ registry.get<Gfx::RenderObject>(mMaterialRenderObjectEntity) };

				bool changed{ false };
				// if (ImGui::Button("Next Frame", ImVec2(150, 50))) {
				// 	renderObject.currentSpriteFrame++;
				// 	if (renderObject.currentSpriteFrame >= materialDescriptor->spriteFrames.size()) {
				// 		renderObject.currentSpriteFrame = 0;
				// 	}
				// }

				if (ImGui::InputText("Shader Program File-Path", &materialDescriptor->shaderProgramFilePath)) {
					changed = true;
				}

				if (ImGui::InputText("Texture Image File-Path", &materialDescriptor->textureImageFilePath)) {
					changed = true;
				}

				const auto& alphaColour{ materialDescriptor->alphaColour.value_or(Gfx::Colour{ 1.0f, 1.0f, 1.0f, 1.0f }) };
				float transparencyColour[4]{ alphaColour.r, alphaColour.g, alphaColour.b, alphaColour.a };
				if (ImGui::ColorPicker4("Transparency Colour", transparencyColour)) {
					materialDescriptor->alphaColour = { transparencyColour[0], transparencyColour[1],
													   transparencyColour[2], transparencyColour[3] };
					changed = true;
				}

				if (ImGui::Button("Clear Transparency Colour")) {
					materialDescriptor->alphaColour = std::nullopt;
					changed = true;
				}

				float dimensions[2]{ materialDescriptor->width, materialDescriptor->height };
				if (ImGui::InputFloat2("Dimensions", dimensions)) {
					materialDescriptor->width = dimensions[0];
					materialDescriptor->height = dimensions[1];
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
