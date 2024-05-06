module;
#include <iostream>
#include <functional>

#include <imgui.h>

export module component.control;
export import component;
namespace imgui = ImGui;

export namespace view::ctrl {
	class control :public component {
	protected:
		std::string_view _text;
		int _id = 0;
		ImVec2 _size = { 0, 0 };

		std::function<void()> _clicked;
		std::function<void()> _pressed;
		std::function<void()> _released;

	public:
		control() = default;

		control(std::string_view text, int id, ImVec2 size) :_text(text), _id(id), _size(size) {}

		virtual bool text(std::string_view text) {
			_text = text;
			return true;
		}

		virtual bool id(int id) {
			_id = id;
			return true;
		}

		virtual bool size(ImVec2 size) {
			_size = size;
			return true;
		}

		virtual bool clicked(const std::function<void()>& callback) {
			_clicked = callback;
			return true;
		}

		virtual bool pressed(const std::function<void()>& callback) {
			_pressed = callback;
			return true;
		}

		virtual bool released(const std::function<void()>& callback) {
			_released = callback;
			return true;
		}
	};
}