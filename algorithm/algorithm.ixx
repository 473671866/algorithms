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
		std::queue<pointer> q;                                 // ���ڹ�����������Ķ���
		std::unordered_set<int> visited;                       // ���ڼ�¼�Ѿ����ʹ���λ��
		std::unordered_map<int, pointer> parent;               // ���ڼ�¼ÿ��λ�õĸ��ڵ㣬���ڻ���·��

		q.push(startend);                                      // ����ʼλ�ü������
		visited.insert(startend.x * counter + startend.y);     // �����ʼλ��Ϊ�ѷ���

		while (!q.empty()) {
			pointer cur = q.front();                            // ȡ������λ��
			q.pop();

			if (cur.x == endend.x && cur.y == endend.y) {       // �����ǰλ����Ŀ��λ�ã�ֹͣ����
				break;
			}

			// ������ǰλ�õ����������ĸ�����λ��
			for (pointer search : { pointer{ cur.x + 1, cur.y },
				pointer{ cur.x - 1, cur.y },
				pointer{ cur.x, cur.y + 1 },
				pointer{ cur.x, cur.y - 1 } }
			) {
				// �������λ���Ƿ�Ϸ���δ���ʹ�
				if (search.x > counter - 1 || search.y > counter - 1 || search.x < 0 || search.y < 0) {
					continue;
				}

				terrain& temp = terrains[search.x][search.y];
				if (temp.staple == staples::none) {
					temp.color = DimGray;
					temp.staple = staples::watched;
				}

				if (visited.find(search.x * counter + search.y) == visited.end() && temp.staple != staples::deadend) {
					q.push(search);                                 // ������λ�ü������
					visited.insert(search.x * counter + search.y);     // �������λ��Ϊ�ѷ���
					parent[search.x * counter + search.y] = cur;       // ��¼����λ�õĸ��ڵ�Ϊ��ǰλ��
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		// ����·���������ɫ
		pointer cur = endend;
		while (cur.x != startend.x || cur.y != startend.y) {
			terrains[cur.x][cur.y].color = Green;               // ���·���ϵ�λ��Ϊ��ɫ
			cur = parent[cur.x * counter + cur.y];              // ��ȡ��ǰλ�õĸ��ڵ�
		}
	}
};

export
class dijkstra : public algorithm {
public:
	void strartup(terrain(*terrains)[counter], pointer startend, pointer endend) override {
		std::stack<pointer> record;												//��¼�߹���·
		std::vector<pointer>queue;												//���������ĸ�����
		std::vector<pointer>* coming = new std::vector<pointer>();				//��¼���������û������
		std::vector<pointer> result;
		pointer cur = startend;													//��ǰλ��
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

				//������
				auto check = [search](pointer p) {return search.x == p.x && search.y == p.y; };
				if (std::find_if(coming->begin(), coming->end(), check) != coming->end()) {
					continue;
				}

				//�ж��ϰ�
				if (temp.staple != staples::deadend) {
					search.value = manhattan(search, endend);				//����Ȩֵ
					queue.push_back(search);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			//��������ǿյľ�˵����������ͬ�ˣ�����ס����
			if (queue.empty()) {
				if (!record.empty()) {
					cur = record.top();
					record.pop();
				}
			}

			if (!queue.empty()) {
				std::sort(queue.begin(), queue.end(), cmp);												//����
				cur = queue.front();																	//�ߵ������ȵĵط�
				record.push(cur);																		//����·��
				coming->push_back(cur);																	//���������
				queue.clear();
			}

			//�ҵ���
			if (cur.x == endend.x && cur.y == endend.y) {
				while (!record.empty()) {
					result.push_back(record.top());
					record.pop();
				}
				reverse(result.begin(), result.end());
				break;
			}

			//û·��
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
		std::stack<pointer> record;												//��¼�߹���·
		std::vector<pointer>queue;												//���������ĸ�����
		std::vector<pointer>* coming = new std::vector<pointer>();				//��¼���������û������
		std::vector<pointer> result;
		pointer cur = startend;													//��ǰλ��
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

				//������
				auto check = [search](pointer p) {return search.x == p.x && search.y == p.y; };
				if (std::find_if(coming->begin(), coming->end(), check) != coming->end()) {
					continue;
				}

				//�ж��ϰ�
				if (temp.staple != staples::deadend) {
					search.value = manhattan(search, endend) - manhattan(search, startend);				//����Ȩֵ
					queue.push_back(search);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			//��������ǿյľ�˵����������ͬ�ˣ�����ס����
			if (queue.empty()) {
				if (!record.empty()) {
					cur = record.top();
					record.pop();
				}
			}

			if (!queue.empty()) {
				std::sort(queue.begin(), queue.end(), cmp);												//����
				cur = queue.front();																	//�ߵ������ȵĵط�
				record.push(cur);																		//����·��
				coming->push_back(cur);																	//���������
				queue.clear();
			}

			//�ҵ���
			if (cur.x == endend.x && cur.y == endend.y) {
				while (!record.empty()) {
					result.push_back(record.top());
					record.pop();
				}
				reverse(result.begin(), result.end());
				break;
			}

			//û·��
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