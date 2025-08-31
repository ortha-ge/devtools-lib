module;

#include <functional>
#include <string>

#include <entt/fwd.hpp>

export module DevTools.Tool;

export namespace DevTools {

	struct Tool {

		using ToolUpdateFunction = std::function<void(Tool &, entt::registry &)>;

		std::string toolName{"Default Name"};
		ToolUpdateFunction updateFunction;
		bool isOpen{false};
	};

} // DevTools
