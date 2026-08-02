#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/txt_file.h"
#include "../include/utils.h"

#define CHECK(x)          \
    do                    \
    {                     \
        if (!(x))         \
            goto cleanup; \
    } while (0)

void fileObjFree(TxtFile *file)
{
    if (!file)
        return;

    free(file->contents);
    free(file->filePath);
    free(file);
}

TxtFile *toFileObj(char *filepath)
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
        fileObjFree(fileObj);
    return fileObj;
}

// TxtFile *toFileObj(char *filepath)
// {
//     if (!isTxt(filepath))
//         return NULL;

//     FILE *file = fopen(filepath, "r");
//     if (file == NULL)
//         return NULL;

//     TxtFile *fileObj = malloc(sizeof(TxtFile));
//     char *fileStr = malloc(sizeof(char));

//     const int BUFFERSIZE = 20;
//     char buffer[BUFFERSIZE];
//     while (fgets(buffer, BUFFERSIZE, file))
//         strcat(fileStr, buffer);

//     if (fclose(file) == -1)
//     {
//         free(fileObj);
//         free(fileStr);
//         return NULL;
//     }
//     file = NULL;
//     fileObj->contents = fileStr;
//     fileObj->filePath = strdup(filepath);
//     return fileObj;
// }