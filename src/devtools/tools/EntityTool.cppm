export module Ortha.DevTools.EntityTool;

import entt;

export namespace Ortha::DevTools {

	class EntityTool {
	public:

		EntityTool(entt::registry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

	};

} // namespace Ortha::DevTools
