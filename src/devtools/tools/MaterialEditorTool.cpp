module;

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
import Gfx.Material;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.MaterialDescriptor;

namespace DevTools {

	constexpr const char* PrintExportFormatString = R"(Material JSON
=============
%s)";

	bool MaterialEditorTool::_hasValidMaterialResource(entt::registry& registry) const {
		if (mMaterialResourceEntity == entt::null) {
			return false;
		}

		if (!registry.all_of<Core::ResourceHandle>(mMaterialResourceEntity)) {
			return false;
		}

		const auto& resourceHandle = registry.get<Core::ResourceHandle>(mMaterialResourceEntity);
		const entt::entity resourceEntity{ resourceHandle.getResourceEntity() };
		if (resourceEntity == entt::null) {
			return false;
		}

		if (!registry.all_of<Gfx::Material>(resourceEntity)) {
			return false;
		}

		return true;
	}

	void MaterialEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Material Editor", [this](Tool& tool, entt::registry& registry) {
			if (ImGui::Begin(tool.toolName.c_str(), &tool.isOpen)) {
				const bool hasValidMaterial = _hasValidMaterialResource(registry);

				if (mMaterialResourceEntity != entt::null && (!tool.isOpen || !hasValidMaterial)) {
					registry.destroy(mMaterialResourceEntity);
					mMaterialResourceEntity = entt::null;
				}

				if (mMaterialRenderObjectEntity != entt::null && (!tool.isOpen || !hasValidMaterial)) {
					registry.destroy(mMaterialRenderObjectEntity);
					mMaterialRenderObjectEntity = entt::null;
				}

				if (tool.isOpen && !hasValidMaterial) {
					mMaterialResourceEntity = registry.create();
					registry.emplace<Core::ResourceLoadRequest>(
						mMaterialResourceEntity,
						Core::ResourceLoadRequest::create<Core::TypeLoader>(
							"assets/materials/cat.json",
							std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>()));

					mMaterialRenderObjectEntity = registry.create();
					registry.emplace<Core::Spatial>(mMaterialRenderObjectEntity, 500.0f, 500.0f, 10.0f, 20.0f, 20.0f);
					registry.emplace<Gfx::Sprite>(mMaterialRenderObjectEntity);
					registry.emplace<Gfx::RenderObject>(mMaterialRenderObjectEntity, mMaterialResourceEntity);
				}

				if (!registry.all_of<Core::ResourceHandle>(mMaterialResourceEntity)) {
					ImGui::End();
					return;
				}

				auto&& [materialEntity, materialDescriptor] = Core::getResourceAndEntity<Gfx::MaterialDescriptor>(registry, mMaterialResourceEntity);
				if (!materialDescriptor) {
					ImGui::End();
					return;
				}

				auto& renderObject{ registry.get<Gfx::RenderObject>(mMaterialRenderObjectEntity) };

				bool changed{ false };
				if (ImGui::Button("Next Frame", ImVec2(150, 50))) {
					renderObject.currentSpriteFrame++;
					if (renderObject.currentSpriteFrame >= materialDescriptor->spriteFrames.size()) {
						renderObject.currentSpriteFrame = 0;
					}
				}

				if (ImGui::InputText("Shader Program File-Path", &materialDescriptor->shaderProgramFilePath)) {
					changed = true;
				}

				if (ImGui::InputText("Texture Image File-Path", &materialDescriptor->textureImageFilePath)) {
					changed = true;
				}

				const auto& alphaColour{ materialDescriptor->alphaColour.value_or(Gfx::Colour{ 1.0f, 1.0f, 1.0f, 1.0f }) };
				float transparencyColour[3]{ alphaColour.r, alphaColour.g, alphaColour.b };
				if (ImGui::ColorPicker3("Transparency Colour", transparencyColour)) {
					materialDescriptor->alphaColour = { transparencyColour[0], transparencyColour[1],
													   transparencyColour[2] };
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
		if (mMaterialResourceEntity == entt::null) {
			Core::logEntry(registry, "No Material Resource.");
			return;
		}

		if (!registry.all_of<Gfx::MaterialDescriptor>(mMaterialResourceEntity)) {
			Core::logEntry(registry, "Missing Material Descriptor.");
			return;
		}

		auto& materialDescriptor{ registry.get<Gfx::MaterialDescriptor>(mMaterialResourceEntity) };
		std::string exportedJSON{ Core::save(registry, Core::Any{ materialDescriptor }) };

		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace DevTools
