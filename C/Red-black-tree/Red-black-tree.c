#include<stdio.h>
#include<stdlib.h>

#define NODE_SIZE sizeof(Node)

typedef struct Node{
    struct Node *f_addr;
    int value;
    struct Node *l_addr;
    struct Node *r_addr;
    char color;
} Node;

void printTree(Node *n)
{
	static int level = -1; //记录是第几层次
	int i;
	if (NULL == n)
		return;

	level++;
	printTree(n->r_addr);
	level--;
	level++;
	for (i = 0; i < level; i++)
		printf("\t");
	printf("%d%c\n", n->value,n->color);
	printTree(n->l_addr);
	level--;
}


Node* new(int value){
    Node *p = (Node*)malloc(NODE_SIZE);
    p->f_addr = NULL;
    p->value = value;
    p->l_addr = NULL;
    p->r_addr = NULL;
    p->color = 'r';
    return p;
}

void addToTree(Node *tree,Node *newNode){
    Node *p = tree;
    while(1){
        if(newNode->value < p->value){
            if(p->l_addr == NULL){
                p->l_addr = newNode;
                newNode->f_addr = p;
                return;
            }else{
                p=p->l_addr;
            }
        }else if(newNode->value > p->value){
            if(p->r_addr == NULL){
                p->r_addr = newNode;
                newNode->f_addr = p;
                return;
            }else{
                p=p->r_addr;
            }
        }else{
            free(newNode);
            return;
        }
    }
}

void turnLeft(Node *fulcrum){
    Node *r = fulcrum->r_addr;
    fulcrum->r_addr = r->l_addr;
    if(r->l_addr != NULL){
        r->l_addr->f_addr = fulcrum;
    }
    r->f_addr = fulcrum->f_addr;
    if(fulcrum->f_addr->l_addr == fulcrum){
        fulcrum->f_addr->l_addr = r;
    }else{
        fulcrum->f_addr->r_addr = r;
    }
    fulcrum->f_addr = r;
    r->l_addr = fulcrum;
}

void turnRight(Node *fulcrum){
    Node *l = fulcrum->l_addr;
    fulcrum->l_addr = l->r_addr;
    if(l->r_addr != NULL){
        l->r_addr->f_addr = fulcrum;
    }
    l->f_addr = fulcrum->f_addr;
    if(fulcrum->f_addr->l_addr == fulcrum){
        fulcrum->f_addr->l_addr = l;
    }else{
        fulcrum->f_addr->r_addr = l;
    }
    fulcrum->f_addr = l;
    l->r_addr = fulcrum;
}

void balance(Node *tree, Node *currentNode){
    if(currentNode->f_addr->color == 'b'){
        return;
    }
    Node *g = currentNode->f_addr->f_addr;
    Node *u = g->l_addr == currentNode->f_addr ? g->r_addr : g->l_addr;
    if(u == NULL || u->color == 'b'){
        if(currentNode == currentNode->f_addr->r_addr){
            currentNode = currentNode->f_addr;
            turnLeft(currentNode);
            balance(tree,currentNode);
        }else{
            currentNode->f_addr->color = 'b';
            g->color = 'r';
            turnRight(g);
        }
    }else{
        currentNode->f_addr->color = 'b';
        u->color = 'b';
        if(g->f_addr != NULL){
            g->color = 'r';
            balance(tree, g);
        }
    }
}

void add(Node **tree, int value){
    Node *p = new(value);
    if(*tree == NULL){
        p->color = 'b';
        *tree = p;
        return;
    }
    addToTree(*tree, p);
    balance(*tree,p);
}

void main(){
    Node *tree = NULL;
    while(1){
        int a;
        scanf("%d",&a);
        if(a == 0){
            break;
        }else{
            add(&tree,a);
        }
    }
    printTree(tree);
}