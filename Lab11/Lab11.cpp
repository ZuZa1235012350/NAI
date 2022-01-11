#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <limits.h>
#include <functional>

using namespace std;

class Graph {
	vector<vector<int>> matrix;
public:
	Graph(int size = 5) : matrix(size, vector<int>(size, 0)) {

	}

	string getDot() {
		string ms = "graph A{ \n";
		int i, j;
		for (i = 0; i < matrix.size(); i++) {
			for (j = i + 1; j < matrix.size(); j++) {
				if (matrix.at(i).at(j) == 1) {
					string w = to_string(i);
					string k = to_string(j);

					ms += "     " + w + "--" + k + " \n";
				}
			}
		}
		ms += "}";
		return ms;
	}
	void add_edge(int u, int v, int weight = 1) {
		matrix.at(u).at(v) = weight;
		matrix.at(v).at(u) = weight;
	}
	int get_edge(int u, int v) {
		return matrix.at(u).at(v);
	}
	void printMatrix() {
		for (auto& row : matrix) {
			for (auto col : row)
				cout << col << " ";
			cout << endl;
		}
	}
	void printSolution(vector<int>& dist)
	{

		cout << "Vertex \t Distance from Source" << endl;
		for (int i = 0; i < dist.size(); i++)
			cout << i << " \t\t" << dist[i] << endl;
	}
	//dijkstra
	int minDistance(vector<int>& dist, vector<bool>& visit) // finding minimum distance
	{
		int min = INT_MAX, min_index;

		for (int v = 0; v < dist.size(); v++)
			if (visit[v] == false && dist[v] <= min)
				min = dist[v], min_index = v;

		return min_index;
	}

	void dijkstra(int src, int target)
	{
		int visitNodes = 0;
		int len = matrix.size();
		vector<int> dist(len, INT_MAX); 
		vector<bool> visit(len, false);

		dist[src] = 0;
		for (int count = 0; count < len - 1; count++) {
			int u = minDistance(dist, visit);
			if (u == target) {
				break;
			}
			visit[u] = true;

			for (int v = 0; v < len; v++)
				if (!visit[v] && matrix[u][v] && dist[u] != INT_MAX
					&& dist[u] + matrix[u][v] < dist[v])
					dist[v] = dist[u] + matrix[u][v];
		}
		for (int i = 0; i < visit.size(); i++) {
			if (visit[i]) {
				visitNodes++;
			}
		}
		cout << "\nDijkstra" << endl;
		cout << "Visited nodes:" << visitNodes << endl;
		
		printSolution(dist);
	}
	//A*
	int minDistanceAStar(vector<int>& dist, vector<bool>& visit, int target, function<int(int, int)> f)
	{
		int min = INT_MAX, min_index;

		for (int v = 0; v < dist.size(); v++)
			if (visit[v] == false && dist[v] <= min - f(v, target))
				min = dist[v], min_index = v;

		return min_index;
	}
	void A_star(int src, int target, function<int(int, int)> f)
	{
		int visitNodes = 0;
		int len = matrix.size();
		vector<int> dist(len, INT_MAX);
		vector<bool> visit(len, false);

		dist[src] = 0;
		for (int count = 0; count < len - 1; count++) {
			int u = minDistanceAStar(dist, visit, target, f);
			if (u == target) {
				break;
			}
			visit[u] = true;

			for (int v = 0; v < len; v++) {
				if (!visit[v] && matrix[u][v] && dist[u] != INT_MAX
					&& dist[u] + matrix[u][v] < dist[v]) {
					dist[v] = dist[u] + matrix[u][v];
				}

			}

		}
		for (int i = 0; i < visit.size(); i++) {
			if (visit[i]) {
				visitNodes++;
			}
		}

		cout << "\nA star" << endl;
		cout << "Visited nodes:" << visitNodes << endl;
		printSolution(dist);
	}

};

Graph readFromFile(string filename) {
	ifstream inputdata;

	inputdata.open(filename);
	if (!inputdata) {
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}
	string s;
	getline(inputdata, s);
	Graph graph(25);
	while (true) {
		getline(inputdata, s);
		if (s[0] == '}') {
			break;
		}
		else {
			regex linePattern(" +(\\d+)--(\\d+) \\[weight=(\\d+)\\]");
			smatch matches;

			if (regex_search(s, matches, linePattern)) {
				//cout << "Match found\n";
				graph.add_edge(stoi(matches[1]), stoi(matches[2]), stoi(matches[3]));
			}
			else {
				throw invalid_argument("Nothing matches");
			}
		}
	}
	inputdata.close();
	return graph;
}

int main() {
	string filename;
	int start, end;

	cout << "File name: " << endl;
	cin >> filename;

	Graph graph = readFromFile(filename);

	// graph.printMatrix();
	if (filename == "demo.dot") {
		graph.dijkstra(0, 2);
		graph.A_star(0, 2, [](int, int) {return 0; });
	}
	else if (filename == "a_demo.dot") {
		graph.dijkstra(0, 24);
		graph.A_star(0, 24, [](int v1, int v2) {
			int x = v1 / 5;
			int y = v1 % 5;

			int x2 = v2 / 5;
			int y2 = v2 % 5;
			return abs(x - x2) + abs(y - y2);
			});
	}

	
	
	return 0;
}