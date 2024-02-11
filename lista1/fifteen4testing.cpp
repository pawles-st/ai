#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <cassert>
#include <unistd.h>

const uint8_t BOARD_SIZE = 4; // assuming the value will fit on 4 bits
uint64_t states = 0;

using permutation = std::array<uint8_t, BOARD_SIZE * BOARD_SIZE>;

uint8_t calc_heuristic1(uint64_t perm);
uint8_t calc_heuristic2(uint64_t perm);

class Vertex {
	private:

	uint64_t value;
	uint64_t previous;
	uint8_t distance;
	uint8_t heuristic_value;

	public:

	Vertex(uint64_t value, uint64_t previous, uint8_t distance, uint8_t h_type) :
		value(value), previous(previous), distance(distance) {
		if (h_type == 1) {
			heuristic_value = calc_heuristic1(value) + distance;
		} else {
			heuristic_value = calc_heuristic2(value) + distance;
		}
	}

	uint64_t get_value() const {
		return value;
	}
	uint64_t get_previous() const {
		return previous;
	}
	uint8_t get_distance() const {
		return distance;
	}
	uint8_t get_heuristic() const {
		return heuristic_value;
	}
};

struct HeuristicComparator {
	bool operator()(const Vertex& v1, const Vertex& v2) const {return v1.get_heuristic() > v2.get_heuristic();};
};

using vertex_queue = std::priority_queue<Vertex, std::vector<Vertex>, HeuristicComparator>;

uint8_t linear_conflict(uint64_t perm) {
	const uint8_t mask = 0x0F;
	uint8_t heuristic_value = 0;
	std::vector<uint8_t> conflicts(BOARD_SIZE * BOARD_SIZE, 0);
	std::map<uint8_t, std::queue<uint8_t>> conf_values;
	for (int8_t i = BOARD_SIZE * BOARD_SIZE - 1; i >= 0 ; --i) {
		const uint8_t rev_i = BOARD_SIZE * BOARD_SIZE - 1 - i;
		const uint8_t value = (perm >> rev_i * 4) & mask;
		
		// row
		if ((value - 1) / BOARD_SIZE == i / BOARD_SIZE && value != 0) {
			//std::cout << (int)(i + 1) << " -> " << (int)value << "\n";
			for (int8_t j = i - 1; j >= 0 && j % BOARD_SIZE != BOARD_SIZE - 1; --j) {
				const uint8_t rev_j = BOARD_SIZE * BOARD_SIZE - 1 - j;
				const uint8_t sec_value = (perm >> rev_j * 4) & mask;
				if ((sec_value - 1) / BOARD_SIZE == j / BOARD_SIZE && sec_value >= value) {
					//std::cout << "i=" << (int)i << ", " << "j=" << (int)j << " - ";
					//std::cout << std::hex << (int)value << " conflicts with " << (int)sec_value << "\n";
					++conflicts[value];
					conf_values[sec_value].push(value);
					//std::cout << "pushed: " << (int)conf_values[value].front() << "\n";
				}
			}
		}

		// column
		//if (value % BOARD_SIZE == i % BOARD_SIZE) {
		//	for (int8_t j = i - BOARD_SIZE; j > 0; j -= BOARD_SIZE) {
		//		const uint8_t rev_j = BOARD_SIZE * BOARD_SIZE - 1 - j;
		//		const uint8_t sec_value = (perm >> rev_j) & mask;
		//		if (sec_value % BOARD_SIZE == j % BOARD_SIZE && sec_value >= value) {
		//			++conflicts[value];
		//			conf_values[value].push(sec_value);
		//		}
		//	}
		//}
	}
	auto M_iter = std::max_element(std::begin(conflicts), std::end(conflicts));
	while (*M_iter > 0) {
		*M_iter = 0;
		auto v = std::distance(std::begin(conflicts), M_iter);
		while (!conf_values[v].empty()) {
			uint8_t next = conf_values[v].front();
			--conflicts[next];
			conf_values[v].pop();
		}
		M_iter = std::max_element(std::begin(conflicts), std::end(conflicts));
		heuristic_value += 2;
	}
	return heuristic_value;
}

