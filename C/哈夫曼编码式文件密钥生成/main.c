/*������ֻ���ı��ļ��е�Ӣ����ĸ����Сд���ɣ����б��룬
  ���ĵ������ַ����������������Ž��ڼ��ܳ����д���
  ΪԤ���趨�õ����֡�*/
/*������������Ӧ����һ������������������ļ���*/
  /*�������ж�ȡ�ļ���ÿ�����256
  ���ַ�����ע���ļ���Ԥ����*/

#include"public.h"
#include"count.h"
#include"encode.h"

void get_file_directory(char file_name[], int length, char key_file[]);

void main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		//��ȡ�ļ�
		FILE* fp;
		char file_name[120];
		strcpy(file_name, argv[1]);
		if (!strstr(file_name, ".txt")) {
			fprintf(stderr, "�������֧�ֶ�.txt�ļ�����Կ����\n");
			fprintf(stderr, "��%d���ļ�����.txt��ʽ�����������ļ�\n", i);
			fprintf(stderr, "�����ļ�����ʱ��׺���Ƿ���ȷ\n");
			break;
		}
		fp = fopen(file_name, "r");
		if (fp == NULL) {
			fprintf(stderr, "File %s not found\n", file_name);
			exit(1);
		}
		//ͳ��ÿ����ĸ�ĸ���
		count(fp);
		struct huffman_node* root_node[ALPHABET_SIZE];
		//���ɹ�������
		generate_haffman_tree(alphabet, root_node);
		//��������
		char encoding_table[ALPHABET_SIZE][60] = { 0 };
		encode(encoding_table);
		//�������ӡ�����ļ���(��ͬһĿ¼�£�
		char key_file[120];
		get_file_directory(file_name, 120, key_file);
		strcat(key_file, "_key.txt");
		FILE* new_fp;
		new_fp = fopen(key_file, "w");
		if (new_fp == NULL) {
			fprintf(stderr, "���ļ�ʧ��\n");
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

//��ȡ�Ѵ��ļ�������Ŀ¼
void get_file_directory(char file_name[],int length,char key_file[]) {
	for (int i = 0; i < length; i++) {
		//ȷ��"."��λ��
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
