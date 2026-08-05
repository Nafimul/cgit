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

void changeFree(Change *change, bool recursive)
{
    if (recursive)
    {
        free(change->filePath);
        free(change->oldLine);
        free(change->newLine);
    }

    free(change);
}

void commitFree(Commit *commit, bool recursive)
{
    if (recursive)
    {
        linkedListFree(commit->changes, true);
        free(commit->message);
    }

    free(commit);
}

void crash()
{
    cat("there was an error");
    exit(EXIT_FAILURE);
}

TxtFile *getMatchingFile(TxtFile *file, List *files)
{
    if (file == NULL || files == NULL)
        return NULL;

    TxtFile *mathingFile = NULL;
    for (int i = 0; i < linkedListLength(files); i++)
    {
        linkedListGetValue(files, i, (void **)&mathingFile);

        if (mathingFile != NULL && strcmp(mathingFile->filePath, file->filePath) == 0)
            break;
        // if the old file was not found
        if (i == linkedListLength(files) - 1)
            return NULL;
    }
    return mathingFile;
}

bool applyChange(TxtFile *file, Change *change)
{
    return txtFileEditLine(file, change->newLine, change->lineNum);
}

bool applyCommit(TxtFile *file, Commit *commit)
{
    if (file == NULL || commit == NULL)
        return NULL;

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

Change *changeCreate(char *filePath, int lineNum, char *newLine, char *oldLine)
{
    if (filePath == NULL || lineNum <= 0)
        return NULL;

    char *filePathCopy = NULL;
    char *newLineCopy = NULL;
    char *oldLineCopy = NULL;
    filePathCopy = malloc(sizeof(char) * (strlen(filePath) + 1));
    if (newLine != NULL)
        newLineCopy = malloc(sizeof(char) * (strlen(newLine) + 1));
    else
        newLineCopy = malloc(2 * sizeof(char));
    if (oldLine != NULL)
        oldLineCopy = malloc(sizeof(char) * (strlen(oldLine) + 1));
    else
        oldLineCopy = malloc(2 * sizeof(char));
    CHECK(filePathCopy != NULL);
    CHECK(newLineCopy != NULL);
    CHECK(oldLineCopy != NULL);

    Change *change = malloc(sizeof(Change));
    CHECK(change != NULL);
    if (oldLine == NULL)
        strcpy(oldLineCopy, "");
    else
        strcpy(oldLineCopy, oldLine);
    if (newLine == NULL)
        strcpy(newLineCopy, "");
    else
        strcpy(newLineCopy, newLine);
    strcpy(filePathCopy, filePath);

    *change = (Change){.filePath = filePathCopy, .lineNum = lineNum, .newLine = newLineCopy, .oldLine = oldLineCopy};
    return change;

cleanup:
    if (filePathCopy != NULL)
        free(filePathCopy);
    if (newLineCopy != NULL)
        free(newLineCopy);
    if (oldLineCopy != NULL)
        free(oldLineCopy);
    if (change != NULL)
        free(change);

    return NULL;
}

Commit *commitCreate(char *message, List *changes)
{
    if (message == NULL || changes == NULL)
        return NULL;

    char *messageCopy = NULL;
    messageCopy = malloc(sizeof(char) * (strlen(message) + 1));
    CHECK(messageCopy != NULL);

    Commit *commit = malloc(sizeof(Commit));
    CHECK(commit != NULL);
    strcpy(messageCopy, message);

    *commit = (Commit){.message = messageCopy, .changes = changes};
    return commit;

cleanup:
    if (message != NULL)
        free(message);
    if (commit != NULL)
        free(commit);

    return NULL;
}

bool commit(List *commits, TxtFile *newFile, List *oldFiles)
{
    if (commits == NULL || newFile == NULL || oldFiles == NULL)
        return NULL;

    List *changes = NULL;
    List *oldLines = NULL;
    List *newLines = NULL;
    Commit *commit = NULL;

    cat("enter a commit message");
    const int MAX_MESSAGE_LEN = 100;
    char *message = getUserInput(MAX_MESSAGE_LEN);
    CHECK(message != NULL);

    changes = linkedListCreate();
    CHECK(changes != NULL);
    TxtFile *oldFile = getMatchingFile(newFile, oldFiles);

    oldLines = splitStr(oldFile->contents, "\n");
    newLines = splitStr(newFile->contents, "\n");

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
            Change *change = changeCreate(newFile->filePath, lineNum, newLine, oldLine);
            CHECK(change != NULL);
            CHECK(linkedListAddToEnd(changes, change) != NULL);
        }
    }

    commit = commitCreate(message, changes);
    CHECK(commit != NULL);
    CHECK(linkedListAddToEnd(commits, commit) != NULL);
    if (!applyCommit(oldFile, commit))
    {
        linkedListRemoveFromEnd(commits, true);
        goto cleanup;
    }

    linkedListFree(oldLines, true);
    linkedListFree(newLines, true);
    return true;

