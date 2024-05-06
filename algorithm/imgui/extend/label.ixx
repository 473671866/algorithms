module;
#include <imgui.h>
#include <iostream>

export module component.control.label;
export import component.control;
export namespace imgui = ImGui;

export namespace view::ctrl {
	class label : public control {
	private:
		std::string_view _text;

	public:
		label(std::string_view text) : _text(text) {}

		bool domain() {
			imgui::Text(_text.data());
			return true;
		}
	};
}