#include <stdbool.h>
#include "../include/linked_list.h"

#define CHECK(x)          \
    do                    \
    {                     \
        if (!(x))         \
            goto cleanup; \
    } while (0)

void cat(char *string);

bool endsWith(char *str, char *substr);

bool startsWith(char *str, char *substr);

bool isTxt(char *filepath);

List *splitStr(char *str, char *delimiter);

char *getUserInput(int maxLen);

char *parseStr(char *str, char delim, int numDelimsToSkip);

int getUserDigitInputAboveZero(int max);