cleanup:
    if (message != NULL)
        free(message);
    if (oldLines != NULL)
        linkedListFree(oldLines, true);
    if (newLines != NULL)
        linkedListFree(newLines, true);
    if (changes != NULL)
        linkedListFree(changes, true);
    if (commit != NULL)
        commitFree(commit, true);
    return false;
}

void gitLog(List *commits)
{
    if (commits == NULL)
        return;

    for (int i = 0; i < linkedListLength(commits); i++)
    {
        Commit *commit = NULL;
        linkedListGetValue(commits, i, (void **)&commit);
        printf("%d) %s", i + 1, commit->message);
    }
}

TxtFile *selectFile(List *files)
{
    if (files == NULL || linkedListLength(files) == 0)
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
        int choiceNum = getUserDigitInputAboveZero(linkedListLength(files));
        Commit *commit = NULL;
        TxtFile *oldFile = NULL;
        linkedListGetValue(files, choiceNum - 1, (void **)&oldFile);
        if (oldFile == NULL)
        {
            cat("invalid number. try again");
            continue;
        }
        TxtFile *newFile = toTxtFile(oldFile->filePath);
        if (newFile == NULL)
            return NULL;
        return newFile;
    }
}

bool revertChange(Change *change)
{
    if (change == NULL)
        return false;

    bool success = false;

    TxtFile *txtFile = toTxtFile(change->filePath);
    CHECK(txtFile != NULL);
    CHECK(txtFileEditLine(txtFile, change->oldLine, change->lineNum));

    FILE *newFile = fopen(change->filePath, "w+");
    CHECK(newFile != NULL);
    CHECK(fprintf(newFile, "%s", txtFile->contents) >= 0);
    success = true;

cleanup:
    fclose(newFile);
    txtFileFree(txtFile);
    return success;
}

bool revertCommit(Commit *commit)
{
    if (commit == NULL)
        return NULL;

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
    if (commits == NULL || linkedListLength(commits) == 0)
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
        int choiceNum = getUserDigitInputAboveZero(linkedListLength(commits));
        Commit *commit = NULL;
        linkedListGetValue(commits, choiceNum - 1, (void **)&commit);
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

    int NUM_CHOICES = 5;
    int choiceNum = getUserDigitInputAboveZero(NUM_CHOICES);

    if (choiceNum == 1)
    {
        file = selectFile(files);
        if (file == NULL)
            crash();
        else
            cat(file->contents);
    }
    else if (choiceNum == 2)
    {
        file = selectFile(files);
        if (file == NULL)
            crash();
        if (commit(commits, file, files))
            cat("successfully commited");
        else
            crash();
    }
    else if (choiceNum == 3)
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
    else if (choiceNum == 4)
        gitLog(commits);
    else if (choiceNum == 5)
    {
        return false;
    }
    return true;
}

// for random c stuff i need to quickly test. nothing to do with the program
void test(void)
{

    char *str = parseStr("wb ikuki\n\nxoom\0", '\n', 1);
    cat(str);

    exit(EXIT_SUCCESS);
}

int main(void)
{
    // test();
    List *commits = linkedListCreate();
    if (commits == NULL)
        crash();
    List *files = linkedListCreate();
    if (files == NULL)
        crash();

    char *FILEPATHS[3] = {"../gitFiles/a.txt\0", "../gitFiles/b.txt\0", "../gitFiles/d.txt\0"};
    int numStartingFiles = sizeof(FILEPATHS) / sizeof(FILEPATHS[0]);
    for (int i = 0; i < numStartingFiles; i++)
    {
        TxtFile *file = toTxtFile(FILEPATHS[i]);
        if (file == NULL)
            crash();
        if (linkedListAddToEnd(files, file) == NULL)
            crash();
    }

    while (true)
    {
        if (!selectCommand(commits, files))
            break;
    }
    return 0;
}