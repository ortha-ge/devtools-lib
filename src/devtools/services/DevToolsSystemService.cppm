
export module DevTools.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
export import DevTools.Systems;

export namespace DevTools {

    class DevToolsSystemsService : public kgr::single_service<DevToolsSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // DevTools
