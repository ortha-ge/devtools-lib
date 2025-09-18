
export module DevTools.ResourceCacheToolService;

export import DevTools.ResourceCacheTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct ResourceCacheToolService
		: kgr::single_service<ResourceCacheTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
