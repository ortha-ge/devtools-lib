
export module DevTools.NodeToolService;

export import DevTools.NodeTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct NodeToolService
		: kgr::single_service<NodeTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
