/*
* Course : CSE-3318-004-Algorithms and Data Structures
* Assignment:Lab-4
* Author: Jayanth Reddy Gaddam
* ID : 1002123569
* Date: 11/28/2023
* Omega Commands: gcc -std=c99 -o t1 lab4.c
                  ./t1 a.out
* Assignment Description:
*This C program is structured to demonstrate a FIFO queue system utilizing two LIFO stacks. 
*Its primary focus lies in efficiently managing the enqueue and dequeue operations for messages while concurrently computing real-time statistics like average, minimum, and maximum message lengths.
*By processing commands from standard input, it executes various operations and generates corresponding outputs. 
*The '0' command exits the program and displays the count of recycled nodes available for both the inStack and outStack. 
*This implementation underscores the strategic use of stacks, queues, and recycling lists, ensuring optimal handling and efficiency in managing message queue operations.
*/
#include <stdio.h>
#include <stdlib.h>
#include<limits.h>

// Structure for a node in the stack
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structure for the stack
typedef struct Stack {
    Node* top;
    Node* recycledNodes;
} Stack;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to check if the stack is empty
int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

// Function to push an element onto the stack
void push(Stack* stack, int data) {
    Node* newNode;
    if (stack->recycledNodes != NULL) {
        newNode = stack->recycledNodes;
        stack->recycledNodes = newNode->next;
        newNode->data = data;
        newNode->next = stack->top;
        stack->top = newNode;
    } else {
        newNode = createNode(data);
        newNode->next = stack->top;
        stack->top = newNode;
    }
}

// Function to pop an element from the stack
int pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Error: Stack is empty!\n");
        exit(1);
    }
    Node* temp = stack->top;
    int data = temp->data;
    stack->top = temp->next;
    temp->next = stack->recycledNodes;
    stack->recycledNodes = temp;
    return data;
}


// Function to get the top element of the stack without removing it
int peek(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Error: Stack is empty!\n");
        exit(1);
    }
    return stack->top->data;
}

// Function to initialize the queue
void initializeQueue(Stack* inStack, Stack* outStack) {
    inStack->top = NULL;
    inStack->recycledNodes = NULL;
    outStack->top = NULL;
    outStack->recycledNodes = NULL;
}
// Function to enqueue a message
void enqueue(Stack* inStack, int length) {
    push(inStack, length);
    printf("Enqueued %d\n", length);
}

// Function to dequeue a message
int dequeue(Stack* inStack, Stack* outStack) {
    if (isEmpty(outStack)) {
        while (!isEmpty(inStack)) {
            push(outStack, pop(inStack));
        }
    }
    if (isEmpty(outStack)) {
        printf("Error: Queue is empty!\n");
        exit(1);
    }
    return pop(outStack);
}

// Function to compute and print the average length of a message
void computeAverage(Stack* inStack, Stack* outStack) {
    if (isEmpty(inStack) && isEmpty(outStack)) {
        printf("Can't compute average for an empty queue\n");
        return;
    }
    
    int sum = 0;
    int count = 0;
    
    // Compute sum and count from inStack
    Node* temp = inStack->top;
    while (temp != NULL) {
        sum += temp->data;
        count++;
        temp = temp->next;
    }
    
    // Compute sum and count from outStack
    temp = outStack->top;
    while (temp != NULL) {
        sum += temp->data;
        count++;
        temp = temp->next;
    }
    
    double average = (double)sum / count;
    
    printf("average length %.6f\n", average);
}

// Function to determine and print the minimum message length
void determineMinimum(Stack* inStack, Stack* outStack) {
    if (isEmpty(inStack) && isEmpty(outStack)) {
        printf("Can't compute minimum for an empty queue\n");
        return;
    }
    
    int minimum = INT_MAX;
    
    // Find minimum from inStack
    Node* temp = inStack->top;
    while (temp != NULL) {
        if (temp->data < minimum) {
            minimum = temp->data;
        }
        temp = temp->next;
    }
    
    // Find minimum from outStack
    temp = outStack->top;
    while (temp != NULL) {
        if (temp->data < minimum) {
            minimum = temp->data;
        }
        temp = temp->next;
    }
    
    printf("minimum length %d\n", minimum);
}

// Function to determine and print the maximum message length
void determineMaximum(Stack* inStack, Stack* outStack) {
    if (isEmpty(inStack) && isEmpty(outStack)) {
        printf("Can't compute maximum for an empty queue\n");
        return;
    }
    
    int maximum = INT_MIN;
    
    // Find maximum from inStack
    Node* temp = inStack->top;
    while (temp != NULL) {
        if (temp->data > maximum) {
            maximum = temp->data;
        }
        temp = temp->next;
    }
    
    // Find maximum from outStack
    temp = outStack->top;
    while (temp != NULL) {
        if (temp->data > maximum) {
            maximum = temp->data;
        }
        temp = temp->next;
    }
    
    printf("maximum length %d\n", maximum);
}

// Function to count the number of nodes in a stack
int countNodes(Stack* stack) {
    int count = 0;
    Node* temp = stack->top;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Function to calculate the number of available recycled nodes for the inStack and outStack
void calculateRecycledNodes(Stack* inStack, Stack* outStack) {
    int inStackNodes = countNodes(inStack);
    int outStackNodes = countNodes(outStack);
    int totalNodes = inStackNodes + outStackNodes;
    printf("%d\n", totalNodes);

    Node* temp = inStack->recycledNodes;
    int inRecycledNodes = 0;
    while (temp != NULL) {
        inRecycledNodes++;
        temp = temp->next;
    }
    printf("In Stack available nodes %d ", inRecycledNodes);

    temp = outStack->recycledNodes;
    int outRecycledNodes = 0;
    while (temp != NULL) {
        outRecycledNodes++;
        temp = temp->next;
    }
    printf("Out Stack available nodes %d\n", outRecycledNodes);
}
// Function to check if it's possible to dequeue a message
int canDequeue(Stack* inStack, Stack* outStack) {
    return (!isEmpty(inStack) || !isEmpty(outStack));
}

int main() {
    Stack inStack, outStack;
    initializeQueue(&inStack, &outStack);
    
    int command, length;
    
    while (1) {
        scanf("%d", &command);
        
        switch (command) {
            case 0: // Exit the program
                calculateRecycledNodes(&inStack, &outStack);
                return 0;
                
            case 1: // Enqueue a length value
                scanf("%d", &length);
                enqueue(&inStack, length);
                break;
                
            case 2: // Dequeue a length and print it
                if (canDequeue(&inStack, &outStack)) {
                    length = dequeue(&inStack, &outStack);
                    printf("Dequeued %d\n", length);
                } else {
                    printf("Can't dequeue from an empty queue\n");
                }
                break;

            case 3: // Compute and print the average length of a message
                if (canDequeue(&inStack, &outStack))
                    computeAverage(&inStack, &outStack);
                else
                    printf("Can't compute average for an empty queue\n");
                break;

            case 4: // Determine and print the minimum message length
                if (canDequeue(&inStack, &outStack))
                    determineMinimum(&inStack, &outStack);
                else
                    printf("Can't compute minimum for an empty queue\n");
                break;

            case 5: // Determine and print the maximum message length
                if (canDequeue(&inStack, &outStack))
                    determineMaximum(&inStack, &outStack);
                else
                    printf("Can't compute maximum for an empty queue\n");
                break;

            default:
                printf("Invalid command!\n");
                break;
        }
        
        fflush(stdin);
        
        if (command == 0)
            break;
        
        getchar(); // Read and discard newline character after each command
    }
    
    return 0;
}
