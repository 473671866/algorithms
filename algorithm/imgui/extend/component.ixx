module;
#include <concepts>

export module component;

export
template<class parameters>
concept callable = std::invocable<parameters>;

export namespace view {
	class component {
	public:
		virtual bool domain() noexcept(false) = 0;
	};
}