
export module Ortha.DevTools.MaterialEditorToolService;

export import Ortha.DevTools.MaterialEditorTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct MaterialEditorToolService
		: kgr::single_service<MaterialEditorTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
