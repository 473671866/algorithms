module;
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <vector>
#include <algorithm>

export module component.layout.vertical;
export import component.layout;

namespace imgui = ImGui;

export namespace view::layouts {
	class vertical : public layout {
	private:
		std::vector <component*> _children;

	public:
		bool insert(component* child) {
			_children.push_back(child);
			return true;
		}

		bool domain() {
			imgui::BeginGroup();
			for (auto child : _children) {
				child->domain();
			}
			imgui::EndGroup();
			return true;
		}
	};
}