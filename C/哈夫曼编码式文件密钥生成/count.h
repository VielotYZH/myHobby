#include"public.h"
#include<stdbool.h>

#define MAX_BUFFER_SIZE 256

int alphabet[ALPHABET_SIZE];

char scan_file(FILE* fp);
void count(FILE* fp);