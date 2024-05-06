#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include <imgui.h>
#include "entity.inl"

import algorithm;
import graphical;
import component.control.guid;
import component.control.label;
import component.control.block;
import component.control.combo;
import component.control.button;
import component.layout.from;
import component.layout.aside;

namespace imgui = ImGui;

int main() {
	pointer startend;
	pointer endend;
	terrain terrains[counter][counter];
	auto initialize = [&] {
		for (int i = 0; i < counter; i++) {
			for (int j = 0; j < counter; j++) {
				terrain& temp = terrains[i][j];;
				temp.color = DarkGray;
				temp.staple = staples::none;
			}
		}

		terrains[0][0].color = Green;
		terrains[0][0].staple = staples::startend;
		terrains[18][18].color = Yellow;
		terrains[18][18].staple = staples::endend;
		startend = { 0, 0, 0 };
		endend = { 18, 18, 0 };
		};
	initialize();

	auto callback = [&](int flag, int i, int j) {
		clicked_flags flags = static_cast<clicked_flags>(flag);
		terrain& temp = terrains[i][j];
		switch (flags) {
		case clicked_flags::left:
			if (temp.staple != staples::deadend) {
				temp.staple = staples::deadend;
				temp.color = Red;
			}
			else {
				temp.staple = staples::none;
				temp.color = DarkGray;
			}
			break;
		case clicked_flags::right:
			terrains[startend.x][startend.y].color = DarkGray;
			terrains[startend.x][startend.y].staple = staples::none;
			temp.color = Green;
			temp.staple = staples::startend;
			startend = { i, j, 0 };
			break;
		case clicked_flags::dbclick:
			terrains[endend.x][endend.y].color = DarkGray;
			terrains[endend.x][endend.y].staple = staples::none;
			temp.color = Yellow;
			temp.staple = staples::endend;
			endend = { i, j, 0 };
			break;
		}
		return true;
		};

	view::ctrl::guid guid;
	guid.terrains(terrains);
	guid.clicked(callback);

	view::layouts::from from;

	view::ctrl::label startlabel("startend: ");
	view::ctrl::block startblock("##startend", Green);
	from.insert(&startlabel, &startblock);

	view::ctrl::label endlabel("endend:   ");
	view::ctrl::block endblock("##endend", Yellow);
	from.insert(&endlabel, &endblock);

	view::ctrl::label deadlabel("deadend:  ");
	view::ctrl::block deadblock("##deadend", Red);
	from.insert(&deadlabel, &deadblock);

	view::ctrl::label watchlabel("watched:  ");
	view::ctrl::block watchblock("##watched", DimGray);
	from.insert(&watchlabel, &watchblock);

	static int idx = 0;
	view::ctrl::button start("start");
	start.clicked([&] {
		algorithm* algo = algorithms[idx];
		std::thread(&algorithm::strartup, algo, terrains, startend, endend).detach();
		});

	view::ctrl::button reset("reset");
	reset.clicked(initialize);
	from.insert(&start, &reset);

	view::ctrl::combo combo("##algorithm", "Brand\0Dijkstra\0AStar\0");
	combo.changed([&](int selected) {idx = selected; });

	view::layouts::aside aside;
	aside.asided(&combo);
	aside.asided(&from);
	aside.maine(&guid);

	view::graphiacl graph;
	graph.external(L"example");
	graph.principal(&aside);
	graph.flags(ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove);

	ImGuiStyle& style = graph.styles();
	style.Colors[ImGuiCol_WindowBg] = ImColor(36, 40, 49, 255).Value;
	style.FrameRounding = 2.0f;

	graph.startup();
	return 0;
}