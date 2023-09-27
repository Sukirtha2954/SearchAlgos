#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a node in the graph
struct Node {
    char label;
    struct Node* next;
};

// Function to add a node to the adjacency list
void addEdge(struct Node* adjacencyList[], char src, char dest) {
    // Create a new node for the destination vertex
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->label = dest;
    newNode->next = adjacencyList[src - 'A'];
    adjacencyList[src - 'A'] = newNode;
}

// Function to perform a Depth-First Search (DFS)
void dfs(struct Node* adjacencyList[], char source, char goal, char path[], int depth) {
    path[depth] = source;

    // If the current node is the goal, print the path
    if (source == goal) {
        for (int i = 0; i <= depth; i++) {
            printf("%c", path[i]);
            if (i < depth)
                printf("-");
        }
        printf("\n");
        return;
    }

    // Recursively explore adjacent nodes
    struct Node* current = adjacencyList[source - 'A'];
    while (current != NULL) {
        dfs(adjacencyList, current->label, goal, path, depth + 1);
        current = current->next;
    }
}

int main() {
    // Number of vertices in the graph
    int numVertices = 7;

    // Create an array of linked lists to represent the adjacency list
    struct Node* adjacencyList[7] = { NULL };

    // Add edges to the graph
    addEdge(adjacencyList, 'S', 'A');
    addEdge(adjacencyList, 'A', 'B');
    addEdge(adjacencyList, 'A', 'D');
    addEdge(adjacencyList, 'B', 'C');
    addEdge(adjacencyList, 'C', 'D');
    addEdge(adjacencyList, 'D', 'E');
    addEdge(adjacencyList, 'D', 'G');

    // Initialize a path array to store the current path
    char path[7];

    printf("Paths from S to G:\n");

    // Find and print the path from S to G
    dfs(adjacencyList, 'S', 'G', path, 0);

    // Find and print the path from S to A to D to G
    printf("\nPaths from S to A to D to G:\n");
    dfs(adjacencyList, 'S', 'A', path, 0);
    dfs(adjacencyList, 'A', 'D', path, 0);
    dfs(adjacencyList, 'D', 'G', path, 0);

    return 0;
}
