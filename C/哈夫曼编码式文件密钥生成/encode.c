#include"encode.h"

//���ɹ�������(����root_node�����һ��Ԫ��Ϊ���ɵĹ��������ĸ���㣩
void generate_haffman_tree(int weights[], struct huffman_node* root_node[]) {
	//���ݸ�����ALPHABET_SIZE��Ȩֵ����ALPHABET_SIZE�ö�����������root_node
	//����ÿ�ö�������ֻ��һ����ȨΪweights[i]�ĸ����
	//��������������
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
			fprintf(stderr, "�ڴ治��\n");
			exit(0);
		}
	}
	//�ظ����졢ɾ����ֱ��Fֻ��һ����Ϊֹ
	int tree_number = ALPHABET_SIZE;
	int first_tree = 0;
	while (tree_number > 1) {
		//��F��ѡȡ���ø�����Ȩֵ��С������Ϊ������������һ���µĶ�����
		//���µĶ������ĸ�����ȨֵΪ����������������Ȩֵ֮��
		quick_sort(root_node, first_tree, ALPHABET_SIZE - 1);
		struct huffman_node* p;
		p = (struct huffman_node*)malloc(HUFFMAN_NODE_SIZE);
		if (p) {
			p->weight = root_node[first_tree]->weight + root_node[first_tree + 1]->weight;
			p->left_subtree = root_node[first_tree];
			p->right_subtree = root_node[first_tree + 1];
			p->parent = NULL;
			//��F��ɾ����������
			root_node[first_tree]->parent = root_node[first_tree + 1]->parent = p;
			root_node[first_tree] = root_node[first_tree + 1] = NULL;
			//���µõ��Ķ���������F��
			root_node[first_tree + 1] = p;
			first_tree++;
			tree_number--;
		}
		else {
			fprintf(stderr, "�ڴ治��\n");
			exit(0);
		}
	}
}

//��������root_node
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

//��������
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