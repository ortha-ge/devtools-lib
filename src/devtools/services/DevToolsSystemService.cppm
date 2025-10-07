
export module Ortha.DevTools.SystemsService;

export import Ortha.DevTools.Systems;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	void initDevTools(DevToolsSystems&, kgr::container&);

	struct DevToolsSystemsService
		: kgr::single_service<DevToolsSystems, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>
		, kgr::autocall<kgr::invoke_method<initDevTools>>{};

} // namespace Ortha::DevTools
