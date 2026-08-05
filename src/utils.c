#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../include/linked_list.h"
#include "../include/utils.h"

void cat(char *string)
{
    printf("%s\n", string);
}

/*
 * @return bool whether the string ends with the substring.
 * @note returns false if either string has 0 length. Does not check null character at end.
 */
bool endsWith(char *str, char *substr)
{
    if (str == NULL || substr == NULL)
        return false;
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
    if (str == NULL || substr == NULL)
        return false;
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

/// @return a substring ending at the given delimiter. where it starts depends on numDelimsToSkip.
char *parseStr(char *str, char delim, int numDelimsToSkip)
{
    if (str == NULL || numDelimsToSkip < 0)
        return NULL;

    int delimLocation = -1;
    int startLocation = 0;

    for (int i = 0; i <= numDelimsToSkip; i++)
    {
        startLocation = delimLocation + 1;
        for (int j = startLocation; j <= strlen(str); j++)
        {
            if (str[j] == '\0')
            {
                delimLocation = j + 1;
                if (i < numDelimsToSkip)
                    return NULL;
            }
            if (str[j] == delim)
            {
                delimLocation = j;
                break;
            }
        }
    }

    int subStrLen = (delimLocation - startLocation + 1);
    char *subStr = malloc(sizeof(char) * subStrLen);
    char *offsetStr = str + startLocation;

    char tempSubStr[subStrLen];
    strncpy(tempSubStr, offsetStr, subStrLen);
    tempSubStr[subStrLen - 1] = '\0';
    strncpy(subStr, tempSubStr, subStrLen);

    return subStr;
}

List *splitStr(char *str, char *delimiter)
{
    if (str == NULL)
        return NULL;

    char *tempSubStr = NULL;
    char *strCopy = malloc(sizeof(char) * (strlen(str) + 1));
    CHECK(strCopy != NULL);
    strcpy(strCopy, str);
    List *strings = linkedListCreate();
    CHECK(strings != NULL);

    char *substr = NULL;
    for (int i = 0;; i++)
    {
        tempSubStr = parseStr(strCopy, *delimiter, i);
        if (tempSubStr == NULL)
            break;
        substr = malloc((strlen(tempSubStr) + 1) * sizeof(char));
        CHECK(substr != NULL);
        strcpy(substr, tempSubStr);
        free(tempSubStr);
        tempSubStr = NULL;
        if (linkedListAddToEnd(strings, substr) == NULL)
        {
            free(substr);
            goto cleanup;
        }
    }

    free(strCopy);
    return strings;

cleanup:
    free(strCopy);
    free(tempSubStr);
    linkedListFree(strings, free);
    return NULL;
}

char *getUserInput(int maxLen)
{
    if (maxLen <= 0)
        return NULL;

    int maxBits = maxLen * sizeof(char);
    char *input = malloc(maxBits);
    CHECK(input != NULL);
    while (true)
    {
        char *fgetsResult = fgets(input, maxBits, stdin);
        CHECK(fgetsResult != NULL);
        // only stop taking input when valid, non-blank, input is entered
        if (strcmp(input, "") != 0 && strcmp(input, "\n") != 0)
            break;
        // if input was too long
        CHECK(strstr(input, "\n") != NULL);
    }
    return input;

cleanup:
    if (input != NULL)
        free(input);
    return NULL;
}

int getUserDigitInputAboveZero(int max)
{
    if (max <= 0)
        return -1;

    char *choice = NULL;
    while (true)
    {
        choice = getUserInput(2);
        CHECK(choice != NULL);
        int choiceNum = atoi(choice);
        free(choice);
        if (choiceNum <= 0 || choiceNum > max)
        {
            cat("not valid input. try again");
            continue;
        }
        return choiceNum;
    }

cleanup:
    free(choice);
    return -1;
}