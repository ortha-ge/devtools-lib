module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module DevTools.Systems;

import Core.EnTTRegistry;
import Core.ResourceHandle;
import Core.Scheduler;
import Gfx.Systems;

export namespace DevTools {

	class DevToolsSystems {
	public:
		DevToolsSystems(Core::EnTTRegistry&, Core::Scheduler&);
		~DevToolsSystems();

		DevToolsSystems(DevToolsSystems&&) = delete;
		DevToolsSystems& operator=(DevToolsSystems&&) noexcept = delete;

		DevToolsSystems(const DevToolsSystems&) = delete;
		DevToolsSystems& operator=(const DevToolsSystems&) = delete;

	private:

		static entt::entity createFontTexture(entt::registry&);
		void drawDevToolsImGui(entt::registry&);
		void _renderImGui(entt::registry&);

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};
		entt::entity mFontImage{ entt::null };

		Core::ResourceHandlePtr mShaderProgram;
		Core::ResourceHandlePtr mImageShaderProgram;

		entt::entity mViewportEntity{ entt::null };

		bool mIsEnabled{ false };

	};

} // namespace DevTools
