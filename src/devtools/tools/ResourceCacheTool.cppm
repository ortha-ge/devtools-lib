export module Ortha.DevTools.ResourceCacheTool;

import entt;

export namespace Ortha::DevTools {

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
} // namespace Ortha::DevTools