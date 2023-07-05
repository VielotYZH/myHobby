#include"encode.h"

//生成哈夫曼树(最终root_node的最后一个元素为生成的哈夫曼树的根结点）
void generate_haffman_tree(int weights[], struct huffman_node* root_node[]) {
	//根据给定的ALPHABET_SIZE个权值构成ALPHABET_SIZE棵二叉树的数组root_node
	//其中每棵二叉树中只有一个带权为weights[i]的根结点
	//其左右子树均空
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		struct huffman_node* p;
		p = (struct huffman_node*)malloc(HUFFMAN_NODE_SIZE);
		if (p) {
			p->weight = weights[i];
			p->left_subtree = NULL;
			p->right_subtree = NULL;
			p->parent = NULL;
			root_node[i] = p;
			leave[i] = p;
		}
		else {
			fprintf(stderr, "内存不足\n");
			exit(0);
		}
	}
	//重复构造、删除，直到F只含一棵树为止
	int tree_number = ALPHABET_SIZE;
	int first_tree = 0;
	while (tree_number > 1) {
		//在F中选取两棵根结点的权值最小的树作为左右子树构造一棵新的二叉树
		//且新的二叉树的根结点的权值为其左、右子树根结点的权值之和
		quick_sort(root_node, first_tree, ALPHABET_SIZE - 1);
		struct huffman_node* p;
		p = (struct huffman_node*)malloc(HUFFMAN_NODE_SIZE);
		if (p) {
			p->weight = root_node[first_tree]->weight + root_node[first_tree + 1]->weight;
			p->left_subtree = root_node[first_tree];
			p->right_subtree = root_node[first_tree + 1];
			p->parent = NULL;
			//在F中删除这两棵树
			root_node[first_tree]->parent = root_node[first_tree + 1]->parent = p;
			root_node[first_tree] = root_node[first_tree + 1] = NULL;
			//将新得到的二叉树加入F中
			root_node[first_tree + 1] = p;
			first_tree++;
			tree_number--;
		}
		else {
			fprintf(stderr, "内存不足\n");
			exit(0);
		}
	}
}

//快速排序root_node
void quick_sort(struct huffman_node* array[], int begin, int end) {
	if (begin > end) {
		return;
	}
	struct huffman_node* temp = array[begin];
	int i = begin;
	int j = end;
	while (i != j) {
		while (array[j]->weight >= temp->weight && j > i) {
			j--;
		}
		while (array[i]->weight <= temp->weight && j > i) {
			i++;
		}
		if (j > i) {
			struct huffman_node* t = array[i];
			array[i] = array[j];
			array[j] = t;
		}
	}
	array[begin] = array[i];
	array[i] = temp;
	quick_sort(array, begin, i - 1);
	quick_sort(array, i + 1, end);
}

//按树编码
void encode(char encoding_table[][60]) {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		int current_encode_location = 0;
		struct huffman_node* p;
		p = leave[i];
		while (p->parent != NULL) {
			if (p->parent->left_subtree == p) {
				encoding_table[i][current_encode_location] = '0';
			}
			else {
				encoding_table[i][current_encode_location] = '1';
			}
			current_encode_location++;
			p = p->parent;
		}
		for (int j = 0, k = current_encode_location - 1; j < current_encode_location / 2; j++ , k--) {
			int temp = encoding_table[i][j];
			encoding_table[i][j] = encoding_table[i][k];
			encoding_table[i][k] = temp;
		}
	}
}