module;

#include <imgui.h>
#include <imgui_stdlib.h>
#include <rpp/rpp.hpp>

module DevTools.MaterialEditorTool;

import Ortha.RTTI.Any;
import Core.EnTTNode;
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
import Gfx.RenderTexture;
import Gfx.Sprite;
import Gfx.SpriteObject;
import Gfx.Material;
import Gfx.MaterialDescriptor;
import Gfx.Reflection.MaterialDescriptor;
import Gfx.Reflection.Sprite;
import Gfx.Viewport;
import entt;
import glm;

namespace DevTools::MaterialEditorToolInternal {
	constexpr const char* PrintExportFormatString = R"(Material JSON
=============
{})";
}

namespace DevTools {

	MaterialEditorTool::MaterialEditorTool(entt::registry& registry)
		: mMaterialDescriptor(Gfx::MaterialDescriptor{
			.shaderProgramFilePath = "assets/shaders/sprite_shader_program.json",
			.textureImageFilePath = "assets/textures/frog.png"
		})
		, mMaterialRenderPos(250.0f, 200.0f)
		, mMaterialRenderScale(1.0f, 1.0f) {

		setupTool(registry);
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

	void MaterialEditorTool::setupTool(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Material Editor",
			[this](entt::registry& registry, Tool&) { open(registry); },
			[this](entt::registry& registry, Tool&) { update(registry); },
			[this](entt::registry& registry, Tool&) { destroy(registry); }
		);
	}

	void MaterialEditorTool::open(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		const entt::entity sceneRoot = createEnTTNode(registry, "MaterialToolRoot");

		// Render Texture
		mRenderTexture = registry.create();

		RenderTexture renderTexture;
		renderTexture.width = 512;
		renderTexture.height = 512;
		registry.emplace<RenderTexture>(mRenderTexture, renderTexture);

		// Camera + Viewport
		const entt::entity cameraEntity = registry.create();
		registry.emplace<Spatial>(cameraEntity);

		Viewport viewport;
		viewport.renderTarget = mRenderTexture;
		viewport.offset = { 0.0f, 0.0f };
		viewport.dimensions = { 1.0f, 1.0f };
		registry.emplace<Viewport>(cameraEntity, viewport);

		Camera camera;
		camera.viewport = cameraEntity;
		camera.sceneRoot = sceneRoot;
		registry.emplace<Camera>(cameraEntity, camera);

		// Material SpriteObject
		Sprite spriteResource {};
		spriteResource.descriptor.frames = {};

		mMaterialRenderObjectRoot = createChildEnTTNode(registry, sceneRoot, "MaterialPreview");
		registry.emplace<Spatial>(mMaterialRenderObjectRoot, glm::vec3{ 500.0f, 500.0f, 0.0f }, glm::vec3{ 4.0f, 4.0f, 1.0f });
		registry.emplace<SpriteObject>(mMaterialRenderObjectRoot, spriteResource);

		registerSubscriptions(registry);
	}

	void MaterialEditorTool::update(entt::registry& registry) {
		if (!mRunLoop.is_empty()) {
			mRunLoop.dispatch();
		}

		auto materialDescriptor{ mMaterialDescriptor.get_value() };

		ImGui::Image(static_cast<ImTextureID>(mRenderTexture), ImVec2{ 512, 512});

		ImGui::DragFloat2("Material Render Pos", &mMaterialRenderPos[0], 0.25f);
		ImGui::DragFloat2("Material Render Scale", &mMaterialRenderScale[0], 0.1f);
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

		auto& renderObjectSpatial{ registry.get<Core::Spatial>(mMaterialRenderObjectRoot) };
		renderObjectSpatial.position = glm::vec3{ mMaterialRenderPos, 0.0f };
		renderObjectSpatial.scale = glm::vec3{ mMaterialRenderScale, 1.0f };
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
		std::string exportedJSON{ Core::save(registry, Ortha::RTTI::Any{ materialDescriptor }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace DevTools
