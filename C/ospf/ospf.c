#include<stdio.h>
#include<stdlib.h>

#define LTCN_SIZE sizeof(LTCN)
#define NODE_SIZE sizeof(Node)

typedef struct LTCN{
    int destination;
    int cost;
    int nextHop;
    struct LTCN* next;
}LTCN;// LSP,confirmed,tentative node
typedef struct Node{
    int nodeID;
    LTCN* LSP;
    LTCN* confirmed;
    LTCN* tentative;
    struct Node* next;
}Node;

Node* initNode(){
    Node* p = (Node*)malloc(NODE_SIZE);
    p->next=NULL;
    p->LSP = (LTCN*)malloc(LTCN_SIZE);
    p->LSP->next = NULL;
    p->confirmed = (LTCN*)malloc(LTCN_SIZE);
    p->confirmed->next = NULL;
    p->tentative = (LTCN*)malloc(LTCN_SIZE);
    p->tentative->next = NULL;
    return p;
}

Node* initNodeTable(FILE* f){
    Node* head = (Node*)malloc(NODE_SIZE);
    head->next=NULL;
    int current = -1;
    int ID,neighbour,cost;
    while(!feof(f)){
        fscanf(f,"%d %d %d",&ID,&neighbour,&cost);
        if(current != ID){
            current = ID;
            Node* p = initNode();
            p->nodeID = ID;
            p->LSP->destination = ID;//this ID show masterID
            LTCN* q = (LTCN*)malloc(LTCN_SIZE);
            q->destination = neighbour;
            q->cost = cost;
            q->nextHop = neighbour;
            q->next = p->LSP->next;
            p->LSP->next=q;
            p->next = head->next;
            head->next = p;
        }else{
            LTCN* q = (LTCN*)malloc(LTCN_SIZE);
            q->destination = neighbour;
            q->cost = cost;
            q->nextHop = neighbour;
            q->next = head->next->LSP->next;
            head->next->LSP->next = q;
        }
    }
    return head;
}

Node* determineNext(Node* nodeTable, int ID){
    Node* p = nodeTable->next;
    while(p!=NULL){
        if(p->nodeID == ID){
            return p;
        }
        p=p->next;
    }
    printf("determineNext fail\n");
    exit(1);
}

LTCN* ifExist(LTCN* TCL, int ID){
    LTCN* p =TCL->next;
    while(p != NULL){
        if(p->destination==ID){
            return p;
        }
        p=p->next;
    }
   return NULL;
}

int determineNextHop(Node* node, int ID){
    Node* p = node->next;
    while(p!=NULL){
        LTCN* q = p->LSP->next;
        while(q!=NULL){
            if(q->destination==ID){
                return ID;
            }
            q=q->next;
        }
        q = p->confirmed->next;
        while(q!=NULL){
            if(q->destination==ID){
                return ID;
            }
            q=q->next;
        }
    }
    printf("determineNextHop fail\n");
    exit(1);
}

void changeTC(Node* myNode, LTCN* nextLSP){
    LTCN* p = nextLSP->next;
    while(p != NULL){
        LTCN* temp = ifExist(myNode->tentative,p->destination);
        if(!(ifExist(myNode->confirmed,p->destination)||temp)){
            LTCN* q = (LTCN*)malloc(LTCN_SIZE);
            q->destination = p->destination;
            q->cost = p->cost;
            q->nextHop = determineNextHop(myNode,nextLSP->destination);
            q->next = myNode->tentative->next;
            myNode->tentative->next = q;
        }else if(temp){
            int newCost = ifExist(myNode->confirmed,nextLSP->destination)+p->cost;
            if(newCost<temp->cost){
                temp->cost = newCost;
                temp->nextHop = determineNextHop(myNode,nextLSP->destination);
            }
        }
        p=p->next;
    }
}

void moveRecord(LTCN* tentative, LTCN* confirmed){
    if(tentative->next==NULL){
        return;
    }
    LTCN* p = tentative->next;
    int minCost = p->cost;
    while(p!=NULL){
        if(p->cost<minCost){
            minCost=p->cost;
        }
        p=p->next;
    }
    p = tentative;
    while(p!=NULL){
        if(p->next->cost==minCost){
            break;
        }
        p=p->next;
    }
    LTCN* q = p->next;
    p->next = q->next;
    q->next = confirmed->next;
    confirmed->next = q;
}

void createRoutingTable(Node* nodeTable, Node* myNode){
    LTCN* p = (LTCN*)malloc(LTCN_SIZE);
    p->destination = myNode->nodeID;
    p->cost = 0;
    p->nextHop = -1;//-1 show null
    p->next = myNode->confirmed->next;
    myNode->confirmed->next = p;
    do{
        moveRecord(myNode->tentative,myNode->confirmed);
        Node* next = determineNext(nodeTable,myNode->confirmed->next->destination);
        changeTC(myNode,next->LSP);
    }while(myNode->tentative->next!=NULL);
}

void main(){
    FILE* f = fopen("test.txt","r");
    Node* nodeTable = initNodeTable(f);
    fclose(f);
    Node* p = nodeTable->next;
    while(p!=NULL){
        createRoutingTable(nodeTable,p);
        p=p->next;
    }
}