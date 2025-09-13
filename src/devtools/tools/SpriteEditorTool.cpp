module;

#include <chrono>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <rxcpp/rx.hpp>

module DevTools.SpriteEditorTool;

import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.Spatial;
import Core.TypeLoader;
import DevTools.Tool;
import Gfx.Camera;
import Gfx.MaterialDescriptor;
import Gfx.RenderObject;
import Gfx.SpriteObject;
import Gfx.Reflection.MaterialDescriptor;

namespace DevTools {

	SpriteEditorTool::SpriteEditorTool()
		: mMaterialResourceFilePath("")
		, mSprite({}) {


	}

	void SpriteEditorTool::registerSubscriptions(entt::registry& registry) {
		mSubscriptions.add(observeMaterialResourceFilePath()
			.observe_on(rxcpp::observe_on_run_loop(mRunLoop))
			.subscribe([this, &registry](auto&& materialFilePath) {
				loadMaterialResource(registry, materialFilePath);
			}));

		mSubscriptions.add(observeSprite()
			.observe_on(rxcpp::observe_on_run_loop(mRunLoop))
			.subscribe([this, &registry](auto&& spriteFilePath) {
				setSprite(registry, spriteFilePath);
			}));
	}

	void SpriteEditorTool::releaseSubscriptions() {
		mSubscriptions = {};
	}

	void SpriteEditorTool::setup(entt::registry& registry) {
		const entt::entity testTool{ registry.create() };
		registry.emplace<Tool>(testTool, "Sprite Editor",
			[this](entt::registry& registry, Tool&) { open(registry); },
			[this](entt::registry& registry, Tool&) { update(registry); },
			[this](entt::registry& registry, Tool&) { destroy(registry); }
		);
	}

	void SpriteEditorTool::open(entt::registry& registry) {
		if (mSpriteObjectRoot == entt::null) {
			mSpriteObjectRoot = registry.create();
			registry.emplace<Core::Spatial>(mSpriteObjectRoot, glm::vec3{}, glm::vec3(5.0f, 5.0f, 1.0f));
			registry.emplace<Gfx::RenderObject>(mSpriteObjectRoot);
			registry.emplace<Gfx::SpriteObject>(mSpriteObjectRoot, mSprite.get_value());
		}

		registerSubscriptions(registry);
	}

	void SpriteEditorTool::update(entt::registry& registry) {
		mRunLoop.dispatch();

		std::string materialResourceFilePath = mMaterialResourceFilePath.get_value();
		if (ImGui::InputText("Material Resource File Path", &materialResourceFilePath)) {
			mMaterialResourceFilePath.get_subscriber().on_next(materialResourceFilePath);
		}

		bool changed { false };
		Gfx::Sprite sprite = mSprite.get_value();
		if (ImGui::CollapsingHeader("Frames")) {
			for (size_t i = 0; i < sprite.frames.size(); ++i) {
				if (ImGui::TreeNode(std::format("Frame {}", i).c_str())) {
					auto& frame{ sprite.frames[i] };
					changed =
						ImGui::InputFloat2("Bottom Left", &frame.bottomLeft[0]) ||
						ImGui::InputFloat2("Top Right", &frame.topRight[0]);
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::Button("Add Frame")) {
			sprite.frames.emplace_back(glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 100.0f, 100.0f });
			changed = true;
		}

		if (changed) {
			mSprite.get_subscriber().on_next(sprite);
		}

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

	rxcpp::observable<std::string> SpriteEditorTool::observeMaterialResourceFilePath() const {
		return mMaterialResourceFilePath.get_observable()
			.observe_on(rxcpp::observe_on_event_loop())
			.debounce(std::chrono::milliseconds(500));
	}

	rxcpp::observable<Gfx::Sprite> SpriteEditorTool::observeSprite() const {
		return mSprite.get_observable()
			.observe_on(rxcpp::observe_on_event_loop())
			.debounce(std::chrono::milliseconds(500));
	}

	void SpriteEditorTool::loadMaterialResource(entt::registry& registry, std::string materialResourceFilePath) const {
		auto& renderObject{ registry.get<Gfx::RenderObject>(mSpriteObjectRoot) };
		renderObject.materialResource = Core::ResourceLoadRequest::create<Core::TypeLoader>(
			registry, std::move(materialResourceFilePath),
			std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>());
	}

	void SpriteEditorTool::setSprite(entt::registry& registry, Gfx::Sprite sprite) const {
		registry.get<Gfx::SpriteObject>(mSpriteObjectRoot).spriteResource = std::move(sprite);
	}

} // namespace DevTools
