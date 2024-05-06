module;
#include <iostream>
#include <functional>

#include <imgui.h>
#include <imgui_internal.h>

#include "entity.inl"

export module component.control.guid;
export import component.control;
namespace imgui = ImGui;

export namespace view::ctrl {
	class guid : public control {
	private:
		terrain(*_terrains)[counter];
		std::function<void(int, int, int)> _clicked;

	public:
		bool clicked(const std::function<void(int, int, int)>& callback) {
			_clicked = callback;
			return true;
		}

		void terrains(terrain(*terrains)[counter]) {
			_terrains = terrains;
		}

		bool domain() {
			for (int i = 1; i <= counter; ++i) {
				for (int j = 1; j <= counter; ++j) {
					imgui::PushID(i + 1000 + j);
					ImGui::ColorButton("", _terrains[i - 1][j - 1].color);
					imgui::PopID();

					if (j != counter) {
						imgui::SameLine();
					}

					int x = i - 1;
					int y = j - 1;
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_None)) {
						_clicked((int)clicked_flags::dbclick, x, y);
						continue;
					}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
						_clicked((int)clicked_flags::left, x, y);
						continue;
					}
					if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
						_clicked((int)clicked_flags::right, x, y);
						continue;
					}
				}
			}

			return true;
		}
	};
}