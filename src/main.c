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

bool isTxt(char *filename)
{
    return endsWith(filename, ".txt\0");
}

bool cat(char *filename)
{
    if (!isTxt(filename))
        return false;

    FILE *fileP = fopen(filename, "r");
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
    char FILENAME[] = "message.txt\0";
    printf("Enter a number:\n");
    printf("1) cat %s\n", FILENAME);
    printf("2) git add %s\n", FILENAME);
    printf("3) git commit -m %s\n", FILENAME);
    printf("4) git revert %s\n", FILENAME);
    printf("5) git log %s\n", FILENAME);

    // int choice;
    // scanf("%d", &choice);
    int choice = 1;

    if (choice == 1)
        cat(FILENAME);
}

int main(void)
{
    selectCommand();
    struct List *list = linkedListCreate();
    char *wow;
    linkedListAddToEnd(list, "jdgsoinoirfoi");
    linkedListGetValue(list, 0, &wow);
    printf("%s", wow);
    return 0;
}
