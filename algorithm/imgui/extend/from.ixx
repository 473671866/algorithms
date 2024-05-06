module;
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

export module component.layout.from;
export import component.layout;
namespace imgui = ImGui;

export namespace view::layouts {
	class from : public layout {
	private:
		std::vector<std::pair<component*, component*>> _children;

	public:
		bool insert(component* left, component* right) {
			_children.push_back(std::make_pair(left, right));
			return true;
		}

		bool domain() {
			imgui::BeginGroup();
			for (auto [left, right] : _children) {
				left->domain();
				imgui::SameLine();
				right->domain();
			}
			imgui::EndGroup();
			return true;
		}
	};
}