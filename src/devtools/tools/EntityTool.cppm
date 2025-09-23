module;

#include <entt/fwd.hpp>

export module DevTools.EntityTool;

import Core.EnTTRegistry;

export namespace DevTools {

	class EntityTool {
	public:

		EntityTool(Core::EnTTRegistry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

	};

} // namespace DevTools
