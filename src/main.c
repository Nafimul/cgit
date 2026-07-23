#include <stdio.h>
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

bool cat(char *filepath)
{
    if (!isTxt(filepath))
        return false;

    FILE *fileP = fopen(filepath, "r");
    if (fileP == NULL)
        return false;

    char fileStr[100];

    while (fgets(fileStr, 100, fileP))
        printf("%s", fileStr);
    printf("\n");

    fclose(fileP);
    return true;
}

void selectCommand(void)
{
    char FILEPATH[] = "../gitFiles/message.txt\0";
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
        cat(FILEPATH);
}

int main(void)
{
    selectCommand();
    return 0;
}
