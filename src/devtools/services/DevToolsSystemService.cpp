module Ortha.DevTools.SystemsService;

import Ortha.DevTools.EntityToolService;
import Ortha.DevTools.LogViewerToolService;
import Ortha.DevTools.MaterialEditorToolService;
import Ortha.DevTools.NodeToolService;
import Ortha.DevTools.ResourceCacheToolService;
import Ortha.DevTools.SceneViewerToolService;
import Ortha.DevTools.SpriteEditorToolService;

namespace Ortha::DevTools {

	void initDevTools(DevToolsSystems&, kgr::container& container) {
		container.emplace<EntityToolService>();
		container.emplace<LogViewerToolService>();
		container.emplace<NodeToolService>();
		container.emplace<MaterialEditorToolService>();
		container.emplace<ResourceCacheToolService>();
		container.emplace<SceneViewerToolService>();
		container.emplace<SpriteEditorToolService>();
	}

} // namespace Ortha::DevTools
