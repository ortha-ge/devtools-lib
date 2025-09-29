export module DevTools.LogViewerTool;

import entt;

export namespace DevTools {

	class LogViewerTool {
	public:
		LogViewerTool(entt::registry& registry);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);
	};

} // namespace DevTools