uint8_t manhattan(uint64_t perm) {
	const uint8_t mask = 0x0F;
	uint8_t heuristic_value = 0;
	for (int8_t i = BOARD_SIZE * BOARD_SIZE - 1; i >= 0; --i) {
		const uint8_t value = perm & mask;
		if (value != 0) {
			const int8_t curr_x = i % BOARD_SIZE;
			const int8_t curr_y = i / BOARD_SIZE;
			const int8_t goal_x = (value - 1) % BOARD_SIZE;
			const int8_t goal_y = (value - 1) / BOARD_SIZE;
			heuristic_value += std::abs(curr_x - goal_x) + std::abs(curr_y - goal_y);
		}
		perm >>= 4;
	}
	return heuristic_value;

}

// Manhattan + LC
uint8_t calc_heuristic2(uint64_t perm) {
	return manhattan(perm) + linear_conflict(perm);
}

// Manhattan
uint8_t calc_heuristic1(uint64_t perm) {
	return manhattan(perm);
}

uint64_t swap_digits(uint64_t value, const uint8_t  first, const uint8_t second) {
	const uint64_t mask = 0x0F;
	const uint8_t first_size = first * 4;
	const uint8_t second_size = second * 4;
	const uint64_t first_mask = mask << first_size;
	const uint64_t second_mask = mask << second_size;
	const uint64_t first_digit = value & first_mask;
	const uint64_t second_digit = value & second_mask;
	//std::cout << std::hex << first_digit << " " << std::dec;
	//std::cout << std::hex << second_digit << "\n" << std::dec;
	value ^= first_digit ^ second_digit;
	value ^= (first_digit >> first_size) << second_size;
	value ^= (second_digit >> second_size) << first_size;
	return value;
}

void add_move(Vertex& vertex, const uint8_t from, const uint8_t to, vertex_queue& vertex_q, const std::set<uint64_t>& visited, std::map<uint64_t, uint64_t>& parent, const uint8_t h_type) {
	const uint64_t curr_value = vertex.get_value();
	const uint64_t new_value = swap_digits(curr_value, from, to);
	if (visited.find(new_value) == std::end(visited)) {
		parent.insert({new_value, curr_value});
		Vertex new_v = Vertex(new_value, curr_value, vertex.get_distance() + 1, h_type);
		vertex_q.push(new_v);
	}
}

void process(Vertex& vertex, vertex_queue& vertex_q, const std::set<uint64_t>& visited, std::map<uint64_t, uint64_t>& parent, const uint8_t h_type) {

	// find index of the empty tile (counting from the right)

	uint8_t empty_idx;

	uint64_t value = vertex.get_value();
	uint64_t mask = 0x0F;
	for (uint8_t i = 0;; ++i) {
		if ((value & mask) == 0) {
			empty_idx = i;
			break;
		}
		value >>= 4;
	}
	//std::cout << "emtpy: " << empty_idx << "\n";
	//const auto empty_it = std::find(std::begin(vertex->get_value()), std::end(vertex->get_value()), 0);
	//const unsigned long long empty_idx = empty_it - std::begin(vertex->get_value());

	if (empty_idx < BOARD_SIZE * (BOARD_SIZE - 1)) { // move up
		add_move(vertex, empty_idx, empty_idx + BOARD_SIZE, vertex_q, visited, parent, h_type);
	}
	if ((empty_idx % BOARD_SIZE) != BOARD_SIZE - 1) { // move left
		add_move(vertex, empty_idx, empty_idx + 1, vertex_q, visited, parent, h_type);
	}
	if ((empty_idx % BOARD_SIZE) != 0) { // move right
		add_move(vertex, empty_idx, empty_idx - 1, vertex_q, visited, parent, h_type);
	}
	if (empty_idx >= BOARD_SIZE) { // move down
		add_move(vertex, empty_idx, empty_idx - BOARD_SIZE, vertex_q, visited, parent, h_type);
	}
}

bool parity(uint64_t p) {

	// transforming parameter into a permutation vector

	permutation perm;
	const uint8_t end = BOARD_SIZE * BOARD_SIZE - 1;
	for (int8_t i = end; i >= 0; --i) {
		const uint8_t value = p & 0x0F;
		p >>= 4;
		perm[i] = value;
	}

	// calculating parity of the permutation

	uint16_t parity = 0;
	std::vector<bool> visited(perm.size() - 1, 0);
	for (uint8_t i = 0; i < perm.size() - 1; ++i) {
		if (!visited[i]) {
			uint8_t x = i;
			uint8_t y = perm[i];
			do {
				++parity;
				visited[x] = true;
				x = y - 1;
				y = perm[y - 1];
			} while (x != i);
			++parity;
		}
	}
	return parity % 2 == 1;
}

