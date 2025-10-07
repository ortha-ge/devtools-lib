
export module Ortha.DevTools.EntityToolService;

export import Ortha.DevTools.EntityTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct EntityToolService
		: kgr::single_service<EntityTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
