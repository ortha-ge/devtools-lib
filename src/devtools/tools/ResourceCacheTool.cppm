export module DevTools.ResourceCacheTool;

import entt;

export namespace DevTools {

	class ResourceCacheTool {
	public:

		ResourceCacheTool(entt::registry&);

		void setupTool(entt::registry&);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();
	};
} // namespace DevTools