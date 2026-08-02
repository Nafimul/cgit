#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../include/linked_list.h"

void cat(char *string)
{
    printf("%s\n", string);
}

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

bool startsWith(char *str, char *substr)
{
    int strLen = strlen(str);
    int substrLen = strlen(substr);
    if (substrLen == 0 || strLen == 0)
        return false;
    for (int i = 0; i < substrLen; i++)
    {
        if (str[i] != substr[i])
            return false;
    }
    return true;
}

bool isTxt(char *filepath)
{
    return endsWith(filepath, ".txt\0");
}

List *splitStr(char *str, char delimiter)
{
    List *strings = linkedListCreate();

    linkedListAddToEnd(strings, strtok(str, &delimiter));

    return strings;
}

void removeCharFromStart(char *str, char toRemove)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != toRemove)
            return;
    }
}

char *getUserInput(int maxLen)
{
    int maxBits = maxLen * sizeof(char);
    char *input = malloc(maxBits);
    while (true)
    {
        input = fgets(input, maxBits, stdin);
        // if fgets failed, return null
        if (input == NULL)
            return NULL;
        // if there are newlines left in stdin, get rid of them
        if (strcmp(input, "") != 0 && strcmp(input, "\n") != 0)
            break;
        // if input was too long return null
        if (strstr(input, "\n") == NULL)
            return NULL;
    }
    return input;
}