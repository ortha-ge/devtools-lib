module;

#include <functional>
#include <string>

#include <entt/fwd.hpp>

export module DevTools.Tool;

export namespace DevTools {

	struct Tool {

		using ToolUpdateFunction = std::function<void(entt::registry&, Tool&)>;
		using ToolCloseFunction = std::function<void(entt::registry&, Tool&)>;

		std::string toolName{ "Default Name" };
		ToolUpdateFunction updateFunction;
		ToolCloseFunction closeFunction;
		bool isOpen{ false };
	};

} // namespace DevTools
