module;
#include <iostream>
#include <functional>
#include <imgui.h>

export module component.control.combo;
export import component.control;
namespace imgui = ImGui;

export namespace view::ctrl {
	class combo :public control {
	private:
		std::function<void(int)> _changed;
		std::string_view _label;
		int _selected;

	public:
		combo(std::string_view label, std::string_view text) :_label(label), control(text, 0, { 0, 0 }) {}

		bool changed(std::function<void(int)> change) {
			_changed = change;
			return true;
		}

		bool domain() {
			imgui::SetNextItemWidth(97);
			if (imgui::Combo(_label.data(), &_selected, _text.data())) {
				if (_changed) {
					_changed(_selected);
				}
			}

			return true;
		}
	};
}