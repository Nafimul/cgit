#pragma once
#include <stdbool.h>

typedef struct
{
    char *filePath;
    char *contents;
} TxtFile;

TxtFile *toTxtFile(char *filepath);

bool txtFileEditLine(TxtFile *file, char *newLine, int lineNum);

void txtFileFree(void *ptr);
