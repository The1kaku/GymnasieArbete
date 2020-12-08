#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DEFAULT_WIDTH 20
#define DEFAULT_HEIGHT 20
#define DEFAULT_RECT_MIN_HEIGHT 2
#define DEFAULT_RECT_MIN_WIDTH 2
#define DEFAULT_MAX_TRIES 100000000

typedef char **Level;
typedef char *LevelRow;
typedef char LevelUnit;
typedef unsigned int Room[4];

Level allocLevel(const unsigned height, const unsigned width);
Level genRect(const unsigned maxHeight, const unsigned maxWidth, const unsigned minHeight, const unsigned minWidth, unsigned *heightPtr, unsigned *widthPtr);
int rectCollision(int t1, int h1, int t2, int h2, int l1, int w1, int l2, int w2);

long long counter = 0;

int 
main(int argc, char *argv[])
{
    unsigned int n, height, width, rectMinHeight, rectMinWidth, rectMaxHeight, rectMaxWidth, maxTries, rectHeight, rectWidth, ymax, xmax, ymin, xmin, y, x;
    Room *rooms, *last;
    Level level, rect;
    if (argc < 2) {
        printf("Invalid input to %s\n", argv[0]);
        exit(-1);
    } 
    n = strtoul(argv[1], NULL, 10);
    height = (argc >= 3) ? strtoul(argv[2], NULL, 10) : DEFAULT_HEIGHT;
    width = (argc >= 4) ? strtoul(argv[3], NULL, 10) : DEFAULT_WIDTH;
    rectMinHeight = (argc >= 5) ? strtoul(argv[4], NULL, 10) : DEFAULT_RECT_MIN_HEIGHT;
    rectMinWidth = (argc >= 6) ? strtoul(argv[5], NULL, 10) : DEFAULT_RECT_MIN_WIDTH;
    rectMaxHeight = (argc >= 7) ? strtoul(argv[6], NULL, 10) : height/2;
    rectMaxWidth = (argc >= 8) ? strtoul(argv[7], NULL, 10) : width/2;
    maxTries = (argc >= 9) ? strtoul(argv[8], NULL, 10) : DEFAULT_MAX_TRIES;
    
    if ((level = allocLevel(height, width)) == NULL) {
        printf("Could not allocate level of size %dx%d\n", height, width);
        exit(-1);
    }

    if ((rooms = (Room *) malloc(n * sizeof(Room))) == NULL) {
        printf("Could not allocate %d rooms.\n", n);
        exit(-1);
    }
    last = rooms;

    srand(time(0));

    ymin = 0;
    xmin = 0;

    for (int i = 0; i < n; i++) {
        if ((rect = genRect(rectMaxHeight, rectMaxWidth, rectMinHeight, rectMinWidth, &rectHeight, &rectWidth)) == NULL) {
            printf("Failed to generate rect of size %dx%d\n", rectHeight, rectWidth);
            exit(-1);
        }

        ymax = height - rectHeight;
        xmax = width - rectWidth;

        for (int j = 0; j < maxTries; j++) {
            Room *beg;
            y = (rand() % (ymax - ymin + 1)) + ymin;
            x = (rand() % (xmax - xmin + 1)) + xmin; 
            for (beg = &rooms[0]; beg < last; beg++)
                if (rectCollision(y, rectHeight, (*beg)[0], (*beg)[2], x, rectWidth, (*beg)[1], (*beg)[3]) == 1)
                    break;
            counter++;
            if (beg == last)
                break;
        }
        for (int j = 0; j < rectHeight; j++)
            memcpy(&level[y+j][x], rect[j], rectWidth);
        (*last)[0] = y;
        (*last)[1] = x;
        (*last)[2] = rectHeight;
        (*last)[3] = rectWidth;
        last++;
        //printf("RECTANGLE (%d) :\n area: %d x %d = %d\n posRange: ymax(%d) xmax(%d)\n pos: y(%d) x(%d)\n\n\n", i, rectHeight, rectWidth, rectHeight * rectWidth, ymax, xmax, y, x);
        free(rect);
    }
   
    for (int i = 0; i < height; i++) {
        printf("%s\n", level[i]);
    }

    printf("RECTCOL COUNTER = %I64d", counter);

    return 0;
}

Level
allocLevel(const unsigned height, const unsigned width)
{
    Level level;
    if ((level = (Level) calloc(height+1, sizeof(LevelRow))) == NULL)
        return NULL;
    for (int i = 0; i < height; i++)
        if ((level[i] = (LevelRow) calloc(width+1, sizeof(LevelUnit))) == NULL) {
            return NULL;
        } else {
            memset(level[i], ' ', width);
            level[i][width] = '\0';
        }
            
    return level;
}

Level 
genRect(const unsigned maxHeight, const unsigned maxWidth, const unsigned minHeight, const unsigned minWidth, unsigned *heightPtr, unsigned *widthPtr)
{
    Level room;
    LevelRow empty;
    int width, height;
    width = (rand() % (maxWidth - minWidth + 1)) + minWidth;
    height = (rand() % (maxHeight - minHeight + 1)) + minHeight;    
    *heightPtr = height;
    *widthPtr = width;

    if ((room = allocLevel(height, width)) == NULL)
        return NULL;
    
    if ((empty = (LevelRow) calloc(width+1, sizeof(LevelUnit))) == NULL)
        return NULL;
    
    memset(empty, '.', width); 
    empty[0] = '#'; empty[width-1] = '#';
    empty[width] = '\0';

    memset(room[0], '#', width);
    for (int i = 1; i < height-1; i++)
        memcpy(room[i], empty, width);
    memset(room[height-1], '#', width);

    return room;
}

int 
rectCollision(int t1, int h1, int t2, int h2, int l1, int w1, int l2, int w2)
{
    return (t1 + h1 > t2 && t1 < t2 + h2 && l2 < l1 + w1 && l2 + w2 > l1);
}