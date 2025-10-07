
export module Ortha.DevTools.NodeToolService;

export import Ortha.DevTools.NodeTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct NodeToolService
		: kgr::single_service<NodeTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
