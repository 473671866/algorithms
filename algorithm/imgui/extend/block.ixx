module;
#include <imgui.h>
#include <iostream>

export module component.control.block;
export import component.control;
namespace imgui = ImGui;

export namespace view::ctrl {
	class block : public control {
	private:
		ImVec4 _color;

	public:
		block(std::string_view text, ImVec4 color, int id = 0) :_color(color), control(text, id, { 0, 0 }) { }

		bool domain() {
			imgui::ColorButton(_text.data(), _color);
			return true;
		}
	};
}