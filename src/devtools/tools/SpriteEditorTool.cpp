module;

#include <chrono>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <rpp/rpp.hpp>

module Ortha.DevTools.SpriteEditorTool;

import Ortha.RTTI.Any;
import Ortha.Core.EnTTNode;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.JsonTypeSaverAdapter;
import Ortha.Core.Log;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.Spatial;
import Ortha.Core.TypeLoader;
import Ortha.Core.TypeSaver;
import Ortha.DevTools.Tool;
import Ortha.Gfx.Camera;
import Ortha.Gfx.MaterialDescriptor;
import Ortha.Gfx.RenderObject;
import Ortha.Gfx.RenderTexture;
import Ortha.Gfx.Sprite;
import Ortha.Gfx.SpriteObject;
import Ortha.Gfx.Reflection.MaterialDescriptor;
import Ortha.Gfx.Reflection.Sprite;
import Ortha.Gfx.Viewport;
import entt;
import glm;

namespace Ortha::DevTools::SpriteEditorToolInternal {
	constexpr const char* PrintExportFormatString = R"(Sprite JSON
=============
{})";

}

namespace Ortha::DevTools {

	SpriteEditorTool::SpriteEditorTool(entt::registry& registry)
		: mMaterialResourceFilePath("")
		, mSprite(Gfx::SpriteDescriptor{}) {

		setupTool(registry);
	}

	void SpriteEditorTool::registerSubscriptions(entt::registry& registry) {
		mSubscriptions.add(mMaterialResourceFilePath.get_observable()
			| rpp::operators::debounce(std::chrono::milliseconds(500), rpp::schedulers::new_thread())
			| rpp::operators::observe_on(mRunLoop)
			| rpp::operators::subscribe_with_disposable([this, &registry](auto&& materialFilePath) {
				loadMaterialResource(registry, materialFilePath);
			})
		);

		mSubscriptions.add(mSprite.get_observable()
			| rpp::operators::distinct_until_changed()
			| rpp::operators::debounce(std::chrono::milliseconds(500), rpp::schedulers::new_thread())
			| rpp::operators::observe_on(mRunLoop)
			| rpp::operators::subscribe_with_disposable([this, &registry](auto&& sprite) {
				setSprite(registry, sprite);
			})
		);
	}

	void SpriteEditorTool::releaseSubscriptions() {
		mSubscriptions.clear();
	}

	void SpriteEditorTool::setupTool(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Sprite Editor",
			[this](entt::registry& registry, Tool&) { open(registry); },
			[this](entt::registry& registry, Tool&) { update(registry); },
			[this](entt::registry& registry, Tool&) { destroy(registry); }
		);
	}

	void SpriteEditorTool::open(entt::registry& registry) {
		using namespace Ortha::Core;
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

		mSpriteObjectRoot = createChildEnTTNode(registry, sceneRoot, "SpritePreview");
		registry.emplace<Spatial>(mSpriteObjectRoot, glm::vec3{ 500.0f, 500.0f, 0.0f }, glm::vec3{ 4.0f, 4.0f, 1.0f });

		registerSubscriptions(registry);
	}

	void SpriteEditorTool::update(entt::registry& registry) {
		if (!mRunLoop.is_empty()) {
			mRunLoop.dispatch();
		}

		ImGui::Image(static_cast<ImTextureID>(mRenderTexture), ImVec2{ 512, 512});

		std::string materialResourceFilePath = mMaterialResourceFilePath.get_value();
		if (ImGui::InputText("Material Resource File Path", &materialResourceFilePath)) {
			mMaterialResourceFilePath.get_observer().on_next(materialResourceFilePath);
		}

		Gfx::SpriteDescriptor spriteDescriptor = mSprite.get_value();
		if (ImGui::CollapsingHeader("Frames")) {
			for (size_t i = 0; i < spriteDescriptor.frames.size(); ++i) {
				if (ImGui::TreeNode(std::format("Frame {}", i).c_str())) {
					auto& frame{ spriteDescriptor.frames[i] };
					ImGui::InputFloat2("Bottom Left", &frame.bottomLeft[0]);
					ImGui::InputFloat2("Top Right", &frame.topRight[0]);
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::Button("Add Frame")) {
			spriteDescriptor.frames.emplace_back(glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 100.0f, 100.0f });
		}

		if (ImGui::Button("Export Sprite JSON")) {
			_printSavedSpriteJSON(registry, spriteDescriptor);
		}

		mSprite.get_observer().on_next(std::move(spriteDescriptor));

		// Sync with the camera position
		auto cameraView = registry.view<Gfx::Camera>();
		if (cameraView.empty()) {
			return;
		}

		const auto& cameraEntity = cameraView.front();
		if (!registry.all_of<Core::Spatial>(cameraEntity)) {
			return;
		}

		const auto& cameraSpatial = registry.get<Core::Spatial>(cameraEntity);

		auto& renderObjectSpatial{ registry.get<Core::Spatial>(mSpriteObjectRoot) };
		renderObjectSpatial.position = cameraSpatial.position + mSpriteRenderPos;
	}

	void SpriteEditorTool::destroy(entt::registry& registry) {
		if (mSpriteObjectRoot != entt::null) {
			registry.destroy(mSpriteObjectRoot);
			mSpriteObjectRoot = entt::null;
		}

		releaseSubscriptions();
	}

	void SpriteEditorTool::loadMaterialResource(entt::registry& registry, std::string materialResourceFilePath) const {
		auto& renderObject{ registry.get_or_emplace<Gfx::RenderObject>(mSpriteObjectRoot) };
		renderObject.materialResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::move(materialResourceFilePath),
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());
	}

	void SpriteEditorTool::setSprite(entt::registry& registry, Gfx::SpriteDescriptor sprite) const {
		registry.get_or_emplace<Gfx::SpriteObject>(mSpriteObjectRoot).spriteResource = Gfx::Sprite{ std::move(sprite) };
	}

	void SpriteEditorTool::_printSavedSpriteJSON(entt::registry& registry, const Gfx::SpriteDescriptor& sprite) {
		using namespace RTTI;
		using namespace SpriteEditorToolInternal;

		std::string exportedJSON{ Core::save(registry, Any{ sprite }) };
		Core::logEntry(registry, PrintExportFormatString, exportedJSON);
	}

} // namespace Ortha::DevTools
