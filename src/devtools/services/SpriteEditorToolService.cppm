
export module DevTools.SpriteEditorToolService;

export import DevTools.SpriteEditorTool;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;
import Gfx.SystemsService;

export namespace DevTools {

	struct SpriteEditorToolService
		: kgr::single_service<SpriteEditorTool, kgr::dependency<Core::EnTTRegistryService>>{};

} // namespace DevTools
