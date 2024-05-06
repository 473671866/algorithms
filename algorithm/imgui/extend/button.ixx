module;
#include <imgui.h>
#include <iostream>
#include <functional>

export module component.control.button;
export import component.control;
namespace imgui = ImGui;

export namespace view::ctrl {
	class button :public control {
	public:
		button(std::string_view text, ImVec2 size = { 0,0 }, int id = 0) : control(text, id, size) {}

		bool domain() override {
			if (_id) {
				imgui::PushID(_id);
			}

			if (imgui::Button(_text.data())) {
				_clicked();
			}

			if (_id) {
				imgui::PopID();
			}

			return true;
		}
	};
}