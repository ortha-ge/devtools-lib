
export module Ortha.DevTools.LogViewerToolService;

export import Ortha.DevTools.LogViewerTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct LogViewerToolService
		: kgr::single_service<LogViewerTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
