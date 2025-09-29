module DevTools.SystemsService;

import DevTools.EntityToolService;
import DevTools.LogViewerToolService;
import DevTools.MaterialEditorToolService;
import DevTools.NodeToolService;
import DevTools.ResourceCacheToolService;
import DevTools.SceneViewerToolService;
import DevTools.SpriteEditorToolService;

namespace DevTools {

	void initDevTools(DevToolsSystems&, kgr::container& container) {
		container.emplace<EntityToolService>();
		container.emplace<LogViewerToolService>();
		container.emplace<NodeToolService>();
		container.emplace<MaterialEditorToolService>();
		container.emplace<ResourceCacheToolService>();
		container.emplace<SceneViewerToolService>();
		container.emplace<SpriteEditorToolService>();
	}

} // namespace DevTools
