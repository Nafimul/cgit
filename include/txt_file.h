typedef struct
{
    char *filePath;
    char *contents;
} TxtFile;

TxtFile *toTxtFile(char *filepath);