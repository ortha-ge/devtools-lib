
export module DevTools.LogViewerToolService;

export import DevTools.LogViewerTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct LogViewerToolService
		: kgr::single_service<LogViewerTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
