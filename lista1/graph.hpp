#include <map>

#ifndef GRAPH_HPP
#define GRAPH_HPP

using nodes_t = unsigned long long;

//template <typename T>
//using edges_vector<T> = std::vector<std::pair<T, T>>;

template <typename T>
struct Node {
	T value;
	std::vector<Node<T>*> neighbours;
	bool visited;

	Node(const T& t) {
		value = t;
	}
};

template <typename K, typename T>
class Graph {
	private:

	std::map<K, Node<T>> nodes;
	bool directed;

	public:

	// print the graph to std::ostream
	//std::ostream& operator<<(std::ostream& s, const Graph& graph);

	// create an empty graph
	Graph(bool is_directed) {
		directed = is_directed;
	}

	// create a graph from an edges vector
	//Graph(nodes_t no_nodes, bool is_directed, const edges_vector& edges);

	// create a deep copy of a graph
	//Graph(const Graph& graph);

	// add a an empty node to the graph
	void add_node(const K& key, const Node<T>& node) {
		nodes.insert(std::make_pair(key, node));
	}

	// add an edge between nodes identified by two keys
	void add_edge(const K& key1, const K& key2) {
		nodes[key1].neighbours.push_back(&nodes[key2]);
		if (!directed) {
			nodes[key2].neighbours.push_back(&nodes[key1]);
		}
	}
	
	// get a node with the given key
	Node<T>* get_node(const K& key) {
		return &nodes[key];
	}

	// mark all nodes as unvisited
	//void reset();

	// destruct the graph
	~Graph() {}
};

#endif
