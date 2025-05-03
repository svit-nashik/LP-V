#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

struct Node {
    int data;
    vector<Node*> children;
};

void parallel_BFS(Node* root) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();
        vector<Node*> current_level;

        // Copy current level nodes
        for (int i = 0; i < level_size; ++i) {
            current_level.push_back(q.front());
            q.pop();
        }

        vector<Node*> next_level;

        // Process current level nodes in parallel
        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); ++i) {
            Node* current = current_level[i];

            // Print node data
            #pragma omp critical
            {
                cout << current->data << " ";
            }

            // Push children into next_level
            #pragma omp critical
            {
                for (int j = 0; j < current->children.size(); ++j) {
                    next_level.push_back(current->children[j]);
                }
            }
        }

        // Add all next_level nodes to the queue
        for (int i = 0; i < next_level.size(); ++i) {
            q.push(next_level[i]);
        }
    }
}

int main() {
    // Create a sample N-ary tree
    Node* root = new Node;
    root->data = 1;

    Node* child1 = new Node;
    child1->data = 2;

    Node* child2 = new Node;
    child2->data = 3;

    Node* grandchild = new Node;
    grandchild->data = 4;

    child2->children.push_back(grandchild);

    root->children.push_back(child1);
    root->children.push_back(child2);

    cout << "Parallel BFS Output: ";
    parallel_BFS(root);
    cout << endl;

    return 0;
}

