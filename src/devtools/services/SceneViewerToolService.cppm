
export module Ortha.DevTools.SceneViewerToolService;

export import Ortha.DevTools.SceneViewerTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct SceneViewerToolService
		: kgr::single_service<SceneViewerTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
