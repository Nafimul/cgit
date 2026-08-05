#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/txt_file.h"
#include "../include/utils.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void txtFileFree(TxtFile *file)
{
    if (!file)
        return;

    free(file->contents);
    free(file->filePath);
    free(file);
}

bool txtFileEditLine(TxtFile *file, char *newLine, int lineNum)
{
    if (file == NULL || newLine == NULL || lineNum <= 0)
        return NULL;

    List *oldLines = NULL;
    char *newContents = NULL;

    newContents = malloc(sizeof(char) * (strlen(file->contents) + strlen(newLine) + 2));
    CHECK(newContents != NULL);
    strcpy(newContents, "");
    oldLines = splitStr(file->contents, "\n");
    CHECK(oldLines != NULL);

    int newContentsNumLines = MAX(linkedListLength(oldLines), lineNum);
    for (int i = 0; i < newContentsNumLines; i++)
    {
        if (i == lineNum - 1)
        {
            strcat(newContents, newLine);
            if (i != newContentsNumLines - 1)
                strcat(newContents, "\n");
        }
        else
        {
            char *oldLine = NULL;
            linkedListGetValue(oldLines, i, (void **)&oldLine);
            if (oldLine != NULL)
            {
                strcat(newContents, oldLine);
            }
            if (i != newContentsNumLines - 1)
                strcat(newContents, "\n");
        }
    }

    linkedListFree(oldLines, true);
    free(file->contents);
    file->contents = newContents;
    return true;

cleanup:
    if (newContents != NULL)
        free(newContents);
    if (oldLines != NULL)
        free(oldLines);
    return false;
}

TxtFile *toTxtFile(char *filepath)
{
    if (filepath == NULL)
        return NULL;
    // Source for reading to file to buffer (modified by me) - https://stackoverflow.com/a/174552
    // Posted by Nils Pipenbrinck
    // Retrieved 2026-08-02, License - CC BY-SA 2.5

    char *buffer = 0;
    long length;
    TxtFile *fileObj = NULL;

    FILE *f = fopen(filepath, "rb");
    CHECK(f != NULL);
    CHECK(fseek(f, 0, SEEK_END) == 0);
    length = ftell(f);
    CHECK(length != -1L);
    CHECK(fseek(f, 0, SEEK_SET) == 0);
    buffer = malloc(length + 1);
    CHECK(buffer != NULL);
    CHECK(fread(buffer, 1, length, f) == (size_t)length);
    buffer[length] = '\0';

    fileObj = malloc(sizeof(TxtFile));
    CHECK(fileObj != NULL);
    fileObj->contents = buffer;
    fileObj->filePath = strdup(filepath);
    buffer = NULL;

    fclose(f);
    return fileObj;

cleanup:
    if (f)
        fclose(f);
    free(buffer);
    txtFileFree(fileObj);
    return NULL;
}