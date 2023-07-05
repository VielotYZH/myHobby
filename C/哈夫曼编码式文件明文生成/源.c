#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_BUFFER_SIZE 1024
#define ALPHABET 52
#define LEN sizeof(struct huffman_node)

int buffer_size = 0;
int current_read_position = 0;
static char buffer[MAX_BUFFER_SIZE];
struct huffman_node {
	char value;
	struct huffman_node* left_subtree;
	struct huffman_node* right_subtree;
};

char scan_file(FILE* fp);
struct huffman_node* generate_huffman_tree(FILE* fp);
void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]);
void generate_ciphertext_content(FILE* plaintext, FILE* ciphertext, struct huffman_node* root_node);

void main(int argc, char* argv[]) {
	//��鴫���ļ�����
	if (argc != 3) {
		fprintf(stderr, "�������֧�ֶ�һ���ļ�����������\n");
		fprintf(stderr, "�����Ƿ������һ����������ļ�\n");
		fprintf(stderr, "�����Ƿ��������Կ�ļ�\n");
		exit(0);
	}
	//��ȡ��Կ
	char key_name[120];
	strcpy(key_name, argv[2]);
	if (!strstr(key_name, "_key.txt")) {
		fprintf(stderr, "δ������Կ�ļ�\n");
		fprintf(stderr, "�����Ƿ�δ�����Կ�ļ�\n");
		fprintf(stderr, "������Կ�ļ�����Ϊ��һ���ļ�����\n");
		exit(0);
	}
	FILE* key = fopen(key_name, "r");
	if (key == NULL) {
		fprintf(stderr, "File %s not found\n", key_name);
		exit(1);
	}
	//���ɹ�������
	struct huffman_node* root_node = generate_huffman_tree(key);
	//��ȡ�����ļ�
	char ciphertext_name[120];
	strcpy(ciphertext_name, argv[1]);
	if (!strstr(ciphertext_name, "_ciphertext.txt")) {
		fprintf(stderr, "�������֧�ֶԶ�Ӧ�����ļ�����������\n");
		fprintf(stderr, "����ĵ�һ���ļ����ǺϷ��İ��ĸ�ʽ\n");
		exit(0);
	}
	FILE* ciphertext = fopen(ciphertext_name, "r");
	if (ciphertext == NULL) {
		fprintf(stderr, "File %s not found\n", ciphertext_name);
		exit(1);
	}
	//�ڰ�����Ŀ¼�´��������ļ�
	char plaintext_name[120];
	get_file_directory(ciphertext_name, 120, plaintext_name);
	strcat(plaintext_name, "_plaintext.txt");
	FILE* plaintext = fopen(plaintext_name, "w");
	if (plaintext == NULL) {
		fprintf(stderr, "�����ļ�����ʧ��\n");
		exit(0);
	}
	//���ݹ����������ִ�ӡ�����ļ���
	generate_ciphertext_content(ciphertext, plaintext, root_node);
	if (key != NULL) {
		fclose(key);
	}
	if (plaintext != NULL) {
		fclose(plaintext);
	}
	if (ciphertext != NULL) {
		fclose(ciphertext);
	}
	free(key);
	free(plaintext);
	free(ciphertext);
}

//ɨ���ļ�
char scan_file(FILE* fp) {
	if (current_read_position >= buffer_size) {
		if (fgets(buffer, MAX_BUFFER_SIZE, fp)) {
			buffer_size = strlen(buffer);
			current_read_position = 0;
			return buffer[current_read_position++];
		}
		else {
			return -1;
		}
	}
	else {
		return buffer[current_read_position++];
	}
}

