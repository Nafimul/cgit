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

TxtFile *getOldFile(TxtFile *newFile, List *oldFiles)
{
    TxtFile *oldFile = NULL;
    for (int i = 0; i < linkedListLength(oldFiles); i++)
    {
        linkedListGetValue(oldFiles, i, (void **)&oldFile);

        if (oldFile != NULL && strcmp(oldFile->filePath, newFile->filePath) == 0)
            break;
        // if the old file was not found
        if (i == linkedListLength(oldFiles) - 1)
            return NULL;
    }
    return oldFile;
}

bool applyChange(TxtFile *file, Change *change)
{
    return txtFileEditLine(file, change->newLine, change->lineNum);
}

bool applyCommit(TxtFile *file, Commit *commit)
{
    bool success = true;

    if (commit->changes == NULL)
        return success;

    for (int i = 0; i < linkedListLength(commit->changes); i++)
    {
        Change *change = NULL;
        linkedListGetValue(commit->changes, i, (void **)&change);
        if (!applyChange(file, change))
            success = false;
    }

    return success;
}

bool commit(List *commits, TxtFile *newFile, List *oldFiles)
{
    cat("enter a commit message");
    char *message = getUserInput(100);

    List *changes = linkedListCreate();
    TxtFile *oldFile = getOldFile(newFile, oldFiles);

    List *oldLines = splitStr(oldFile->contents, '\n');
    List *newLines = splitStr(newFile->contents, '\n');

    for (int lineNum = 1;; lineNum++)
    {
        char *oldLine = NULL;
        linkedListGetValue(oldLines, lineNum - 1, (void **)&oldLine);
        char *newLine = NULL;
        linkedListGetValue(newLines, lineNum - 1, (void **)&newLine);
        if (newLine == NULL)
            break;

        if (oldLine == NULL || strcmp(oldLine, newLine) != 0)
        {
            Change *change = malloc(sizeof(Change));
            *change = (Change){.filePath = newFile->filePath, .lineNum = lineNum, .newLine = newLine, .oldLine = oldLine};
            linkedListAddToEnd(changes, change);
        }
    }

    Commit *commit = malloc(sizeof(Commit));
    *commit = (Commit){.message = message, .changes = changes};
    linkedListAddToEnd(commits, commit);
    applyCommit(oldFile, commit);
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
        TxtFile *newFile = toTxtFile(oldFile->filePath);
        return newFile;
    }
}

bool revertChange(Change *change)
{
    bool success = false;

    TxtFile *txtFile = toTxtFile(change->filePath);
    CHECK(txtFileEditLine(txtFile, change->oldLine, change->lineNum));

    FILE *newFile = fopen(change->filePath, "w+");
    CHECK(newFile != NULL);
    CHECK(fprintf(newFile, "%s", txtFile->contents) >= 0);
    cat(txtFile->contents);
    success = true;

cleanup:
    fclose(newFile);
    txtFileFree(txtFile);
    return success;

    // int BUFFERSIZE = 50;
    // char buffer[BUFFERSIZE];
    // FILE *file = fopen(change->filePath, "r+");
    // for (int i = 1; i < change->lineNum; i++)
    // {
    //     fgets(buffer, BUFFERSIZE, file);
    // }
    // int lineStartPos = ftell(file);
    // fgets(buffer, BUFFERSIZE, file);
    // int lineEndPos = ftell(file);

    // free(file);
}

bool revertCommit(Commit *commit)
{
    printf("%d", linkedListLength(commit->changes));
    bool success = true;

    if (commit->changes == NULL)
        return success;

    for (int i = 0; i < linkedListLength(commit->changes); i++)
    {
        Change *change = NULL;
        linkedListGetValue(commit->changes, i, (void **)&change);
        if (!revertChange(change))
            success = false;
    }

    return success;
}

Commit *selectCommit(List *commits)
{
    if (linkedListLength(commits) == 0)
        return NULL;
    printf("Enter a number:\n");
    for (int i = 0; i < linkedListLength(commits); i++)
    {
        Commit *commit = NULL;
        linkedListGetValue(commits, i, (void **)&commit);
        printf("%d) %s\n", i + 1, commit->message);
    }

    while (true)
    {
        int choice;
        scanf("%d", &choice);
        Commit *commit = NULL;
        linkedListGetValue(commits, choice - 1, (void **)&commit);
        if (commit == NULL)
            continue;
        return commit;
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
            cat("successfully commited");
        else
            crash();
    }
    else if (choice == 3)
    {
        Commit *commit = selectCommit(commits);
        if (commit == NULL)
            cat("there are no commits to revert");
        else
        {
            if (revertCommit(commit))
                cat("successfully reverted!");
            else
                crash();
        }
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

// for random c stuff i need to quickly test. nothing to do with the program
void test(void)
{
    char *str = "wow\0";
    printf("%d\n", strlen(str));
}

int main(void)
{
    test();
    List *commits = linkedListCreate();
    List *files = linkedListCreate();

    char *FILEPATHS[3] = {"../gitFiles/a.txt\0", "../gitFiles/b.txt\0", "../gitFiles/d.txt\0"};
    int numStartingFiles = sizeof(FILEPATHS) / sizeof(FILEPATHS[0]);
    for (int i = 0; i < numStartingFiles; i++)
    {
        TxtFile *file = toTxtFile(FILEPATHS[i]);
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