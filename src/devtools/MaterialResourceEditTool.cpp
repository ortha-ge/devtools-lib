module;

#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>

module DevTools.MaterialResourceEditTool;

import Core.Any;
import Core.JsonTypeSaverAdapter;
import Core.ResourceHandle;
import Core.Spatial;
import Core.TypeSaver;
import DevTools.Tool;
import Gfx.Colour;
import Gfx.RenderObject;
import Gfx.MaterialDescriptor;
import Gfx.MaterialResource;

namespace DevTools {

	constexpr const char* PrintExportFormatString = R"(Material JSON
=============
%s)";

	void MaterialResourceEditTool::setup(entt::registry &registry) {
		const entt::entity testTool{registry.create()};
		registry.emplace<Tool>(testTool, "Test", [this](Tool &tool, entt::registry &registry) {
			if (!tool.isOpen) {
				return;
			}

			if (ImGui::Begin(tool.toolName.c_str(), &tool.isOpen)) {
				if (tool.isOpen && mMaterialResourceEntity == entt::null) {
					Gfx::MaterialDescriptor materialDescriptor{
						"assets/shaders/sprite_shader_program.json",
						"assets/textures/frog.png",
						{
							{
								15,
								24,
								52,
								54
							},
							{
								79,
								71,
								150,
								105
							}
						},
						Gfx::Colour{
							0.607843137255,
							0.678431372549,
							0.717647058823,
							0.0
						},
						448,
						160
					};

					mMaterialResourceEntity = registry.create();
					registry.emplace<Gfx::MaterialDescriptor>(mMaterialResourceEntity, std::move(materialDescriptor));

					auto resourceHandle{ registry.create() };
					registry.emplace<Core::ResourceHandle>(resourceHandle, mMaterialResourceEntity);

					mMaterialRenderObjectEntity = registry.create();
					registry.emplace<Core::Spatial>(mMaterialRenderObjectEntity, 0.0f, 0.0f, 0.0f);
					registry.emplace<Gfx::RenderObject>(mMaterialRenderObjectEntity, resourceHandle);
				}

				auto& materialDescriptor{registry.get<Gfx::MaterialDescriptor>(mMaterialResourceEntity)};
				auto& renderObject{registry.get<Gfx::RenderObject>(mMaterialRenderObjectEntity)};

				bool changed{ false };
				if (ImGui::Button("Next Frame", ImVec2(150, 50))) {
					renderObject.currentSpriteFrame++;
					if (renderObject.currentSpriteFrame >= materialDescriptor.spriteFrames.size()) {
						renderObject.currentSpriteFrame = 0;
					}
				}

				if (ImGui::InputText("Shader Program File-Path", &materialDescriptor.shaderProgramFilePath)) {
					changed = true;
				}

				if (ImGui::InputText("Texture Image File-Path", &materialDescriptor.textureImageFilePath)) {
					changed = true;
				}

				const auto& alphaColour{ materialDescriptor.alphaColour.value_or({ 1.0f, 1.0f, 1.0f, 1.0f}) };
				float transparencyColour[3]{ alphaColour.r, alphaColour.g, alphaColour.b };
				if (ImGui::ColorPicker3("Transparency Colour", transparencyColour)) {
					materialDescriptor.alphaColour = {
						transparencyColour[0],
						transparencyColour[1],
						transparencyColour[2]
					};
					changed = true;
				}

				if (ImGui::Button("Clear Transparency Colour")) {
					materialDescriptor.alphaColour = std::nullopt;
					changed = true;
				}

				float dimensions[2]{ materialDescriptor.width, materialDescriptor.height };
				if (ImGui::InputFloat2("Dimensions", dimensions)) {
					materialDescriptor.width = dimensions[0];
					materialDescriptor.height = dimensions[1];
					changed = true;
				}

				if (ImGui::Button("Export JSON", ImVec2(100, 15))) {
					_printExportedMaterialJSON(registry);
				}

				if (changed && registry.all_of<Gfx::MaterialResource>(mMaterialResourceEntity)) {
					registry.remove<Gfx::MaterialResource>(mMaterialResourceEntity);
				}
			}

			if (!tool.isOpen && mMaterialResourceEntity != entt::null) {
				registry.destroy(mMaterialResourceEntity);
				registry.destroy(mMaterialRenderObjectEntity);
				mMaterialResourceEntity = entt::null;
				mMaterialRenderObjectEntity = entt::null;
			}



			ImGui::End();
		});
	}

	void MaterialResourceEditTool::_printExportedMaterialJSON(entt::registry& registry) {
		if (mMaterialResourceEntity == entt::null) {
			printf("No Material Resource\n");
			return;
		}

		if (!registry.all_of<Gfx::MaterialDescriptor>(mMaterialResourceEntity)) {
			printf("Missing Material Descriptor\n");
			return;
		}

		auto& materialDescriptor{ registry.get<Gfx::MaterialDescriptor>(mMaterialResourceEntity) };
		std::string exportedJSON{ Core::save(Core::Any{ materialDescriptor}) };

		printf(PrintExportFormatString, exportedJSON.c_str());
	}


} // DevTools