//���ɹ�������
struct huffman_node* generate_huffman_tree(FILE* fp) {
	char token;
	char should_be = 'a';
	int i = -1;
	int j = 0;
	struct huffman_node* root_node = (struct huffman_node*)malloc(LEN);
	root_node->left_subtree = root_node->right_subtree = NULL;
	root_node->value = '0';
	struct huffman_node* helper = root_node;
	do {
		if (i >= ALPHABET || j >= 30) {
			fprintf(stderr, "��Կ��\n");
			exit(0);
		}
		token = scan_file(fp);
		if (current_read_position == 1) {
			if (token != should_be) {
				fprintf(stderr, "��Կ��\n");
				exit(0);
			}
			if ((should_be >= 'a' && should_be <= 'z') || (should_be >= 'A' && should_be <= 'Z')) {
				should_be++;
				if (should_be > 'z') {
					should_be -= 58;
				}
			}
			i++;
			j = 0;
			helper = root_node;//�Ӹ���㿪ʼ����
		}
		else if (current_read_position == 2) {
			if (token != ':') {
				fprintf(stderr, "��Կ��\n");
				exit(0);
			}
		}
		else if (current_read_position >= 3 && token != -1) {
			if (token != '\n') {
				if (token == '0') {
					//���������û��
					if (helper->left_subtree == NULL) {
						struct huffman_node* temp = (struct huffman_node*)malloc(LEN);
						temp->left_subtree = temp->right_subtree = NULL;
						//�����Ҷ�ӽ��
						if (current_read_position == buffer_size - 1) {
							if (should_be == 'A') {
								temp->value = 'z';
							}
							else {
								temp->value = should_be - 1;
							}
						}
						//�������Ҷ�ӽ��
						else {
							temp->value = '0';
						}
						helper->left_subtree = temp;
					}
					helper = helper->left_subtree;
				}
				else if (token == '1') {
					//���������û��
					if (helper->right_subtree == NULL) {
						struct huffman_node* temp = (struct huffman_node*)malloc(LEN);
						temp->left_subtree = temp->right_subtree = NULL;
						//�����Ҷ�ӽ��
						if (current_read_position == buffer_size - 1) {
							if (should_be == 'A') {
								temp->value = 'z';
							}
							else {
								temp->value = should_be - 1;
							}
						}
						//�������Ҷ�ӽ��
						else {
							temp->value = '0';
						}
						helper->right_subtree = temp;
					}
					helper = helper->right_subtree;
				}
				j++;
			}
		}
	} while (token != -1);
	return root_node;
}

//��ȡ�Ѵ��ļ�������Ŀ¼����������Ϊ�Ѵ��ļ������飬���鳤�ȣ�Ҫ�����ļ������飩
void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]) {
	char destination[] = "_ciphertext.";
	int max = 0;
	for (int i = 0, j = 0; i < source_file_length; i++) {
		if (source_file_name[i] == destination[j]) {
			//��'.'����һ����
			if (j == 11) {
				max = i - 11;
				break;
			}
			j++;
		}
		else {
			j = 0;
		}
	}
	int i = 0;
	for (; i < max; i++) {
		destination_file_name[i] = source_file_name[i];
	}
	destination_file_name[i] = '\0';
}

//ɨ�������ļ�
//���ݹ����������ִ�ӡ�����ļ���
void generate_ciphertext_content(FILE* ciphertext, FILE* plaintext, struct huffman_node* root_node) {
	char token;
	char content_to_write[2];
	struct huffman_node* p = root_node;
	do {
		token = scan_file(ciphertext);
		if (token == '0' || token == '1') {
			if (token == '0') {
				p = p->left_subtree;
			}
			else {
				p = p->right_subtree;
			}
			//�ж��ǲ���Ҷ�ӽ��
			if (p->left_subtree == NULL && p->right_subtree == NULL) {
				content_to_write[0] = p->value;
				p = root_node;//�ҵ�һ��Ҷ�ӽ�㣬�´δӸ���㿪ʼѰ��
			}
			else {
				continue;
			}
			content_to_write[1] = '\0';
			fputs(content_to_write, plaintext);
		}
		else if (token >= '2' && token <= ';') {
			content_to_write[0] = token - 2;
			content_to_write[1] = '\0';
			fputs(content_to_write, plaintext);
		}
		else {
			switch (token) {
			case 'C':
				content_to_write[0] = ' ';
				break;
			case 'D':
				content_to_write[0] = '.';
				break;
			case 'E':
				content_to_write[0] = ',';
				break;
			case 'F':
				content_to_write[0] = ':';
				break;
			case 'G':
				content_to_write[0] = ';';
				break;
			case 'H':
				content_to_write[0] = '-';
				break;
			case 'I':
				content_to_write[0] = '!';
				break;
			case 'J':
				content_to_write[0] = '?';
				break;
			case 'K':
				content_to_write[0] = '"';
				break;
			case 'L':
				content_to_write[0] = '\'';
				break;
			case 'M':
				content_to_write[0] = '(';
				break;
			case 'N':
				content_to_write[0] = ')';
				break;
			case 'O':
				content_to_write[0] = '[';
				break;
			case 'P':
				content_to_write[0] = ']';
				break;
			case 'Q':
				content_to_write[0] = '{';
				break;
			case 'R':
				content_to_write[0] = '}';
				break;
			case 'S':
				content_to_write[0] = '<';
				break;
			case 'T':
				content_to_write[0] = '>';
				break;
			case '\n':
				fprintf(plaintext, "\n");
				continue;
			default:
				continue;
			}
			content_to_write[1] = '\0';
			fputs(content_to_write, plaintext);
		}
	} while (token != -1);
}