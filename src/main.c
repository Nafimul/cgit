#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/linked_list.h"

bool endsWith(char *str, char *substr)
{
    int strLen = strlen(str);
    int substrLen = strlen(substr);
    if (substrLen == 0 || strLen == 0)
        return false;
    for (int i = 1; i < substrLen; i++)
    {
        if (str[strLen - i] != substr[substrLen - i])
            return false;
    }
    return true;
}

bool isTxt(char *filepath)
{
    return endsWith(filepath, ".txt\0");
}

char *fileToString(char *filepath)
{
    if (!isTxt(filepath))
        return NULL;

    FILE *fileP = fopen(filepath, "r");
    if (fileP == NULL)
        return NULL;

    char *fileStr = malloc(sizeof(char *));
    const int BUFFERSIZE = 20;
    char buffer[BUFFERSIZE];
    while (fgets(buffer, BUFFERSIZE, fileP))
        strcat(fileStr, buffer);

    fclose(fileP);
    return fileStr;
}

void cat(char *string)
{
    printf("%s\n", string);
}

void selectCommand(void)
{
    char FILEPATH[] = "../gitFiles/message.txt\0";
char *fileContents = fileToString(FILEPATH);
    printf("Enter a number:\n");
    printf("1) cat %s\n", FILEPATH);
    printf("2) git add %s\n", FILEPATH);
    printf("3) git commit -m %s\n", FILEPATH);
    printf("4) git revert %s\n", FILEPATH);
    printf("5) git log %s\n", FILEPATH);

    // int choice;
    // scanf("%d", &choice);
    int choice = 1;

    if (choice == 1)
        cat(fileContents);

    free(fileContents);
}

int main(void)
{
    selectCommand();
    return 0;
}