int main() {

	///// initialisation

	// create structures for exploration

	std::set<uint64_t> visited;
	std::map<uint64_t, uint64_t> parent;
	vertex_queue vertex_q;

	// create the goal permutation

	const uint64_t goal = 0x123456789ABCDEF0;

	// create and verify the start permutation

	//unsigned long long start_perm = 0x3271D59A4C68FEB0;
	//uint64_t start_perm = 0x207b1536a94fed8c;
	//uint64_t start_perm = 0x9a7b12564dfce830;
	//uint64_t start_perm = 0x123456789abcdef0;
	//unsigned long long start_perm = 0xFEDCBA9876543120;
	permutation random;
	std::iota(std::begin(random), std::end(random) - 1, 1);
	*(std::end(random) - 1) = 0;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(std::begin(random), std::end(random) - 1, g);

	uint64_t start_perm = 0;
	for (auto v : random) {
		start_perm *= 16;
		start_perm += v;
	}
	std::cout << std::hex << start_perm << "\n";
	if (parity(start_perm) == true) {
		std::cout << "unsolvable\n";
		return 1;
	}
	std::cout << "solvable\n";

	///// explore the graph with A*
	states = 0;

	std::cout << "--- MH ---\n";

	{
		// create structures for exploration

		std::set<uint64_t> visited;
		std::map<uint64_t, uint64_t> parent;
		vertex_queue vertex_q;
		Vertex current(start_perm, 0, 0, 1);

		while (current.get_value() != goal) {
			//std::cout << std::hex << std::setw(16) << std::setfill('0') << current.get_value() << " - " << std::dec << +current.get_heuristic() << "\n";

			++states;

			// mark the current permutation as visited

			visited.insert(current.get_value());

			// add possible moves to the queue

			process(current, vertex_q, visited, parent, 1);
			//std::cout << "alloced: " << allocated * sizeof(Vertex) << "b\n";
			//std::cout << "map: " << visited.size() * sizeof(permutation) << "b\n"; 
			//std::cout << "queue: " << vertex_q.size() * sizeof(Vertex) << "b\n"; 

			// find next vertex

			while (true) {
				current = vertex_q.top();
				vertex_q.pop();
				if (visited.find(current.get_value()) == std::end(visited)) {
					break;
				}
			}
		}

		std::cout << "finished\n";

		const uint8_t distance = current.get_distance();
		std::cout << "found in " << std::dec << (int)distance << " moves\n";
		std::cout << "visited " << std::dec << states << " states in total\n";

		uint64_t curr_value = current.get_value();
		std::cout << std::hex << curr_value << " <- ";
		auto prev = parent.find(curr_value);
		while (prev != std::end(parent)) {
			std::cout << std::hex << std::setw(16) << std::setfill('0') << prev->second << " <- ";
			curr_value = prev->second;
			prev = parent.find(curr_value);
		}
	}
	states = 0;

	std::cout << "\n--- MH + LC ---\n";
	
	{
		// create structures for exploration

		std::set<uint64_t> visited;
		std::map<uint64_t, uint64_t> parent;
		vertex_queue vertex_q;
		Vertex current(start_perm, 0, 0, 2);

		while (current.get_value() != goal) {
			//std::cout << std::hex << std::setw(16) << std::setfill('0') << current.get_value() << " - " << std::dec << +current.get_heuristic() << "\n";

			++states;

			// mark the current permutation as visited

			visited.insert(current.get_value());

			// add possible moves to the queue

			process(current, vertex_q, visited, parent, 2);
			//std::cout << "alloced: " << allocated * sizeof(Vertex) << "b\n";
			//std::cout << "map: " << visited.size() * sizeof(permutation) << "b\n"; 
			//std::cout << "queue: " << vertex_q.size() * sizeof(Vertex) << "b\n"; 

			// find next vertex

			while (true) {
				current = vertex_q.top();
				vertex_q.pop();
				if (visited.find(current.get_value()) == std::end(visited)) {
					break;
				}
			}
		}

		std::cout << "finished\n";

		const uint8_t distance = current.get_distance();
		std::cout << "found in " << std::dec << (int)distance << " moves\n";
		std::cout << "visited " << std::dec << states << " states in total\n";

		uint64_t curr_value = current.get_value();
		std::cout << std::hex << curr_value << " <- ";
		auto prev = parent.find(curr_value);
		while (prev != std::end(parent)) {
			std::cout << std::hex << std::setw(16) << std::setfill('0') << prev->second << " <- ";
			curr_value = prev->second;
			prev = parent.find(curr_value);
		}
	}
	
	return 0;
}
