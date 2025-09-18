
export module DevTools.MaterialEditorToolService;

export import DevTools.MaterialEditorTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct MaterialEditorToolService
		: kgr::single_service<MaterialEditorTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
