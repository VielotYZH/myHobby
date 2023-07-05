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
	//检查传入文件数量
	if (argc != 3) {
		fprintf(stderr, "本程序仅支持对一个文件的明文生成\n");
		fprintf(stderr, "请检查是否仅传入一个待处理的文件\n");
		fprintf(stderr, "或者是否添加了密钥文件\n");
		exit(0);
	}
	//获取密钥
	char key_name[120];
	strcpy(key_name, argv[2]);
	if (!strstr(key_name, "_key.txt")) {
		fprintf(stderr, "未发现密钥文件\n");
		fprintf(stderr, "请检查是否未添加密钥文件\n");
		fprintf(stderr, "或者密钥文件被作为第一个文件传入\n");
		exit(0);
	}
	FILE* key = fopen(key_name, "r");
	if (key == NULL) {
		fprintf(stderr, "File %s not found\n", key_name);
		exit(1);
	}
	//生成哈夫曼树
	struct huffman_node* root_node = generate_huffman_tree(key);
	//获取暗文文件
	char ciphertext_name[120];
	strcpy(ciphertext_name, argv[1]);
	if (!strstr(ciphertext_name, "_ciphertext.txt")) {
		fprintf(stderr, "本程序仅支持对对应暗文文件的明文生成\n");
		fprintf(stderr, "传入的第一个文件并非合法的暗文格式\n");
		exit(0);
	}
	FILE* ciphertext = fopen(ciphertext_name, "r");
	if (ciphertext == NULL) {
		fprintf(stderr, "File %s not found\n", ciphertext_name);
		exit(1);
	}
	//在暗文文目录下创建明文文件
	char plaintext_name[120];
	get_file_directory(ciphertext_name, 120, plaintext_name);
	strcat(plaintext_name, "_plaintext.txt");
	FILE* plaintext = fopen(plaintext_name, "w");
	if (plaintext == NULL) {
		fprintf(stderr, "暗文文件创建失败\n");
		exit(0);
	}
	//根据哈夫曼树逐字打印至新文件中
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

//扫描文件
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

//生成哈夫曼树
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
			fprintf(stderr, "密钥损坏\n");
			exit(0);
		}
		token = scan_file(fp);
		if (current_read_position == 1) {
			if (token != should_be) {
				fprintf(stderr, "密钥损坏\n");
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
			helper = root_node;//从根结点开始构造
		}
		else if (current_read_position == 2) {
			if (token != ':') {
				fprintf(stderr, "密钥损坏\n");
				exit(0);
			}
		}
		else if (current_read_position >= 3 && token != -1) {
			if (token != '\n') {
				if (token == '0') {
					//如果左子树没有
					if (helper->left_subtree == NULL) {
						struct huffman_node* temp = (struct huffman_node*)malloc(LEN);
						temp->left_subtree = temp->right_subtree = NULL;
						//如果是叶子结点
						if (current_read_position == buffer_size - 1) {
							if (should_be == 'A') {
								temp->value = 'z';
							}
							else {
								temp->value = should_be - 1;
							}
						}
						//如果不是叶子结点
						else {
							temp->value = '0';
						}
						helper->left_subtree = temp;
					}
					helper = helper->left_subtree;
				}
				else if (token == '1') {
					//如果右子树没有
					if (helper->right_subtree == NULL) {
						struct huffman_node* temp = (struct huffman_node*)malloc(LEN);
						temp->left_subtree = temp->right_subtree = NULL;
						//如果是叶子结点
						if (current_read_position == buffer_size - 1) {
							if (should_be == 'A') {
								temp->value = 'z';
							}
							else {
								temp->value = should_be - 1;
							}
						}
						//如果不是叶子结点
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

//获取已打开文件的所在目录（参数依次为已打开文件名数组，数组长度，要创建文件名数组）
void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]) {
	char destination[] = "_ciphertext.";
	int max = 0;
	for (int i = 0, j = 0; i < source_file_length; i++) {
		if (source_file_name[i] == destination[j]) {
			//到'.'都是一样的
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

//扫描明文文件
//根据哈夫曼树逐字打印至新文件中
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
			//判断是不是叶子结点
			if (p->left_subtree == NULL && p->right_subtree == NULL) {
				content_to_write[0] = p->value;
				p = root_node;//找到一个叶子结点，下次从根结点开始寻找
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