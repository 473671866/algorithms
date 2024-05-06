module;
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

export module component.layout.horizontal;
export import component.layout;

namespace imgui = ImGui;

export namespace view::layouts {
	class horizontal : public layout {
	private:
		std::vector <component*> _children;

	public:
		horizontal() = default;

		bool insert(component* child) {
			_children.push_back(child);
			return true;
		}

		bool domain() {
			imgui::BeginGroup();
			for (auto child : _children) {
				child->domain();
				imgui::SameLine();
			}
			imgui::EndGroup();
			return true;
		}
	};
}