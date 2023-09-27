#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define VERTICES 7

struct Node {
    char label;
    struct Node* next;
    int cost; // Cost of the edge
};

struct Stack {
    char data;
    struct Stack* next;
    int costSoFar; // Cost to reach this node
};

struct ExtendedList {
    char path[VERTICES];
    int costSoFar;
};

void push(struct Stack** top, char data, int costSoFar) {
    struct Stack* newNode = (struct Stack*)malloc(sizeof(struct Stack));
    newNode->data = data;
    newNode->costSoFar = costSoFar;
    newNode->next = *top;
    *top = newNode;
}

char pop(struct Stack** top, int* costSoFar) {
    if (*top == NULL) {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
    struct Stack* temp = *top;
    char data = temp->data;
    *costSoFar = temp->costSoFar;
    *top = temp->next;
    free(temp);
    return data;
}

bool isGoalReached(char current, char goal) {
    return current == goal;
}

void branchAndBound(struct Node* adjacencyList[], char source, char goal, char* oraclePath, int* oracleCosts) {
    struct Stack* stack = NULL;
    bool visited[VERTICES] = {false};
    int depth = 0;
    int costSoFar = 0;
    char path[VERTICES];

    struct ExtendedList extendedList[VERTICES];
    int extendedListSize = 0;

    push(&stack, source, costSoFar);

    while (stack != NULL) {
        char current = pop(&stack, &costSoFar);
        path[depth] = current;

        if (current == oraclePath[depth]) {
            if (costSoFar == oracleCosts[depth]) {
                visited[current - 'A'] = true;

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
                        if (!visited[next - 'A']) {
                            push(&stack, next, costSoFar + neighbor->cost);
                        }
                        neighbor = neighbor->next;
                    }

                    depth++;
                }
            }
        }

        // Add to the extended list
        if (depth < VERTICES - 1) {
            strcpy(extendedList[extendedListSize].path, path);
            extendedList[extendedListSize].costSoFar = costSoFar;
            extendedListSize++;
        }
    }

    printf("No more valid paths found for the oracle.\n");

    // Display extended list
    printf("Extended List:\n");
    for (int i = 0; i < extendedListSize; i++) {
        printf("Path: %s (Cost: %d)\n", extendedList[i].path, extendedList[i].costSoFar);
    }
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
    char oraclePath[] = "SADG";
    int oracleCosts[] = {3, 3, 5, 0}; // Cost of each segment in the oracle path

    branchAndBound(adjacencyList, source, goal, oraclePath, oracleCosts);

    return 0;
}
