#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

class Graph {
    vector<vector<int>> matrix;

public:
    Graph(int size = 20) : matrix(size, vector<int>(size,0)) {
        
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
    void add_edge(int u, int v) {
        matrix.at(u).at(v) = 1;
        matrix.at(v).at(u) = 1;
    }




};

void wirteToFile(string msg) {
    ofstream outdata;
        
    outdata.open("demo.dot"); 
    if (!outdata) { 
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    outdata << msg << endl;

    outdata.close();
}

int main() {
    Graph graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(0, 4);
    graph.add_edge(1, 2);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    string msg = graph.getDot();

    wirteToFile(msg);
    system("dot demo.dot -Tpng -o image.png");
	return 0;
}