module;

#include <entt/entt.hpp>

module DevTools.SystemsService;

import DevTools.LogViewerToolService;
import DevTools.MaterialEditorToolService;
import DevTools.ResourceCacheToolService;
import DevTools.SpriteEditorToolService;

namespace DevTools {

	void initDevTools(DevToolsSystems&, kgr::container& container) {
		container.emplace<LogViewerToolService>();
		container.emplace<MaterialEditorToolService>();
		container.emplace<ResourceCacheToolService>();
		container.emplace<SpriteEditorToolService>();
	}

} // namespace DevTools
