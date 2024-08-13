/*
    红黑规则：
    1.每一个节点或是红色的，或是黑色的
    2.根节点必须是黑色
    3.除根节点的父节点之外，所有NULL视为叶节点，每个叶节点是黑色的
    4.如果某节点是红色的，那么它的子节点必须是黑色的
    5.对每一个节点，从该节点到其所有后代叶节点的简单路径上，均包含数目相同的黑色节点
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NODE_SIZE sizeof(Node)

// 红黑树的颜色只有红与黑两种
typedef enum
{
    RED,
    BLACK
} Color;

typedef struct Node{
    struct Node *f_addr;
    int value; // 此处将红黑树节点的值简化为int类型
    struct Node *l_addr;
    struct Node *r_addr;
    Color color;
} Node;

/*
    C没有将枚举类型与其整数值直接转换的标准库函数
    该函数将将color的值转换为对应的常量字符
*/
const char get_color(Color color){
    switch (color)
    {
    case RED:
        return 'r';
    case BLACK:
        return 'b';
    }
}

// 逆时针旋转90°打印二叉树
void printTree(Node *root, int *level){
    if (root == NULL){
        // 若传入的树是空树，直接返回
        return;
    }
    if(root->r_addr != NULL){
        (*level)++; // 存在下一层可进入，level加1
        printTree(root->r_addr, level);
    }
    for (int i = 0; i < *level; i++){
        printf("\t");
    }
    printf("%d%c\n", root->value, get_color(root->color));
    if(root->l_addr != NULL){
        (*level)++; // 存在下一层可进入，level加1
        printTree(root->l_addr, level);
    }
    (*level)--; // 即将返回上一层，level减1
}

// 保证每次调用printTree前level都自动重置为0
void print_tree(Node *root){
    int level = 0;
    printTree(root, &level);
}

// 返回值为被添加节点的指针表示添加成功，为NULL表示已存入相同的值
Node* add_node_to_tree(Node *root, int value){
    Node *node = (Node *)malloc(NODE_SIZE);
    node->value = value;
    node->l_addr = NULL;
    node->r_addr = NULL;
    node->color = RED; // 非根节点默认颜色为红色 
    Node *p = root;
    while(true){
        if(value < p->value){
            // 要添加的值比当前节点的值小
            if(p->l_addr != NULL){
                // 当前节点左子树不为空，往下走
                p = p->l_addr;
            }else{
                p->l_addr = node;
                node->f_addr = p;
                return node;
            }
        }else if(value > p->value){
            // 要添加的值比当前节点的值大
            if(p->r_addr != NULL){
                // 当前节点右子树不为空，往下走
                p = p->r_addr;
            }else{
                p->r_addr = node;
                node->f_addr = p;
                return node;
            }
        }else{
            // 要添加的值与当前节点的值相等，不添加
            return NULL;
        }
    }
}

// 左旋
Node* turnLeft(Node *pivot){
    /*
        实现思路：
        不平衡点（以下简称支点）：父节点=其原右子节点
                                左子节点不变
                                右子节点=其原右子节点的左子节点
        支点的右子节点： 父节点=支点的父节点
                        左子节点=支点
                        右子节点不变
    */
    Node *r_addr = pivot->r_addr;
    r_addr->f_addr = pivot->f_addr;
    pivot->f_addr = r_addr;
    pivot->r_addr = r_addr->l_addr;
    r_addr->l_addr = pivot;
    if(pivot->r_addr != NULL){
        // 支点原右子节点的左子节点（即此时支点的右子节点）的父节点=支点
        pivot->r_addr->f_addr = pivot;
    }
    if (r_addr->f_addr != NULL)
    {
        // 下面判断支点是其父节点的左子节点还是右子节点
        if (r_addr->f_addr->l_addr == pivot)
        {
            r_addr->f_addr->l_addr = r_addr;
        }
        else
        {
            r_addr->f_addr->r_addr = r_addr;
        }

        return NULL; //支点不为根节点，返回NULL
    }else{
        return r_addr; // 支点为根节点，返回新的根节点
    }
}

