module;

#include <string>

export module Ortha.DevTools.NodeTool;

import Ortha.Core.Node;
import entt;

export namespace Ortha::DevTools {

	class NodeTool {
	public:

		NodeTool(entt::registry&);

		void setupTool(entt::registry&);

		void onOpen(entt::registry&);
		void onUpdate(entt::registry&);
		void onClose(entt::registry&);

	private:

		void recurseNodeTree(entt::registry&, Core::Node::Ptr, size_t = 0u);

		std::string mLoadNodeJsonString;
		std::string mRenameNodeString;
		bool mIsNodeRenameOpen;

	};

} // namespace Ortha::DevTools
