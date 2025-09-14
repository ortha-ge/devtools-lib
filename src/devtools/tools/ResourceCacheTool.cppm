module;

#include <entt/fwd.hpp>

export module DevTools.ResourceCacheTool;

export namespace DevTools {

	class ResourceCacheTool {
	public:

		ResourceCacheTool();

		void setup(entt::registry&);

		void open(entt::registry&);
		void update(entt::registry&);
		void destroy(entt::registry&);

	private:

		void registerSubscriptions(entt::registry&);
		void releaseSubscriptions();
	};
} // namespace DevTools