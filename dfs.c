#include <stdio.h>
#include <stdbool.h>

#define VERTICES 7

struct Node {
    char label;
    struct Node* next;
};

bool visited[VERTICES] = {false};
char path[VERTICES];

struct Node* adjacencyList[VERTICES] = {NULL};

void addEdge(char src, char dest) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->label = dest;
    newNode->next = adjacencyList[src - 'A'];
    adjacencyList[src - 'A'] = newNode;
}

bool dfs(char current, char goal, int depth) {
    path[depth] = current;
    visited[current - 'A'] = true;

    if (current == goal) {
        for (int i = 0; i <= depth; i++) {
            printf("%c", path[i]);
            if (i < depth) {
                printf("-");
            }
        }
        printf("\n");
        return true;
    }

    struct Node* neighbor = adjacencyList[current - 'A'];

    while (neighbor != NULL) {
        char next = neighbor->label;
        if (!visited[next - 'A']) {
            if (dfs(next, goal, depth + 1)) {
                return true;
            }
        }
        neighbor = neighbor->next;
    }

    visited[current - 'A'] = false;
    return false;
}

int main() {
   
    addEdge('S', 'A');
    addEdge('A', 'D');
    addEdge('D', 'G');

    char source = 'S';
    char goal = 'G';

    printf("Path from S to G: ");
    if (!dfs(source, goal, 0)) {
        printf("No path found.\n");
    }

    return 0;
}


