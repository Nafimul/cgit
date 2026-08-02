#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/txt_file.h"
#include "../include/utils.h"

TxtFile *toFileObj(char *filepath)
{
    if (!isTxt(filepath))
        return NULL;

    cat("start");
    FILE *file = fopen(filepath, "r");
    cat("poststart");
    if (file == NULL)
        return NULL;

    TxtFile *fileObj = malloc(sizeof(TxtFile));
    char *fileStr = malloc(sizeof(char));

    const int BUFFERSIZE = 20;
    char buffer[BUFFERSIZE];
    while (fgets(buffer, BUFFERSIZE, file))
        strcat(fileStr, buffer);

    cat("preend");
    cat(fileStr);
    if (fclose(file) == -1)
        return NULL;
    file = NULL;
    cat("end");
    fileObj->contents = fileStr;
    fileObj->filePath = filepath;
    return fileObj;
}