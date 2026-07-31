#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/linked_list.h"

void cat(char *string)
{
    printf("%s\n", string);
}

typedef struct
{
    char *filePath;
    char *oldLine;
    char *newLine;
    int lineNum;
} Change;

typedef struct
{
    char *message;
    List *changes;
} Commit;

typedef struct
{
    char *filePath;
    char *contents;
} TxtFile;

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

TxtFile *toFileObj(char *filepath)
{
    if (!isTxt(filepath))
        return NULL;

    FILE *file = fopen(filepath, "r");
    if (file == NULL)
        return NULL;

    TxtFile *fileObj = malloc(sizeof(TxtFile));
    char *fileStr = malloc(sizeof(char));

    const int BUFFERSIZE = 20;
    char buffer[BUFFERSIZE];
    while (fgets(buffer, BUFFERSIZE, file))
        strcat(fileStr, buffer);

    fclose(file);
    file = NULL;
    fileObj->contents = fileStr;
    fileObj->filePath = filepath;
    return fileObj;
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

bool commit(List *commits, TxtFile *newFile, List *oldFiles)
{
    cat("enter a commit message");
    char *message = getUserInput(100);

    List *changes = linkedListCreate();
    TxtFile *oldFile = NULL;

    // get the oldFile corresponding to newFile
    for (int i = 0; i < linkedListLength(oldFiles); i++)
    {
        linkedListGetValue(oldFiles, i, (void **)&oldFile);

        if (oldFile != NULL && strcmp(oldFile->filePath, newFile->filePath) == 0)
            break;
        // if the old file was not found
        if (i == linkedListLength(oldFiles) - 1)
            return false;
    }

    List *oldLines = splitStr(oldFile->contents, '\n');
    List *newLines = splitStr(newFile->contents, '\n');

    for (int lineNum = 1;; lineNum++)
    {
        char *oldLine = NULL;
        linkedListGetValue(oldLines, lineNum - 1, (void **)&oldLine);
        char *newLine = NULL;
        linkedListGetValue(newLines, lineNum - 1, (void **)&newLine);
        if (oldLine == NULL || newLine == NULL)
            break;
        cat(oldLine);
        cat(newLine);
        if (strcmp(oldLine, newLine) != 0)
        {
            Change *change = malloc(sizeof(Change));
            *change = (Change){.filePath = newFile->filePath, .lineNum = lineNum, .newLine = newLine, .oldLine = oldLine};
            cat(change->newLine);
            cat(change->oldLine);
            linkedListAddToEnd(changes, change);
        }
    }

    Commit *commit = malloc(sizeof(Commit));
    *commit = (Commit){.message = message, .changes = changes};
    return true;
}

void gitLog(List *commits)
{
    for (int i = 0; i < linkedListLength(commits); i++)
    {
        Commit *commit = NULL;
        linkedListGetValue(commits, i, (void **)&commit);
        printf("%d) %s", i + 1, commit->message);
    }
}

bool selectCommand(List *commits, List *files)
{
    char *FILEPATH = "../gitFiles/c.txt\0";
    TxtFile *file = toFileObj(FILEPATH);
    printf("Enter a number:\n");
    printf("1) cat %s\n", FILEPATH);
    printf("2) git commit -m %s\n", FILEPATH);
    printf("3) git revert %s\n", FILEPATH);
    printf("4) git log\n");
    printf("5) exit\n");

    int choice;
    scanf("%d", &choice);
    // int choice = 2;

    if (choice == 1)
        cat(file->contents);
    else if (choice == 2)
    {
        if (commit(commits, file, files))
            cat("succsessfully commited");
    }
    else if (choice == 4)
        gitLog(commits);
    else if (choice == 5)
        return false;
    return true;
}

int main(void)
{
    List *commits = linkedListCreate();
    List *files = linkedListCreate();

    char *FILEPATHS[3] = {"../gitFiles/a.txt\0", "../gitFiles/b.txt\0", "../gitFiles/c.txt\0"};
    int numStartingFiles = sizeof(FILEPATHS) / sizeof(FILEPATHS[0]);
    for (int i = 0; i < numStartingFiles; i++)
    {
        TxtFile *file = toFileObj(FILEPATHS[i]);
        linkedListAddToEnd(files, file);
    }

    while (true)
    {
        if (!selectCommand(commits, files))
            break;
    }
    return 0;
}