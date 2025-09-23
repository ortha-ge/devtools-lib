
export module DevTools.EntityToolService;

export import DevTools.EntityTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct EntityToolService
		: kgr::single_service<EntityTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
