module;

#include <entt/fwd.hpp>

export module DevTools.LogViewerTool;

import Core.EnTTRegistry;

export namespace DevTools {

	class LogViewerTool {
	public:
		LogViewerTool(Core::EnTTRegistry& registry);

		void setupTool(entt::registry&);
	};

} // namespace DevTools
