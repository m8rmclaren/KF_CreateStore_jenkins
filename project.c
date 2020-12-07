/*
* FILE: project.c
* PROJECT: Huffman Tree
* PURPOSE: Create a Huffman tree using a little end up heap inside an array with i=2j property
* AUTHOR: Hayden Roszell
* HISTORY: Version 8.0 11/30/2020
*
* Copyright © 2020 Hayden Roszell. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct hNode {
    struct hNode *leftNode, *rightNode;
    char character;
    int frequency;
} HTN;

void insertHeap(HTN **heap, HTN *newNode);
HTN *removeHeap(HTN **heap);
HTN *getNewNode(char character, int frequency);

void buildHuffman(HTN **heap);
void inOrderTraverse( HTN *ptrToTheRootOfSomeBinaryTree );
void preOrderTraverse( HTN *ptrToTheRootOfSomeBinaryTree );

void printHuffmanCodes(HTN *head, int codes[], int arrLen);
void writeToFile(char character, int *array, int arrLen);

void listArrayValues(int *array, int arrayLength);

#define MAX_HEAP 128
int main() {
    HTN *heap[MAX_HEAP];
    heap[0] = getNewNode('*', 0);
    char choice;
    int frequency;

    do {
        printf("Enter a character (enter a '$' to quit entering characters): ");
        scanf("%s", &choice);
        if (choice != '$') {
            printf("  Enter '%c's frequency: ", choice);
            scanf("%d", &frequency);
            HTN *node = getNewNode(choice, frequency);
            insertHeap(heap, node);
        } else if (choice == '$') {
            fputc('\n', stdout);
        } else {
            printf("What was that input lmao\n");
        }

    } while (choice != '$');

    buildHuffman(heap);

    printf("\nThe nodes of the Huffman tree in In-order are:\n");
    inOrderTraverse(heap[1]);
    printf("\n and in Pre-order are:\n");
    preOrderTraverse(heap[1]);

    int codes[MAX_HEAP];
    int arrLen = 0;
    printf("The Huffman codes (instead of the manual thing i just modified the traverse to make codes lol):\n");
    printHuffmanCodes(heap[1], codes, arrLen);

    return 0;
}

void printHuffmanCodes(HTN *head, int codes[], int arrLen) {
    if (head != NULL )
    {
        codes[arrLen] = 0;
        printHuffmanCodes(head->leftNode, codes, arrLen+1);
        codes[arrLen] = 1;
        printHuffmanCodes(head->rightNode, codes, arrLen+1);
        if (head->character != '-') {
            printf("%4c ", head->character);
            listArrayValues(codes, arrLen);
        }
    }
}

void buildHuffman(HTN **heap) {
    while (heap[0]->frequency != 1) {
        HTN *newTree = getNewNode('-', 0);
        newTree->leftNode = removeHeap(heap);
        newTree->rightNode = removeHeap(heap);
        newTree->frequency = newTree->leftNode->frequency + newTree->rightNode->frequency;
        insertHeap(heap, newTree);
    }
}

void inOrderTraverse( HTN *ptrToTheRootOfSomeBinaryTree )
{
    if (ptrToTheRootOfSomeBinaryTree != NULL )
    {
        inOrderTraverse(ptrToTheRootOfSomeBinaryTree->leftNode);
        fflush(stdout);
        printf("%4c %12d\n", ptrToTheRootOfSomeBinaryTree->character, ptrToTheRootOfSomeBinaryTree->frequency);
        inOrderTraverse(ptrToTheRootOfSomeBinaryTree->rightNode);
    }
}

void preOrderTraverse( HTN *ptrToTheRootOfSomeBinaryTree )
{
    if (ptrToTheRootOfSomeBinaryTree != NULL )
    {
        fflush(stdout);
        printf("%4c %12d\n", ptrToTheRootOfSomeBinaryTree->character, ptrToTheRootOfSomeBinaryTree->frequency);
        preOrderTraverse(ptrToTheRootOfSomeBinaryTree->leftNode);
        preOrderTraverse(ptrToTheRootOfSomeBinaryTree->rightNode);
    }
}

HTN *getNewNode(char character, int frequency) {
    HTN *new = (HTN*)malloc(sizeof(HTN));
    new->leftNode = new->rightNode = NULL;
    new->character = character;
    new->frequency = frequency;

    return new;
}

void listArrayValues(int *array, int arrayLength) {
    int i = 0;
    for (i = 0; i < arrayLength; ++i) {
        printf("%d", array[i]);
    } fputc('\n', stdout);
}

void insertHeap(HTN **heap, HTN *newNode) {
    HTN *node = newNode;
    ++heap[0]->frequency;
    int count = heap[0]->frequency;
    int search = 1;
    heap[count] = node;

    while (search == 1) {
        if (count % 2 == 0) {
            if (heap[count/2]->frequency > node->frequency && count/2 > 0) {
                HTN *temp = heap[count / 2];
                heap[count / 2] = heap[count];
                heap[count] = temp;
                count /= 2;
            } else
                search = 0;
        } else {
            if (heap[(count-1)/2]->frequency > node->frequency && (count-1)/2 > 0) {
                HTN *temp = heap[(count - 1) / 2];
                heap[(count-1)/2] = heap[count];
                heap[count] = temp;
                count -= 1;
                count /= 2;
            } else
                search = 0;
        }
    }
}

HTN *removeHeap(HTN **heap) {
    int index = 1;
    int search = 1;

    if (heap[0]->frequency < 1) {
        printf("Heap is empty!\n");
        return heap[0];
    }

    HTN *returnVal = heap[1];

    heap[1] = heap[heap[0]->frequency];
    --heap[0]->frequency;

    while (search == 1 && heap[0]->frequency > 0) {
        if (index*2 > heap[0]->frequency)
            search = 0;
        else if (heap[index * 2]->frequency < heap[(index * 2) + 1]->frequency && heap[index * 2]->frequency < heap[index]->frequency && index*2 < heap[0]->frequency) {
            HTN *temp = heap[index * 2];
            heap[index * 2] = heap[index];
            heap[index] = temp;
            index *= 2;

        } else if (heap[index * 2]->frequency > heap[(index * 2) + 1]->frequency && heap[(index * 2) + 1]->frequency < heap[index]->frequency && index*2 < heap[0]->frequency) {
            HTN *temp = heap[(index * 2) + 1];
            heap[(index * 2) + 1] = heap[index];
            heap[index] = temp;
            index *= 2;
            index += 1;
        } else {
            heap[heap[0]->frequency+1] = NULL;
            return returnVal;
        }
    }
    heap[heap[0]->frequency+1] = NULL;
    return returnVal;
}


// one time a teacher told me i had to leave a comment so here's mine lmao