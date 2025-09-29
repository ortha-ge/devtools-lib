export module DevTools.NodeTool;

import entt;

export namespace DevTools {

	class NodeTool {
	public:

		NodeTool(entt::registry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

	};

} // namespace DevTools
