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

bool selectCommand(List *commits, List *files)
{
    char *FILEPATH = "../gitFiles/a.txt\0";
    TxtFile *file = toFileObj(FILEPATH);
    if (file == NULL)
    {
        cat("failed to read file");
        return false;
    }

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

    // char *FILEPATHS[3] = {"../gitFiles/a.txt\0", "../gitFiles/b.txt\0", "../gitFiles/c.txt\0"};
    char *FILEPATHS[1] = {"../gitFiles/a.txt\0"};
    int numStartingFiles = sizeof(FILEPATHS) / sizeof(FILEPATHS[0]);
    for (int i = 0; i < numStartingFiles; i++)
    {
        TxtFile *file = toFileObj(FILEPATHS[i]);
        if (file == NULL)
            exit(EXIT_FAILURE);
        linkedListAddToEnd(files, file);
    }

    while (true)
    {
        if (!selectCommand(commits, files))
            break;
    }
    return 0;
}