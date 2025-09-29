export module DevTools.Systems;

import Core.ResourceHandle;
import Core.Scheduler;
import Gfx.Systems;
import entt;

export namespace DevTools {

	class DevToolsSystems {
	public:
		DevToolsSystems(entt::registry&, Core::Scheduler&);
		~DevToolsSystems();

		DevToolsSystems(DevToolsSystems&&) = delete;
		DevToolsSystems& operator=(DevToolsSystems&&) noexcept = delete;

		DevToolsSystems(const DevToolsSystems&) = delete;
		DevToolsSystems& operator=(const DevToolsSystems&) = delete;

	private:

		static entt::entity createFontTexture(entt::registry&);
		void drawDevToolsImGui(entt::registry&);
		void _renderImGui(entt::registry&);

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle{};
		entt::entity mFontImage{ entt::null };

		Core::ResourceHandlePtr mShaderProgram;
		Core::ResourceHandlePtr mImageShaderProgram;

		entt::entity mViewportEntity{ entt::null };

		bool mIsEnabled{ false };

	};

} // namespace DevTools
