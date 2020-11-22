#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "level.h"

char **
readLevelFromFile(const char *f)
{
    FILE *fp;
    char *emptyRow, **level;

    fp = fopen(f, "r");
    if (fp == NULL) {
        printf("Failed to open file \"%s\" reason:\n%s\n", f, strerror(errno));
        return NULL;
    }

    emptyRow = (char *) calloc(LEVEL_WIDTH + 1, sizeof(char));
    for (int i = 0; i < LEVEL_WIDTH; i++)
        emptyRow[i] = ' ';
    emptyRow[LEVEL_WIDTH] = '\0';

    level = (char **) calloc(LEVEL_HEIGHT, sizeof(char *));
    for (int i = 0; i < LEVEL_HEIGHT; i++) {
        level[i] = (char *) calloc(LEVEL_WIDTH + 1, sizeof(char));
        if (!ferror(fp) && !feof(fp)) {
            char *nl;
            fgets(level[i], LEVEL_WIDTH, fp);
            if ((nl = strchr(level[i], '\n')) != NULL)
                *nl = ' ';
            strncat(level[i], emptyRow, LEVEL_WIDTH - strlen(level[i]));
            level[i][LEVEL_WIDTH] = '\0';
        } else {
            strcpy(level[i], emptyRow);
        }
    }
    return level;
}