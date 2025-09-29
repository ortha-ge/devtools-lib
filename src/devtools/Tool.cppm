module;

#include <functional>
#include <string>

export module DevTools.Tool;

import entt;

export namespace DevTools {

	struct Tool {

		using ToolOpenFunction = std::function<void(entt::registry&, Tool&)>;
		using ToolUpdateFunction = std::function<void(entt::registry&, Tool&)>;
		using ToolCloseFunction = std::function<void(entt::registry&, Tool&)>;

		std::string toolName{ "Default Name" };
		ToolOpenFunction openFunction;
		ToolUpdateFunction updateFunction;
		ToolCloseFunction closeFunction;
		bool isOpen{ false };
		bool isMinimized{ false };
		bool isOpenFunctionCalled { false };
	};

} // namespace DevTools
