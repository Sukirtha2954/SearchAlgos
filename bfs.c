#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VERTICES 7

struct Node {
    char label;
    struct Node* next;
};

struct Queue {
    char data;
    struct Queue* next;
};

void enqueue(struct Queue** front, struct Queue** rear, char data) {
    struct Queue* newNode = (struct Queue*)malloc(sizeof(struct Queue));
    newNode->data = data;
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

void bfs(struct Node* adjacencyList[], char source, char goal) {
    struct Queue* front = NULL;
    struct Queue* rear = NULL;
    bool visited[VERTICES] = {false};
    int level[VERTICES] = {0};

    enqueue(&front, &rear, source);
    visited[source - 'A'] = true;

    while (front != NULL) {
        char current = dequeue(&front, &rear);

        if (isGoalReached(current, goal)) {
            printf("Path from S to G: ");
            printf("%c", current);

            // Print the path from S to G
            int parent = level[current - 'A'];
            while (parent != 0) {
                printf("-");
                for (char c = 'A'; c <= 'G'; c++) {
                    if (level[c - 'A'] == parent - 1 && adjacencyList[c - 'A'] != NULL) {
                        printf("%c", c);
                        parent--;
                        break;
                    }
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
                level[next - 'A'] = level[current - 'A'] + 1;
                enqueue(&front, &rear, next);
            }
            neighbor = neighbor->next;
        }
    }

    printf("No path from S to G found.\n");
}

int main() {
    struct Node* adjacencyList[VERTICES] = {NULL};

    // Add edges to the graph
    addEdge(adjacencyList, 'S', 'A');
    addEdge(adjacencyList, 'A', 'B');
    addEdge(adjacencyList, 'B', 'C');
    addEdge(adjacencyList, 'C', 'D');
    addEdge(adjacencyList, 'D', 'G');
    addEdge(adjacencyList, 'D', 'E');

    char source = 'S';
    char goal = 'G';

    bfs(adjacencyList, source, goal);

    return 0;
}
