export module Ortha.DevTools.LogViewerTool;

import entt;

export namespace Ortha::DevTools {

	class LogViewerTool {
	public:
		LogViewerTool(entt::registry& registry);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);
	};

} // namespace Ortha::DevTools
