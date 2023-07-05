/*本程序只对文本文件中的英文字母（大小写均可）进行编码，
  中文等其他字符均不作处理，标点符号将在加密程序中处理
  为预先设定好的数字。*/
/*本程序主函数应传入一个参数，即待处理的文件。*/
  /*本程序按行读取文件，每行最多256
  个字符，请注意文件的预处理。*/

#include"public.h"
#include"count.h"
#include"encode.h"

void get_file_directory(char file_name[], int length, char key_file[]);

void main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		//获取文件
		FILE* fp;
		char file_name[120];
		strcpy(file_name, argv[1]);
		if (!strstr(file_name, ".txt")) {
			fprintf(stderr, "本程序仅支持对.txt文件的密钥生成\n");
			fprintf(stderr, "第%d个文件并非.txt格式，已跳过该文件\n", i);
			fprintf(stderr, "请检查文件输入时后缀名是否正确\n");
			break;
		}
		fp = fopen(file_name, "r");
		if (fp == NULL) {
			fprintf(stderr, "File %s not found\n", file_name);
			exit(1);
		}
		//统计每个字母的个数
		count(fp);
		struct huffman_node* root_node[ALPHABET_SIZE];
		//生成哈夫曼树
		generate_haffman_tree(alphabet, root_node);
		//按树编码
		char encoding_table[ALPHABET_SIZE][60] = { 0 };
		encode(encoding_table);
		//将结果打印到新文件中(在同一目录下）
		char key_file[120];
		get_file_directory(file_name, 120, key_file);
		strcat(key_file, "_key.txt");
		FILE* new_fp;
		new_fp = fopen(key_file, "w");
		if (new_fp == NULL) {
			fprintf(stderr, "打开文件失败\n");
			exit(0);
		}
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			char content[60];
			content[0] = i <= 25 ? 'a' + i : 'A' + i - 26;
			content[1] = ':';
			content[2] = '\0';
			strcat(content, encoding_table[i]);
			fputs(content, new_fp);
			fprintf(new_fp, "\n");
		}
		fclose(new_fp);
		fclose(fp);
		free(new_fp);
		free(fp);
	}
}

//获取已打开文件的所在目录
void get_file_directory(char file_name[],int length,char key_file[]) {
	for (int i = 0; i < length; i++) {
		//确定"."的位置
		if (file_name[i] == '.') {
			int j = 0;
			for (; j < i; j++) {
				key_file[j] = file_name[j];
			}
			key_file[j] = '\0';
			break;
		}
	}
}
