#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VERTICES 7

struct Node {
    char label;
    struct Node* next;
    int cost; // Cost of the edge
};

struct Queue {
    char data;
    struct Queue* next;
    int level; // Level of the node
};

void enqueue(struct Queue** front, struct Queue** rear, char data, int level) {
    struct Queue* newNode = (struct Queue*)malloc(sizeof(struct Queue));
    newNode->data = data;
    newNode->level = level;
    newNode->next = NULL;
    if (*rear == NULL) {
        *front = *rear = newNode;
    } else {
        (*rear)->next = newNode;
        *rear = newNode;
    }
}

char dequeue(struct Queue** front, struct Queue** rear) {
    if (*front == NULL) {
        fprintf(stderr, "Queue is empty!\n");
        exit(1);
    }
    struct Queue* temp = *front;
    char data = temp->data;
    *front = temp->next;
    if (*front == NULL) {
        *rear = NULL;
    }
    free(temp);
    return data;
}

bool isGoalReached(char current, char goal) {
    return current == goal;
}

void beamSearch(struct Node* adjacencyList[], char source, char goal) {
    struct Queue* front = NULL;
    struct Queue* rear = NULL;
    struct Queue* beamFront = NULL;
    struct Queue* beamRear = NULL;
    bool visited[VERTICES] = {false};
    int pathCost = 0;
    char path[VERTICES];
    int depth = 0;
    int beamWidth = 2; // Set the beam width to 2

    enqueue(&front, &rear, source, 0);

    while (front != NULL) {
        char current = dequeue(&front, &rear);
        path[depth] = current;

        if (isGoalReached(current, goal)) {
            printf("Path from S to G: ");
            for (int i = 0; i <= depth; i++) {
                printf("%c", path[i]);
                if (i < depth) {
                    printf("-");
                }
            }
            printf("\n");
            return;
        }

        struct Node* neighbor = adjacencyList[current - 'A'];

        while (neighbor != NULL) {
            char next = neighbor->label;
            if (!visited[next - 'A']) {
                visited[next - 'A'] = true;
                enqueue(&beamFront, &beamRear, next, depth + 1);
            }
            neighbor = neighbor->next;
        }

        // Sort beamFront lexicographically
        int count[VERTICES] = {0};
        struct Queue* temp = beamFront;

        while (temp != NULL) {
            count[temp->data - 'A']++;
            temp = temp->next;
        }

        for (char c = 'A'; c <= 'G'; c++) {
            while (count[c - 'A'] > 0 && beamWidth > 0) {
                enqueue(&front, &rear, c, depth + 1);
                count[c - 'A']--;
                beamWidth--;
            }
        }

        depth++;
    }

    printf("No path from S to G found.\n");
}

int main() {
    struct Node* adjacencyList[VERTICES] = {NULL};

    // Add edges to the graph with associated costs
    addEdge(adjacencyList, 'S', 'A', 3);
    addEdge(adjacencyList, 'S', 'B', 4);
    addEdge(adjacencyList, 'B', 'C', 4);
    addEdge(adjacencyList, 'C', 'E', 6);
    addEdge(adjacencyList, 'B', 'G', 6);
    addEdge(adjacencyList, 'C', 'G', 7); // Plus sign indicates the cost is higher
    addEdge(adjacencyList, 'A', 'D', 3);
    addEdge(adjacencyList, 'D', 'G', 5);
    addEdge(adjacencyList, 'A', 'G', 7); // Plus sign indicates the cost is higher

    char source = 'S';
    char goal = 'G';

    beamSearch(adjacencyList, source, goal);

    return 0;
}