// 左旋的封装，解决旋转过程中根节点改变的问题
void turn_left(Node **root, Node *pivot){
    Node *tmp = turnLeft(pivot);
    if (tmp != NULL)
    {
        *root = tmp;
    }
}

// 右旋
Node* turnRight(Node *pivot){
    /*
        实现思路：
        不平衡点（以下简称支点）：父节点=其原左子节点
                                左子节点=其原左子节点的右子节点
                                右子节点不变
        支点的左子节点： 父节点=支点的父节点
                        左子节点不变
                        右子节点=支点
    */
    Node *l_addr = pivot->l_addr;
    l_addr->f_addr = pivot->f_addr;
    pivot->f_addr = l_addr;
    pivot->l_addr = l_addr->r_addr;
    l_addr->r_addr = pivot;
    if(pivot->l_addr != NULL){
        // 支点原左子节点的右子节点（即此时支点的左子节点）的父节点=支点
        pivot->l_addr->f_addr = pivot;
    }
    if(l_addr->f_addr != NULL){
        // 下面判断支点是其父节点的左子节点还是右子节点
        if(l_addr->f_addr->l_addr == pivot){
            l_addr->f_addr->l_addr = l_addr;
        }else{
            l_addr->f_addr->r_addr = l_addr;
        }

        return NULL; // 支点不为根节点，返回NULL
    }else{
        return l_addr; // 支点为根节点，返回新的根节点
    }
}

// 右旋的封装，解决旋转过程中根节点改变的问题
void turn_right(Node **root, Node *pivot){
    Node *tmp = turnRight(pivot);
    if(tmp != NULL){
        *root = tmp;
    }
}

void balance(Node **root, Node *new_node){
    /*
        根节点已在主函数中单独处理
        故new_node必非根
        非根则new_node的父节点必不为空
    */
    if(new_node->f_addr->color == RED){
        /*
            new_node的父节点为红色则其必非根（红黑树根为黑色）
            非根则其父节点必不为空
            故new_node必有祖父节点
        */
        Node *fahter_node = new_node->f_addr;
        Node *grandfather_node = fahter_node->f_addr;
        Node *uncle_node = grandfather_node->l_addr == fahter_node ? grandfather_node->r_addr : grandfather_node->l_addr;
        if(uncle_node != NULL && uncle_node->color == RED){
            // 叔叔节点为红色
            fahter_node->color = BLACK; // 将父节点变为黑色
            uncle_node->color = BLACK; // 将叔叔节点变为黑色
            if(grandfather_node->f_addr != NULL){
                // 祖父节点非根
                grandfather_node->color = RED; // 将祖父节点变为红色
                balance(root, grandfather_node); // 将祖父节点设为new_node重新判断
            }
        }else{
            // 叔叔节点为黑色（NULL也视为黑色）
            if(new_node == fahter_node->r_addr){
                // new_node为父节点的右子节点
                turn_left(root, fahter_node); // 将父节点作为支点左旋
                balance(root, fahter_node); // 将父节点设为new_node重新判断
            }else{
                // new_node为父节点的左子节点
                fahter_node->color = BLACK; // 将父节点变为黑色
                grandfather_node->color = RED; // 将祖父节点变为红色
                turn_right(root, grandfather_node); // 将祖父节点作为支点右旋
            }
        }
    }
}

int main(){
    Node *root = NULL;
    FILE *f = fopen("test.txt", "r");
    int value;
    fscanf(f, "%d", &value);
    if(!feof(f)){
        // 第一个节点为根节点，单独处理
        root = (Node *)malloc(NODE_SIZE);
        root->f_addr = NULL;
        root->value = value;
        root->l_addr = NULL;
        root->r_addr = NULL;
        root->color = BLACK; // 根节点为黑色
    }
    while (!feof(f))
    {
        fscanf(f, "%d", &value);
        Node *new_node = add_node_to_tree(root, value);
        if(new_node != NULL){
            balance(&root, new_node);
        }
    }
    fclose(f);
    print_tree(root);
    return 0;
}