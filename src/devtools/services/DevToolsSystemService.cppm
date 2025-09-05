
export module DevTools.SystemsService;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;
export import DevTools.Systems;

export namespace DevTools {

	class DevToolsSystemsService
		: public kgr::single_service<DevToolsSystems,
			  kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService, Gfx::GfxSystemsService>> {};

} // namespace DevTools
