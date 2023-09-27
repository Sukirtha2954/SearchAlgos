#include <stdio.h>
#include <stdbool.h>

#define VERTICES 7

struct Node {
    char label;
    struct Node* next;
    int cost; // Cost of the edge
};

struct QueueNode {
    char label;
    int costSoFar;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

void enqueue(struct Queue* queue, char label, int costSoFar) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->label = label;
    newNode->costSoFar = costSoFar;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

char dequeue(struct Queue* queue, int* costSoFar) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty!\n");
        exit(1);
    }

    struct QueueNode* temp = queue->front;
    char label = temp->label;
    *costSoFar = temp->costSoFar;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return label;
}

bool isGoalReached(char current, char goal) {
    return current == goal;
}

void AStar(struct Node* adjacencyList[], char source, char goal, char* oraclePath, int* oracleCosts) {
    struct Queue queue = {NULL, NULL};
    int depth = 0;
    int costSoFar = 0;
    char path[VERTICES];

    enqueue(&queue, source, costSoFar);

    while (queue.front != NULL) {
        char current = dequeue(&queue, &costSoFar);
        path[depth] = current;

        if (current == oraclePath[depth]) {
            if (costSoFar == oracleCosts[depth]) {
                if (isGoalReached(current, goal) && depth == strlen(oraclePath) - 1) {
                    printf("Path found: ");
                    for (int i = 0; i <= depth; i++) {
                        printf("%c", path[i]);
                        if (i < depth) {
                            printf("-");
                        }
                    }
                    printf(" (Cost: %d)\n", costSoFar);
                    depth++;
                } else {
                    struct Node* neighbor = adjacencyList[current - 'A'];

                    while (neighbor != NULL) {
                        char next = neighbor->label;
                        int newCost = costSoFar + neighbor->cost;

                        enqueue(&queue, next, newCost);
                        neighbor = neighbor->next;
                    }

                    depth++;
                }
            }
        }
    }

    printf("No more valid paths found for the oracle.\n");
}

int main() {
    struct Node* adjacencyList[VERTICES] = {NULL};

    // Add edges to the graph with associated costs
    // Replace with actual edge information

    char source = 'S';
    char goal = 'G';
    char oraclePath[] = "SADG";
    int oracleCosts[] = {3, 3, 5, 0}; // Cost of each segment in the oracle path

    AStar(adjacencyList, source, goal, oraclePath, oracleCosts);

    return 0;
}
