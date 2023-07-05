#include"file_processing.h"
#include"public_variable.h"

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

//生成编码表
void generate_encoding_table(FILE* fp) {
	char token;
	char should_be = 'a';
	int i = -1;
	int j = 0;
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
		}
		else if (current_read_position == 2) {
			if (token != ':') {
				fprintf(stderr, "密钥损坏\n");
				exit(0);
			}
		}
		else if (current_read_position >= 3 && token != -1) {
			if (token != '\n') {
				encoding_table[i][j] = token;
				j++;
			}
		}
	} while (token != -1);
}

//扫描明文文件
//根据编码表逐字打印至新文件中
void generate_ciphertext_content(FILE* plaintext, FILE* ciphertext) {
	char token;
	char content_to_write[30];
	do {
		token = scan_file(plaintext);
		if (isalpha(token)) {
			int index = token >= 'a' ? token - 'a' : token - 'A' + 26;
			for (int i = 0; i < 30; i++) {
				content_to_write[i] = encoding_table[index][i];
				if (content_to_write[i] == '\0') {
					break;
				}
			}
		}
		else {
			content_to_write[1] = '\0';
			if (isdigit(token)) {
				content_to_write[0] = token + 2;
			}
			else {
				switch (token) {
				case ' ':
					content_to_write[0] = 'C';
					break;
				case '.':
					content_to_write[0] = 'D';
					break;
				case ',':
					content_to_write[0] = 'E';
					break;
				case ':':
					content_to_write[0] = 'F';
					break;
				case ';':
					content_to_write[0] = 'G';
					break;
				case '-':
					content_to_write[0] = 'H';
					break;
				case '!':
					content_to_write[0] = 'I';
					break;
				case '?':
					content_to_write[0] = 'J';
					break;
				case '"':
					content_to_write[0] = 'K';
					break;
				case '\'':
					content_to_write[0] = 'L';
					break;
				case '(':
					content_to_write[0] = 'M';
					break;
				case ')':
					content_to_write[0] = 'N';
					break;
				case '[':
					content_to_write[0] = 'O';
					break;
				case ']':
					content_to_write[0] = 'P';
					break;
				case '{':
					content_to_write[0] = 'Q';
					break;
				case '}':
					content_to_write[0] = 'R';
					break;
				case '<':
					content_to_write[0] = 'S';
					break;
				case '>':
					content_to_write[0] = 'T';
					break;
				case '\n':
					content_to_write[0] = 'V';
					break;
				default:
					content_to_write[0] = 'W';
					break;
				}
			}
		}
		fputs(content_to_write, ciphertext);
		if (current_read_position >= buffer_size) {
			fprintf(ciphertext, "\n");
		}
	} while (token != -1);
}