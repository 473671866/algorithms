module;
#include <iostream>
#include <vector>
#include <imgui.h>

export module component.layout.aside;
export import component.layout;

namespace imgui = ImGui;

export namespace view::layouts {
	class aside :public layout {
	private:
		std::vector<component*> _asides;
		std::vector<component*> _mains;
		float _barsize = 180.f;

	public:

		bool asided(component* aside) {
			_asides.push_back(aside);
			return true;
		}

		bool maine(component* main) {
			_mains.push_back(main);
			return true;
		}

		bool domain() {
			auto draw = imgui::GetWindowDrawList();
			auto pos = imgui::GetWindowPos();
			auto wndsize = imgui::GetWindowSize();

			draw->AddRectFilled({ wndsize.x - _barsize, 0 }, { wndsize.x, wndsize.y }, IM_COL32(56, 62, 74, 255));

			ImGui::SetCursorPosX(wndsize.x - _barsize + 10);
			ImGui::SetCursorPosY(10);
			imgui::BeginGroup();

			for (auto aside : _asides) {
				aside->domain();
			}

			imgui::EndGroup();

			ImGui::SetCursorPosX(5);
			ImGui::SetCursorPosY(5);
			imgui::BeginGroup();

			for (auto main : _mains) {
				main->domain();
			}

			imgui::EndGroup();
			return true;
		}
	};
}