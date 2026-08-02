typedef struct
{
    char *filePath;
    char *contents;
} TxtFile;

TxtFile *toFileObj(char *filepath);