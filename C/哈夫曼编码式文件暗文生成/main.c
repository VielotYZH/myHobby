/*本程序只对文本文件中的英文字母（大小写均可）、数字、
  空格以及英文式的句号、逗号、冒号、分号、短横线、叹
  号、问号、双引号、单引号、圆括号、方括号、花括号、
  尖括号进行编码（省略号为三个句号），中文等其他字符
  均会在暗文转明文时省略.*/
/*本程序主函数应传入两个参数，第一个参数为待处理的文
  件，第二个参数为密钥文件。*/
  /*本程序按行读取文件，每行最多256个字符，请注意文
  件的预处理。*/

#include"file_processing.h"

void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]);

void main(int argc, char* argv[]) {
	//检查传入文件数量
	if (argc != 3) {
		fprintf(stderr, "本程序仅支持对一个文件的暗文生成\n");
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
	//生成编码表
	generate_encoding_table(key);
	//获取明文文件
	char plaintext_name[120];
	strcpy(plaintext_name, argv[1]);
	if (!strstr(plaintext_name, ".txt")) {
		fprintf(stderr, "本程序仅支持对.txt文件的暗文生成\n");
		fprintf(stderr, "传入的第一个文件并非.txt格式\n");
		exit(0);
	}
	FILE* plaintext = fopen(plaintext_name, "r");
	if (plaintext == NULL) {
		fprintf(stderr, "File %s not found\n", plaintext_name);
		exit(1);
	}
	//在明文目录下创建暗文文件
	char ciphertext_name[120];
	get_file_directory(plaintext_name, 120, ciphertext_name);
	strcat(ciphertext_name, "_ciphertext.txt");
	FILE* ciphertext = fopen(ciphertext_name, "w");
	if (ciphertext == NULL) {
		fprintf(stderr, "暗文文件创建失败\n");
		exit(0);
	}
	//根据编码表逐字打印至新文件中
	generate_ciphertext_content(plaintext, ciphertext);
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

//获取已打开文件的所在目录
void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]) {
	for (int i = 0; i < source_file_length; i++) {
		//确定"."的位置
		if (source_file_name[i] == '.') {
			int j = 0;
			for (; j < i; j++) {
				destination_file_name[j] = source_file_name[j];
			}
			destination_file_name[j] = '\0';
			break;
		}
	}
}