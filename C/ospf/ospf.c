/*
Use numbers instead of letters as node numbers;
The meaning of each line in the entered text:
Local node neighbor cost
*/

#include <stdio.h>
#include <stdlib.h>
#include<assert.h>

#define NODE_SIZE sizeof(Node)
#define LSP_SIZE sizeof(LSP)
#define TC_SIZE sizeof(TC)

typedef struct LSP
{
    int neighbor;
    int cost;
    struct LSP *next;
} LSP; // Take the table in the LSP as itself
typedef struct TC
{
    int destination;
    int cost;
    int nestHop;
    struct TC *next;
} TC;
typedef struct Node
{
    int number;
    LSP *LSPList;
    TC *tentative;
    TC *confirmed;
    struct Node *next;
} Node;

Node *initNodeList(FILE *f)
{
    int number, neighbor, cost;
    Node *list = (Node *)malloc(NODE_SIZE);
    list->LSPList = NULL;
    list->tentative = NULL;
    list->confirmed = NULL;
    list->next = NULL;
    Node *p = list;
    int current = 0;
    LSP *r = NULL; // Always refers to the last node in the LSP table
    while (!feof(f))
    {
        fscanf(f, "%d %d %d", &number, &neighbor, &cost);
        if (current != number)
        {
            current = number;
            Node *q = (Node *)malloc(NODE_SIZE);
            q->number = number;

            q->LSPList = (LSP *)malloc(LSP_SIZE);
            q->LSPList->neighbor = number; // The node number is placed in the head node
            q->LSPList->next = (LSP *)malloc(LSP_SIZE);
            r = q->LSPList->next;
            r->neighbor = neighbor;
            r->cost = cost;
            r->next = NULL;

            q->tentative = (TC *)malloc(TC_SIZE);
            q->tentative->next = NULL;

            q->confirmed = (TC *)malloc(TC_SIZE);
            q->confirmed->next = NULL;

            q->next = NULL;
            p->next = q;
            p = q;
        }
        else
        {
            r->next = (LSP *)malloc(LSP_SIZE);
            r->next->neighbor = neighbor;
            r->next->cost = cost;
            r->next->next = NULL;
            r = r->next;
        }
    }
    return list;
}

void addNodeToTCList(TC *list, int destination, int cost, int nextHop)
{
    TC *p = list;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = (TC *)malloc(TC_SIZE);
    p->next->destination = destination;
    p->next->cost = cost;
    p->next->nestHop = nextHop;
    p->next->next = NULL;
}

LSP *locateLSPNode(Node *list, int number)
{
    Node *p = list->next;
    while (p != NULL)
    {
        if (p->number == number)
        {
            return p->LSPList;
        }
        p = p->next;
    }
    printf("locateLSPNode error\n");
    exit(1);
}

TC *ifExist(int number, TC *list)
{
    TC *p = list->next;
    while (p != NULL)
    {
        if (p->destination == number)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Pick the least cost record from the tentative and move it into confirmed
void moveTCNode(TC *confirmed, TC *tentative)
{
    TC *q = tentative;
    TC *minCost = q->next;
    q = q->next->next;
    while (q != NULL)
    {
        if (q->cost < minCost->cost)
        {
            minCost = q;
        }
        q = q->next;
    }
    addNodeToTCList(confirmed, minCost->destination, minCost->cost, minCost->nestHop);
    q = tentative;
    while (q->next != minCost)
    {
        q = q->next;
    }
    q->next = minCost->next;
    free(minCost);
}

void createRoutineTable(Node *list, Node* q)
{
    TC *cp = q->confirmed; // Always refers to the last node in the confirmed table
    addNodeToTCList(q->confirmed, q->number, 0, 0);
    cp = cp->next;
    do
    {
        LSP *r = locateLSPNode(list, cp->destination)->next;
        while (r != NULL)
        {
            int newCost = cp->cost + r->cost;
            TC *s = ifExist(r->neighbor, q->tentative);
            if (!(ifExist(r->neighbor, q->confirmed) || s))
            {
                addNodeToTCList(q->tentative, r->neighbor, newCost, cp->nestHop == 0 ? r->neighbor : cp->nestHop);
            }
            else if (s && newCost < s->cost)
            {
                s->cost = newCost;
                s->nestHop = cp->nestHop == 0 ? r->neighbor : cp->nestHop;
            }
            r = r->next;
        }
        if (q->tentative->next == NULL)
        {
            break;
        }
        else
        {
            moveTCNode(q->confirmed, q->tentative);
            cp = cp->next;
        }
    } while (1);
}

void main()
{
    FILE *f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("file is not exist\n");
        exit(1);
    }
    Node *list = initNodeList(f);
    Node *p = list->next;
    while (p != NULL)
    {
        createRoutineTable(list, p);
        TC *q = p->confirmed->next;
        printf("-------confirmed of node %c-------\n", p->number + 64);
        while (q != NULL)
        {
            printf("(%c,%d,", q->destination + 64, q->cost);
            if (q->nestHop == 0)
            {
                printf("%c)\n", '-');
            }
            else
            {
                printf("%c)\n", q->nestHop + 64);
            }
            q = q->next;
        }
        p = p->next;
    }
}