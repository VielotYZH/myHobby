/*������ֻ���ı��ļ��е�Ӣ����ĸ����Сд���ɣ������֡�
  �ո��Լ�Ӣ��ʽ�ľ�š����š�ð�š��ֺš��̺��ߡ�̾
  �š��ʺš�˫���š������š�Բ���š������š������š�
  �����Ž��б��루ʡ�Ժ�Ϊ������ţ������ĵ������ַ�
  �����ڰ���ת����ʱʡ��.*/
/*������������Ӧ����������������һ������Ϊ���������
  �����ڶ�������Ϊ��Կ�ļ���*/
  /*�������ж�ȡ�ļ���ÿ�����256���ַ�����ע����
  ����Ԥ����*/

#include"file_processing.h"

void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]);

void main(int argc, char* argv[]) {
	//��鴫���ļ�����
	if (argc != 3) {
		fprintf(stderr, "�������֧�ֶ�һ���ļ��İ�������\n");
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
	//���ɱ����
	generate_encoding_table(key);
	//��ȡ�����ļ�
	char plaintext_name[120];
	strcpy(plaintext_name, argv[1]);
	if (!strstr(plaintext_name, ".txt")) {
		fprintf(stderr, "�������֧�ֶ�.txt�ļ��İ�������\n");
		fprintf(stderr, "����ĵ�һ���ļ�����.txt��ʽ\n");
		exit(0);
	}
	FILE* plaintext = fopen(plaintext_name, "r");
	if (plaintext == NULL) {
		fprintf(stderr, "File %s not found\n", plaintext_name);
		exit(1);
	}
	//������Ŀ¼�´��������ļ�
	char ciphertext_name[120];
	get_file_directory(plaintext_name, 120, ciphertext_name);
	strcat(ciphertext_name, "_ciphertext.txt");
	FILE* ciphertext = fopen(ciphertext_name, "w");
	if (ciphertext == NULL) {
		fprintf(stderr, "�����ļ�����ʧ��\n");
		exit(0);
	}
	//���ݱ�������ִ�ӡ�����ļ���
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

//��ȡ�Ѵ��ļ�������Ŀ¼
void get_file_directory(char source_file_name[], int source_file_length, char destination_file_name[]) {
	for (int i = 0; i < source_file_length; i++) {
		//ȷ��"."��λ��
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