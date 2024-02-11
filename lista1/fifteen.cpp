#include <algorithm>
#include <array>
#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <unistd.h>

int allocated = 0;

const unsigned char BOARD_SIZE = 4;

std::ostream& operator<<(std::ostream& s, const std::array<unsigned char, BOARD_SIZE * BOARD_SIZE> arr) {
	s << "(";
	for (const auto e : arr) {
		s << " " << +e;
	}
	return s << " )";
}

using permutation = std::array<unsigned char, BOARD_SIZE * BOARD_SIZE>;

unsigned char calc_heuristic(const permutation& perm);

class Vertex {
	private:

	permutation value;
	Vertex *previous;
	unsigned char distance;
	unsigned char heuristic_value;

	public:

	Vertex(permutation value, Vertex *previous, unsigned char distance) :
		value(value), previous(previous), distance(distance), heuristic_value(calc_heuristic(value) + get_distance()) {}

	void increase_heuristic(unsigned char h) {
		heuristic_value += h;
	}
	unsigned char get_heuristic() const {
		return heuristic_value;
	}
	Vertex* get_previous() const {
		return previous;
	}
	unsigned char get_distance() const {
		return distance;
	}
	const permutation& get_value() const {
		return value;
	}
};
	
struct HeuristicComparator {
	bool operator()(const Vertex* const v1, const Vertex* const v2) const {return v1->get_heuristic() > v2->get_heuristic();};
};

using vertex_queue = std::priority_queue<Vertex*, std::vector<Vertex*>, HeuristicComparator>;

unsigned char calc_heuristic(const permutation& perm) {
	unsigned char heuristic_value = 0;
	for (unsigned short i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
		if (perm[i] != 0) {
			char curr_x = i % BOARD_SIZE;
			char curr_y = i / BOARD_SIZE;
			char goal_x = (perm[i] - 1) % BOARD_SIZE;
			char goal_y = (perm[i] - 1) / BOARD_SIZE;
			heuristic_value += std::abs(curr_x - goal_x) + std::abs(curr_y - goal_y);
		}
	}
	return heuristic_value;
}

void process(Vertex* vertex, vertex_queue& vertex_q, const std::set<permutation>& visited) {
	const auto empty_it = std::find(std::begin(vertex->get_value()), std::end(vertex->get_value()), 0);
	const unsigned long long empty_idx = empty_it - std::begin(vertex->get_value());

	if (empty_idx >= BOARD_SIZE) { // move up
		permutation new_perm = vertex->get_value();
		std::swap(new_perm[empty_idx], new_perm[empty_idx - BOARD_SIZE]);
		if (visited.find(new_perm) == std::end(visited)) {
			++allocated;
			Vertex* new_v = new Vertex(new_perm, vertex, vertex->get_distance() + 1);
			vertex_q.push(new_v);
		}
	}
	if (empty_idx % BOARD_SIZE != 0) { // move left
		permutation new_perm = vertex->get_value();
		std::swap(new_perm[empty_idx], new_perm[empty_idx - 1]);
		if (visited.find(new_perm) == std::end(visited)) {
			++allocated;
			Vertex* new_v = new Vertex(new_perm, vertex, vertex->get_distance() + 1);
			vertex_q.push(new_v);
		}
	}
	if (empty_idx % BOARD_SIZE != BOARD_SIZE - 1) { // move right
		permutation new_perm = vertex->get_value();
		std::swap(new_perm[empty_idx], new_perm[empty_idx + 1]);
		if (visited.find(new_perm) == std::end(visited)) {
			++allocated;
			Vertex* new_v = new Vertex(new_perm, vertex, vertex->get_distance() + 1);
			vertex_q.push(new_v);
		}
	}
	if (empty_idx < BOARD_SIZE * (BOARD_SIZE - 1)) { // move down
		permutation new_perm = vertex->get_value();
		std::swap(new_perm[empty_idx], new_perm[empty_idx + BOARD_SIZE]);
		if (visited.find(new_perm) == std::end(visited)) {
			++allocated;
			Vertex* new_v = new Vertex(new_perm, vertex, vertex->get_distance() + 1);
			vertex_q.push(new_v);
		}
	}
}

unsigned short parity(const permutation& perm) {
	short parity;
	std::vector<bool> visited(perm.size(), 0);
	for (unsigned char i = 0; i < perm.size() - 1; ++i) {
		if (visited[i] == 0) {
			unsigned char x = i + 1;
			unsigned char y = perm[i];
			do {
				++parity;
				visited[x - 1] = 1;
				x = y;
				y = perm[y - 1];
			} while (x != i + 1);
			++parity;
		}
	}
	return parity % 2;
}

int main() {

	// initialisation

	std::set<permutation> visited;
	vertex_queue vertex_q;

	permutation goal;
	for (unsigned short i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
		goal[i] = (i + 1) % (BOARD_SIZE * BOARD_SIZE);
	}
	permutation start_perm{7, 1, 3, 2, 5, 9, 13, 10, 4, 12, 6, 8, 15, 14, 11, 0};
	if (parity(start_perm) == 1) {
		std::cout << "unsolvable\n";
		return 1;
	}
	Vertex start(start_perm, NULL, 0);
	Vertex *current = &start;

	int curr_dist = 0;

	while (current->get_value() != goal) {
		if (current->get_distance() > curr_dist) {
			curr_dist = current->get_distance();
			std::cout << curr_dist << "\n";
		}
		//std::cout << current->get_value() << " - " << +current->get_heuristic() << "\n";

		visited.insert(current->get_value()); // time: log
		
		process(current, vertex_q, visited); // add new vertices to the queue
		//std::cout << "alloced: " << allocated * sizeof(Vertex) << "b\n";
		//std::cout << "map: " << visited.size() * sizeof(permutation) << "b\n"; 
		//std::cout << "queue: " << vertex_q.size() * sizeof(Vertex) << "b\n"; 

		while (true) {
			current = vertex_q.top();
			vertex_q.pop();
			if (visited.find(current->get_value()) != std::end(visited)) {
				--allocated;
				delete current;
			} else {
				break;
			}
		}
	
//		assert(current->get_previous() != &current->;
	}

	unsigned char distance = current->get_distance();

	std::cout << current->get_value() << "\n";
	while (current->get_previous() != NULL) {
		current = current->get_previous();
		std::cout << current->get_value() << "\n";
	}

	std::cout << "found in " << +distance << " moves!\n";
	
	return 0;
}
