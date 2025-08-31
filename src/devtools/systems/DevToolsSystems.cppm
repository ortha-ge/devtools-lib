module;

#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>

export module DevTools.Systems;

import Core.EnTTRegistry;
import Core.Scheduler;
import DevTools.MaterialResourceEditTool;
import Gfx.Systems;

export namespace DevTools {

    class DevToolsSystems {
    public:

        DevToolsSystems(Core::EnTTRegistry&, Core::Scheduler&, Gfx::GfxSystems&);
        ~DevToolsSystems();

        DevToolsSystems(DevToolsSystems&&) = delete;
        DevToolsSystems& operator=(DevToolsSystems&&) noexcept = delete;

        DevToolsSystems(const DevToolsSystems&) = delete;
        DevToolsSystems& operator=(const DevToolsSystems&) = delete;

    private:

        void _renderImGui(entt::registry&);
        void _setupTools(entt::registry&);

        Core::EnTTRegistry& mRegistry;
        Core::Scheduler& mScheduler;
        Core::TaskHandle mTickHandle{};
        entt::entity mRenderCallbackEntity{ entt::null };
        MaterialResourceEditTool mMaterialResourceEditTool;

    };

} // DevTools
