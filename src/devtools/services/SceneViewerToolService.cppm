
export module DevTools.SceneViewerToolService;

export import DevTools.SceneViewerTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct SceneViewerToolService
		: kgr::single_service<SceneViewerTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
