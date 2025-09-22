module;

#include <entt/fwd.hpp>

export module DevTools.NodeTool;

import Core.EnTTRegistry;

export namespace DevTools {

	class NodeTool {
	public:

		NodeTool(Core::EnTTRegistry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

	};

} // namespace DevTools
