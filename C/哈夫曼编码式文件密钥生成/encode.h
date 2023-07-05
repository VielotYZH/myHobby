#include"public.h"
#include<stdlib.h>

struct huffman_node {
	int weight;
	struct huffman_node* left_subtree;
	struct huffman_node* right_subtree;
	struct huffman_node* parent;
};
#define HUFFMAN_NODE_SIZE sizeof(struct huffman_node)

struct huffman_node* leave[ALPHABET_SIZE];

void quick_sort(struct huffman_node* array[], int begin, int end);
void generate_haffman_tree(int weights[], struct huffman_node* root_node[]);
void encode(char encoding_table[][60]);