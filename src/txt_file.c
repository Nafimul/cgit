#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/txt_file.h"
#include "../include/utils.h"

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
    char *newContents = malloc(sizeof(char) * strlen(file->contents) + sizeof(newLine));
    if (newContents == NULL)
        return false;
    List *oldLines = splitStr(file->contents, '\n');

    for (int i = 0; i < linkedListLength(oldLines); i++)
    {
        if (i == lineNum - 1)
        {
            strcat(newContents, newLine);
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
            strcat(newContents, "\n");
        }
    }

    free(file->contents);
    file->contents = newContents;
    return true;
}

TxtFile *toTxtFile(char *filepath)
{
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

cleanup:
    if (f)
        fclose(f);
    free(buffer);

    if (fileObj && fileObj->filePath == NULL)
        txtFileFree(fileObj);
    return fileObj;
}