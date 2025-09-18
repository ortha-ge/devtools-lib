
export module DevTools.SystemsService;

export import DevTools.Systems;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	void initDevTools(DevToolsSystems&, kgr::container&);

	struct DevToolsSystemsService
		: kgr::single_service<DevToolsSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>
		, kgr::autocall<kgr::invoke_method<initDevTools>>{};

} // namespace DevTools
