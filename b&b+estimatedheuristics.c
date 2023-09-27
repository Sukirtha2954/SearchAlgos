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

struct EstimatedCosts {
    char label;
    int cost;
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

// A simple heuristic function to estimate the remaining cost
int estimateRemainingCost(char current, char goal) {
    // Replace this with a more sophisticated heuristic if available
    return 0;
}

// Sort nodes based on estimated costs
void sortNodes(struct EstimatedCosts* estimatedCosts, int size) {
    // Use a simple insertion sort for demonstration
    int i, j;
    struct EstimatedCosts key;
    for (i = 1; i < size; i++) {
        key = estimatedCosts[i];
        j = i - 1;

        while (j >= 0 && estimatedCosts[j].cost > key.cost) {
            estimatedCosts[j + 1] = estimatedCosts[j];
            j = j - 1;
        }
        estimatedCosts[j + 1] = key;
    }
}

void branchAndBound(struct Node* adjacencyList[], char source, char goal, char* oraclePath, int* oracleCosts) {
    struct Stack* stack = NULL;
    bool visited[VERTICES] = {false};
    int depth = 0;
    int costSoFar = 0;
    char path[VERTICES];

    struct EstimatedCosts estimatedCosts[VERTICES];
    int estimatedCostsSize = 0;

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
                            int remainingCost = estimateRemainingCost(next, goal);
                            estimatedCosts[estimatedCostsSize].label = next;
                            estimatedCosts[estimatedCostsSize].cost = costSoFar + neighbor->cost + remainingCost;
                            estimatedCostsSize++;
                        }
                        neighbor = neighbor->next;
                    }

                    // Sort nodes based on estimated costs
                    sortNodes(estimatedCosts, estimatedCostsSize);

                    // Push nodes in sorted order onto the stack
                    for (int i = 0; i < estimatedCostsSize; i++) {
                        push(&stack, estimatedCosts[i].label, costSoFar + estimatedCosts[i].cost - remainingCost);
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

    branchAndBound(adjacencyList, source, goal, oraclePath, oracleCosts);

    return 0;
}
