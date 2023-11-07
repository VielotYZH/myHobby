// n replace -1

#include <stdio.h>
#include <stdlib.h>

#define n 3

int sourceSize = 60;

char *decode(char *coding)
{
    char *result = (char *)malloc(sourceSize * sizeof(char));
    int index = 0;
    int last = 1;
    while (coding[index] != '\0')
    {
        if (coding[index] == 'n')
        {
            result[index] = '1';
            if (last == -1)
            {
                for (int i = index; i >= index - n; i--)
                {
                    result[i] = '0';
                }
            }
            else
            {
                last = -last;
            }
        }
        else if (coding[index] == '1')
        {
            result[index] = '1';
            if (last == 1)
            {
                for (int i = index; i >= index - n; i--)
                {
                    result[i] = '0';
                }
            }
            else
            {
                last = -last;
            }
        }
        else if (coding[index] == '0')
        {
            result[index] = '0';
        }
        index++;
    }
    result[index] = '\0';
    return result;
}

char *encode(char *source)
{
    char *result = (char *)malloc(sourceSize * sizeof(char));
    int index = 0;
    int counter = 0;
    int last = 1;  // The first 1 is encoded as -1
    int lastV = 1; // The first V is encoded as -V
    while (source[index] != '\0')
    {
        if (source[index] == '1')
        {
            counter = 0; // A 1 indicates continuous 0 interruption
            result[index] = last == 1 ? 'n' : '1';
            last = -last;
        }
        else if (source[index] == '0')
        {
            counter++;
            result[index] = '0';
            if (counter == n + 1)
            {
                counter = 0;
                result[index] = lastV == 1 ? 'n' : '1';
                lastV = -lastV;
                if (result[index - n - 1] != result[index])
                {
                    result[index - n] = result[index];
                }
            }
        }
        index++;
    }
    result[index] = '\0'; // Add a terminator '\0' to the result manually
    return result;
}

char *readFile(FILE *f)
{
    char *source = (char *)malloc(sourceSize * sizeof(char));
    fgets(source, sourceSize, f);
    while (!feof(f))
    {
        rewind(f); // Re-read from the beginning of the file each time
        sourceSize = sourceSize * 1.5;
        source = realloc(source, sourceSize);
        fgets(source, sourceSize, f);
    }
    return source;
}

void main()
{
    FILE *f = fopen("test.txt", "r");
    if (f == NULL)
    {
        printf("file is not exist");
        exit(1);
    }
    char *source = readFile(f);
    char *coding = encode(source);
    char *decoding = decode(coding);
    printf("source:\t\t%s\n", source);
    printf("coding:\t\t%s\n", coding);
    printf("decoding:\t%s\n", decoding);
}