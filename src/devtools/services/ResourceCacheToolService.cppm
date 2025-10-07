
export module Ortha.DevTools.ResourceCacheToolService;

export import Ortha.DevTools.ResourceCacheTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct ResourceCacheToolService
		: kgr::single_service<ResourceCacheTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
