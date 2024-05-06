module;
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <thread>
#include <chrono>
#include <unordered_set>
#include <unordered_map>

#include <imgui.h>
#include "entity.inl"

export module algorithm;
namespace imgui = ImGui;

export
class algorithm {
public:
	virtual void strartup(terrain(*terrains)[counter], pointer startend, pointer endend) = 0;

	int manhattan(pointer p1, pointer p2) {
		return  std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
	}
};

export
class bread : public algorithm {
public:
	void strartup(terrain(*terrains)[counter], pointer startend, pointer endend) override {
		std::queue<pointer> q;                                 // 用于广度优先搜索的队列
		std::unordered_set<int> visited;                       // 用于记录已经访问过的位置
		std::unordered_map<int, pointer> parent;               // 用于记录每个位置的父节点，用于回溯路径

		q.push(startend);                                      // 将起始位置加入队列
		visited.insert(startend.x * counter + startend.y);     // 标记起始位置为已访问

		while (!q.empty()) {
			pointer cur = q.front();                            // 取出队首位置
			q.pop();

			if (cur.x == endend.x && cur.y == endend.y) {       // 如果当前位置是目标位置，停止搜索
				break;
			}

			// 遍历当前位置的上下左右四个相邻位置
			for (pointer search : { pointer{ cur.x + 1, cur.y },
				pointer{ cur.x - 1, cur.y },
				pointer{ cur.x, cur.y + 1 },
				pointer{ cur.x, cur.y - 1 } }
			) {
				// 检查相邻位置是否合法且未访问过
				if (search.x > counter - 1 || search.y > counter - 1 || search.x < 0 || search.y < 0) {
					continue;
				}

				terrain& temp = terrains[search.x][search.y];
				if (temp.staple == staples::none) {
					temp.color = DimGray;
					temp.staple = staples::watched;
				}

				if (visited.find(search.x * counter + search.y) == visited.end() && temp.staple != staples::deadend) {
					q.push(search);                                 // 将相邻位置加入队列
					visited.insert(search.x * counter + search.y);     // 标记相邻位置为已访问
					parent[search.x * counter + search.y] = cur;       // 记录相邻位置的父节点为当前位置
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		// 回溯路径并标记颜色
		pointer cur = endend;
		while (cur.x != startend.x || cur.y != startend.y) {
			terrains[cur.x][cur.y].color = Green;               // 标记路径上的位置为绿色
			cur = parent[cur.x * counter + cur.y];              // 获取当前位置的父节点
		}
	}
};

export
class dijkstra : public algorithm {
public:
	void strartup(terrain(*terrains)[counter], pointer startend, pointer endend) override {
		std::stack<pointer> record;												//记录走过的路
		std::vector<pointer>queue;												//用来保存四个方向
		std::vector<pointer>* coming = new std::vector<pointer>();				//记录这个坐标有没有来过
		std::vector<pointer> result;
		pointer cur = startend;													//当前位置
		record.push(cur);
		coming->push_back(cur);

		auto cmp = [](pointer p1, pointer p2) -> bool {return p1.value < p2.value; };

		while (true) {
			for (pointer search : { pointer{ cur.x + 1, cur.y },
				pointer{ cur.x - 1, cur.y },
				pointer{ cur.x, cur.y + 1 },
				pointer{ cur.x, cur.y - 1 },
			}) {
				if (search.x > counter - 1 || search.y > counter - 1 || search.x < 0 || search.y < 0) {
					continue;
				}

				terrain& temp = terrains[search.x][search.y];
				if (temp.staple == staples::none) {
					temp.color = DimGray;
					temp.staple = staples::watched;
				}

				//来过了
				auto check = [search](pointer p) {return search.x == p.x && search.y == p.y; };
				if (std::find_if(coming->begin(), coming->end(), check) != coming->end()) {
					continue;
				}

				//判断障碍
				if (temp.staple != staples::deadend) {
					search.value = manhattan(search, endend);				//计算权值
					queue.push_back(search);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			//如果队列是空的就说明遇到死胡同了，尝试住回走
			if (queue.empty()) {
				if (!record.empty()) {
					cur = record.top();
					record.pop();
				}
			}

			if (!queue.empty()) {
				std::sort(queue.begin(), queue.end(), cmp);												//排序
				cur = queue.front();																	//走到最优先的地方
				record.push(cur);																		//保存路径
				coming->push_back(cur);																	//标记来过了
				queue.clear();
			}

			//找到了
			if (cur.x == endend.x && cur.y == endend.y) {
				while (!record.empty()) {
					result.push_back(record.top());
					record.pop();
				}
				reverse(result.begin(), result.end());
				break;
			}

			//没路走
			if (record.empty()) {
				break;
			}
		}
		delete coming;

		if (result.empty()) {
			return;
		}

		for (pointer p : result) {
			terrains[p.x][p.y].color = Green;
			terrains[p.x][p.y].staple = staples::startend;
		}
	}
};

export
class astar : public algorithm {
public:
	void strartup(terrain(*terrains)[counter], pointer startend, pointer endend) override {
		std::stack<pointer> record;												//记录走过的路
		std::vector<pointer>queue;												//用来保存四个方向
		std::vector<pointer>* coming = new std::vector<pointer>();				//记录这个坐标有没有来过
		std::vector<pointer> result;
		pointer cur = startend;													//当前位置
		record.push(cur);
		coming->push_back(cur);

		auto cmp = [](pointer p1, pointer p2) -> bool {return p1.value < p2.value; };

		while (true) {
			for (pointer search : { pointer{ cur.x + 1, cur.y },
				pointer{ cur.x - 1, cur.y },
				pointer{ cur.x, cur.y + 1 },
				pointer{ cur.x, cur.y - 1 },
				pointer{ cur.x + 1, cur.y + 1 },
				pointer{ cur.x + 1, cur.y - 1 },
				pointer{ cur.x - 1, cur.y + 1 },
				pointer{ cur.x - 1, cur.y - 1 },
			})
			{
				if (search.x > counter - 1 || search.y > counter - 1 || search.x < 0 || search.y < 0) {
					continue;
				}

				terrain& temp = terrains[search.x][search.y];
				if (temp.staple == staples::none) {
					temp.color = DimGray;
					temp.staple = staples::watched;
				}

				//来过了
				auto check = [search](pointer p) {return search.x == p.x && search.y == p.y; };
				if (std::find_if(coming->begin(), coming->end(), check) != coming->end()) {
					continue;
				}

				//判断障碍
				if (temp.staple != staples::deadend) {
					search.value = manhattan(search, endend) - manhattan(search, startend);				//计算权值
					queue.push_back(search);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			//如果队列是空的就说明遇到死胡同了，尝试住回走
			if (queue.empty()) {
				if (!record.empty()) {
					cur = record.top();
					record.pop();
				}
			}

			if (!queue.empty()) {
				std::sort(queue.begin(), queue.end(), cmp);												//排序
				cur = queue.front();																	//走到最优先的地方
				record.push(cur);																		//保存路径
				coming->push_back(cur);																	//标记来过了
				queue.clear();
			}

			//找到了
			if (cur.x == endend.x && cur.y == endend.y) {
				while (!record.empty()) {
					result.push_back(record.top());
					record.pop();
				}
				reverse(result.begin(), result.end());
				break;
			}

			//没路走
			if (record.empty()) {
				break;
			}
		}
		delete coming;

		if (result.empty()) {
			return;
		}

		for (pointer p : result) {
			terrains[p.x][p.y].color = Green;
			terrains[p.x][p.y].staple = staples::startend;
		}
	}
};

export algorithm* algorithms[3] = { new bread ,new dijkstra, new astar };