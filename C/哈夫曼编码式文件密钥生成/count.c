#include"count.h"

int buffer_size = 0;
int current_read_position = 0;
bool file_over = false;

//扫描文件
char scan_file(FILE* fp) {
	static char buffer[MAX_BUFFER_SIZE];
	if (current_read_position >= buffer_size) {
		if (fgets(buffer, MAX_BUFFER_SIZE, fp)) {
			buffer_size = strlen(buffer);
			current_read_position = 0;
			return buffer[current_read_position++];
		}
		else {
			file_over = true;
			return '0';
		}
	}
	else {
		return buffer[current_read_position++];
	}
}

//统计每个字母的个数
void count(FILE* fp) {
	char token;
	do {
		token = scan_file(fp);
		if (current_read_position < buffer_size) {
			if (token >= 'a' && token <= 'z') {
				alphabet[token - 'a']++;
			}
			else if (token >= 'A' && token <= 'Z') {
				alphabet[token - 'A' + 26]++;
			}
		}
	} while (!file_over);
}