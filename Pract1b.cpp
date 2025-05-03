#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

struct Node {
    int data;
    vector<Node*> neighbors;
};

// Thread-safe DFS using OpenMP
void parallel_DFS(Node* node, vector<bool>& visited) {
    #pragma omp critical
    {
        if (visited[node->data]) return;
        visited[node->data] = true;
        cout << node->data << " ";
    }

    #pragma omp parallel for
    for (int i = 0; i < node->neighbors.size(); ++i) {
        Node* neighbor = node->neighbors[i];

        #pragma omp critical
        {
            if (!visited[neighbor->data]) {
                parallel_DFS(neighbor, visited);
            }
        }
    }
}

int main() {
    // Create a sample undirected graph (0-1-3, 0-2-4)
    vector<Node> graph(5);
    for (int i = 0; i < 5; ++i) {
        graph[i].data = i;
    }

    // Manual push_back for neighbors
    graph[0].neighbors.push_back(&graph[1]);
    graph[0].neighbors.push_back(&graph[2]);

    graph[1].neighbors.push_back(&graph[0]);
    graph[1].neighbors.push_back(&graph[3]);

    graph[2].neighbors.push_back(&graph[0]);
    graph[2].neighbors.push_back(&graph[4]);

    graph[3].neighbors.push_back(&graph[1]);

    graph[4].neighbors.push_back(&graph[2]);

    vector<bool> visited(graph.size(), false);

    cout << "Parallel DFS Output: ";
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallel_DFS(&graph[0], visited);
        }
    }
    cout << endl;

    return 0;
}

