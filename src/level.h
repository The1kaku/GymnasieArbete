#ifndef LEVEL_H
#define LEVEL_H

#define LEVEL_HEIGHT 100
#define LEVEL_WIDTH 100

typedef char **Level;

char **readLevelFromFile(const char *f);

#endif