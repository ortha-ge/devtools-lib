module;

#include <entt/entity/entity.hpp>
#include <entt/fwd.hpp>

export module DevTools.LogViewerTool;

export namespace DevTools {

	class LogViewerTool {
	public:
		void setup(entt::registry& registry);
	};

} // namespace DevTools
