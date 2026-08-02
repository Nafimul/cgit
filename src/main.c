#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/linked_list.h"
#include "../include/utils.h"
#include "../include/txt_file.h"

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

void crash()
{
    cat("there was an error");
    exit(EXIT_FAILURE);
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
    linkedListAddToEnd(commits, commit);
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

TxtFile *selectFile(List *files)
{
    if (linkedListLength(files) == 0)
        return NULL;
    printf("Enter a number:\n");
    for (int i = 0; i < linkedListLength(files); i++)
    {
        TxtFile *file = NULL;
        linkedListGetValue(files, i, (void **)&file);
        printf("%d) %s\n", i + 1, file->filePath);
    }

    while (true)
    {
        int choice;
        scanf("%d", &choice);
        TxtFile *oldFile = NULL;
        linkedListGetValue(files, choice - 1, (void **)&oldFile);
        if (oldFile == NULL)
            continue;
        TxtFile *newFile = toFileObj(oldFile->filePath);
        return newFile;
    }
}

bool selectCommand(List *commits, List *files)
{
    TxtFile *file = NULL;

    printf("Enter a number:\n");
    printf("1) cat\n");
    printf("2) git commit\n");
    printf("3) git revert\n");
    printf("4) git log\n");
    printf("5) exit\n");

    int choice;
    scanf("%d", &choice);
    // int choice = 2;

    if (choice == 1)
    {
        file = selectFile(files);
        if (file == NULL)
            crash();
        cat(file->contents);
    }
    else if (choice == 2)
    {
        file = selectFile(files);
        if (file == NULL)
            crash();
        if (commit(commits, file, files))
            cat("succsessfully commited");
    }
    else if (choice == 4)
        gitLog(commits);
    else if (choice == 5)
    {
        free(file);
        return false;
    }
    free(file);
    return true;
}

int main(void)
{
    List *commits = linkedListCreate();
    List *files = linkedListCreate();

    char *FILEPATHS[3] = {"../gitFiles/a.txt\0", "../gitFiles/b.txt\0", "../gitFiles/d.txt\0"};
    int numStartingFiles = sizeof(FILEPATHS) / sizeof(FILEPATHS[0]);
    for (int i = 0; i < numStartingFiles; i++)
    {
        TxtFile *file = toFileObj(FILEPATHS[i]);
        if (file == NULL)
            crash();
        linkedListAddToEnd(files, file);
    }

    while (true)
    {
        if (!selectCommand(commits, files))
            break;
    }
    return 0;
}