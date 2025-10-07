
export module Ortha.DevTools.SpriteEditorToolService;

export import Ortha.DevTools.SpriteEditorTool;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;
import Ortha.Gfx.SystemsService;

export namespace Ortha::DevTools {

	struct SpriteEditorToolService
		: kgr::single_service<SpriteEditorTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace Ortha::DevTools
