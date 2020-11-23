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

int **
getRoomBordersFromActor(const Level l, const Actor *a)
{
    return getRoomBordersFromPoint(l, a->y, a->x);
}

int **
getRoomBordersFromPoint(const Level l, const int y, const int x)
{
    int i, j, **ret;
    ret = (int **) malloc(2 * sizeof(int*));
    ret[0] = (int *) malloc(2 * sizeof(int));
    ret[1] = (int *) malloc(2 * sizeof(int));
    ret[0][0] = ret[0][1] = ret[1][0] = ret[1][1] = -1;

    if (y < 0 || y > LEVEL_HEIGHT || x < 0 || x > LEVEL_WIDTH)
        return ret;
    if (l[y][x] == '#' || l[y][x] == ' ')
        return ret;
    
    for (i = y; i > 0 && l[i][x] != '#'; i--)
        ;
    for (j = x; j > 0 && l[y][j] != '#'; j--)
        ;
    ret[0][0] = i;
    ret[0][1] = j;

    for (i = y; i < LEVEL_HEIGHT && l[i][x] != '#'; i++)
        ;
    for (j = x; j < LEVEL_WIDTH && l[y][j] != '#'; j++)
        ;
    ret[1][0] = i;
    ret[1][1] = j;

    return ret;
}