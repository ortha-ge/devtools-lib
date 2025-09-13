module;

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <rpp/rpp.hpp>

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

namespace DevTools::MaterialEditorToolInternal {
	constexpr const char* PrintExportFormatString = R"(Material JSON
=============
{})";
}

namespace DevTools {

	MaterialEditorTool::MaterialEditorTool()
		: mMaterialDescriptor(Gfx::MaterialDescriptor{}){

	}

	void MaterialEditorTool::registerSubscriptions(entt::registry& registry) {
		mSubscriptions.add(mMaterialDescriptor.get_observable()
			| rpp::operators::distinct_until_changed()
			| rpp::operators::debounce(std::chrono::milliseconds(500), rpp::schedulers::new_thread())
			| rpp::operators::observe_on(mRunLoop)
			| rpp::operators::subscribe_with_disposable([this, &registry](auto&& materialDescriptor) {
				setMaterialDescriptor(registry, materialDescriptor);
			})
		);
	}

	void MaterialEditorTool::releaseSubscriptions() {
		mSubscriptions.clear();
	}

	void MaterialEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Material Editor",
			[this](entt::registry& registry, Tool&) { open(registry); },
			[this](entt::registry& registry, Tool&) { update(registry); },
			[this](entt::registry& registry, Tool&) { destroy(registry); }
		);
	}

	void MaterialEditorTool::open(entt::registry& registry) {
		Gfx::Sprite spriteResource {};
		spriteResource.descriptor.frames = {
			{ { 0.0f, 0.0f }, { 100.0f, 100.0f } }
		};

		mMaterialRenderObjectRoot = registry.create();
		registry.emplace<Core::Spatial>(mMaterialRenderObjectRoot, glm::vec3{ 50.0f, 50.0f, 50.0f });
		registry.emplace<Gfx::SpriteObject>(mMaterialRenderObjectRoot, spriteResource);

		registerSubscriptions(registry);
	}

	void MaterialEditorTool::update(entt::registry& registry) {
		if (!mRunLoop.is_empty()) {
			mRunLoop.dispatch();
		}

		auto materialDescriptor{ mMaterialDescriptor.get_value() };

		ImGui::InputFloat3("Material Render Pos", &mMaterialRenderPos[0]);
		ImGui::InputText("Shader Program File-Path", &materialDescriptor.shaderProgramFilePath);
		ImGui::InputText("Texture Image File-Path", &materialDescriptor.textureImageFilePath);

		auto alphaColour{ materialDescriptor.alphaColour.value_or(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }) };
		if (ImGui::ColorPicker4("Transparency Colour", &alphaColour[0])) {
			materialDescriptor.alphaColour = alphaColour;
		}

		if (ImGui::Button("Clear Transparency Colour")) {
			materialDescriptor.alphaColour = std::nullopt;
		}

		if (ImGui::Button("Export JSON", ImVec2(100, 15))) {
			_printExportedMaterialJSON(registry, materialDescriptor);
		}

		mMaterialDescriptor.get_observer().on_next(std::move(materialDescriptor));

		// Update the position relative to the camera
		auto cameraView = registry.view<Gfx::Camera>();
		if (cameraView.empty()) {
			return;
		}

		const auto& cameraEntity = cameraView.front();
		if (!registry.all_of<Core::Spatial>(cameraEntity)) {
			return;
		}

		const auto& cameraSpatial = registry.get<Core::Spatial>(cameraEntity);
		auto& renderObjectSpatial{ registry.get<Core::Spatial>(mMaterialRenderObjectRoot) };
		renderObjectSpatial.position = cameraSpatial.position + mMaterialRenderPos;
	}

	void MaterialEditorTool::destroy(entt::registry& registry) {
		releaseSubscriptions();

		if (mMaterialRenderObjectRoot != entt::null) {
			registry.destroy(mMaterialRenderObjectRoot);
			mMaterialRenderObjectRoot = entt::null;
		}
	}

	void MaterialEditorTool::setMaterialDescriptor(entt::registry& registry, const Gfx::MaterialDescriptor& materialDescriptor) {
		registry.get_or_emplace<Gfx::RenderObject>(mMaterialRenderObjectRoot).materialResource = materialDescriptor;
		registry.remove<Gfx::Material>(mMaterialRenderObjectRoot);
		registry.remove<Gfx::MaterialDescriptor>(mMaterialRenderObjectRoot);
	}

	void MaterialEditorTool::_printExportedMaterialJSON(entt::registry& registry, const Gfx::MaterialDescriptor& materialDescriptor) {
		using namespace MaterialEditorToolInternal;
		std::string exportedJSON{ Core::save(registry, Core::Any{ materialDescriptor }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace DevTools
