#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define MAX_BUFFER_SIZE 256
#define ALPHABET 52

static char buffer[MAX_BUFFER_SIZE];
char encoding_table[ALPHABET][30];

char scan_file(FILE* fp);
void generate_encoding_table(FILE* fp);
void generate_ciphertext_content(FILE* plaintext, FILE* ciphertext);