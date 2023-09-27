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
};

void push(struct Stack** top, char data) {
    struct Stack* newNode = (struct Stack*)malloc(sizeof(struct Stack));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

char pop(struct Stack** top) {
    if (*top == NULL) {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
    struct Stack* temp = *top;
    char data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

bool isGoalReached(char current, char goal) {
    return current == goal;
}

void hillClimbing(struct Node* adjacencyList[], char source, char goal) {
    struct Stack* stack = NULL;
    bool visited[VERTICES] = {false};
    int pathCost = 0;
    char path[VERTICES];
    int depth = 0;

    push(&stack, source);

    while (stack != NULL) {
        char current = pop(&stack);
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
        struct Node* nextNode = NULL;
        int minCost = 1000; // Initialize to a large value

        while (neighbor != NULL) {
            if (!visited[neighbor->label - 'A'] && neighbor->cost < minCost) {
                minCost = neighbor->cost;
                nextNode = neighbor;
            }
            neighbor = neighbor->next;
        }

        if (nextNode != NULL) {
            visited[current - 'A'] = true;
            depth++;
            pathCost += minCost;
            push(&stack, nextNode->label);
        } else {
            depth--;
            pathCost -= adjacencyList[path[depth] - 'A']->cost;
        }
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

    hillClimbing(adjacencyList, source, goal);

    return 0;
}
