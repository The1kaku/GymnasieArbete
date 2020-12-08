#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum {
    DEFAULT_LEVEL_HEIGHT = 150,
    DEFAULT_LEVEL_WIDTH = 250,
    START_ROOM_HEIGHT = 5,
    START_ROOM_WIDTH = 7,
    DEFAULT_MAX_DEPTH = 5,
};

enum {
    ROOM_MIN_HEIGHT = 7,
    ROOM_MAX_HEIGHT = 10,
    ROOM_MIN_WIDTH = 10,
    ROOM_MAX_WIDTH = 15,
    ROOM_MIN_DELTA_Y = 15,
    ROOM_MAX_DELTA_Y = 25,
    ROOM_MIN_DELTA_X = 20,
    ROOM_MAX_DELTA_X = 30,
};

typedef struct Room {
    int centery, centerx;
    int height, width;
    int starty, startx;
    int depth;
} Room;

typedef struct Path {
    int length;
    int **dirs;
    int starty;
    int startx;
    int endy;
    int endx;
} Path;


int randomInt(int min, int max);
int randomPercent(int percentage);

char **createLevel(int height, int width);
Path createPath(int starty, int startx, int endy, int endx);
Room createRoom(int d, int cy, int cx, int minh, int maxh, int minw, int maxw);
void generateBranches(Room *room, int depth, int chance);
int generateRandomBranch(Path *path, Room *room, int chance, int cy, int cx, int mindy, int maxdy, int mindx, int maxdx, int minh, int maxh, int minw, int maxw, int depth);
int AABB(Room room);
int rectCollision(int t1, int h1, int t2, int h2, int l1, int w1, int l2, int w2);

void addRoom(char **level, const Room *room, char c);
void addPath(char **level, const Path *path, char c);

void printLevel(FILE *f, const char **level);
void printPath(Path path);
void printRoom(Room room);

Path paths[500];
Room rooms[500];
Room start;
int path_counter = 0;
int max_depth, max_branches, display_depth;

int
main(int argc, char **argv)
{
    char ** level;

    max_depth = (argc >= 2) ? strtol(argv[1], NULL, 10) : DEFAULT_MAX_DEPTH;
    max_branches = (argc >= 3) ? strtol(argv[2], NULL, 10) : __INT_MAX__;
    display_depth = (argc >= 4) ? (strtol(argv[3], NULL, 10) > 0) : 0;

    srand(time(NULL));
    
    level = createLevel(DEFAULT_LEVEL_HEIGHT, DEFAULT_LEVEL_WIDTH);
    start = createRoom(0, DEFAULT_LEVEL_HEIGHT/2, DEFAULT_LEVEL_WIDTH/2, 7, 7, 10, 10);

    generateBranches(&start, 1, 100);

    int n, largest;
    largest = 0;
    for (int i = 0; i < path_counter; i++)
        if (abs(rooms[i].centerx - start.centerx) + abs(rooms[i].centery - start.centery) > largest) {
            largest = abs(rooms[i].centerx - start.centerx) + abs(rooms[i].centery - start.centery);
            n = i;
        }

    for (int i = 0; i < path_counter; i++) {
        addPath(level, &paths[i], '/');
    }

    
    addRoom(level, &start, '$');
    for (int i = 0; i < path_counter; i++) {
        addRoom(level, &rooms[i], (i == n) ? 'E' : (display_depth > 0) ? rooms[i].depth + '0' : '.');
    }

    FILE *f;
    f = fopen("levels/level", "w");
    printLevel(f, level);
}

inline int 
randomInt(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

inline int
randomPercent(int percentage)
{
    return ((rand() % 100) < percentage);
}

char **
createLevel(int height, int width)
{
    char **level;
    level = (char **) malloc(height * sizeof(char *));
    if (level == NULL) {
        fprintf(stderr, "Out of memory");
        exit(0);
    }
    for (int i = 0; i < width+1; i++) {
        level[i] = (char *) malloc((width+1) * sizeof(char));
        if (level[i] == NULL) {
            fprintf(stderr, "Out of memory");
            exit(0);
        }
        memset(level[i], ' ', width);
        level[i][width] = '\0';
    }
    memset(level[0], ' ', width);
    memset(level[1], ' ', width);
    memset(level[2], ' ', width);
    return level;
}

inline Path
createPath(int starty, int startx, int endy, int endx)
{
    int diry, dirx, dy, dx, len, y, x, **dirs;
    if (starty == endy) {
        diry = 0;
    } else {
        diry = (starty > endy) ? -1 : 1;
    } 
    if (startx == endx) {
        dirx = 0;
    } else {
        dirx = (startx > endx) ? -1 : 1;
    }
        
    dy = abs(starty - endy);
    dx = abs(startx - endx);
    y = starty;
    x = startx;
    len = 0;
    dirs = (int **) malloc(1000 * sizeof(int *));
    if (dirs == NULL) {
        fprintf(stderr, "Out of memory");
        exit(0);
    }

    for (int i = 0; i < 1000; i++) {
        dirs[i] = (int *) malloc(2 * sizeof(int));
        if (dirs[i] == NULL) {
            fprintf(stderr, "Out of memory");
            exit(0);
        }
    }

    // for (len = 0; len < ((dy > dx) ? ROOM_MAX_HEIGHT+1 : ROOM_MAX_WIDTH+1); len++) {
    //     dirs[len][0] = (y += (dy > dx) ? diry : 0   );
    //     dirs[len][1] = (x += (dy > dx) ? 0    : dirx);
    // }

    dy = abs(y - endy);
    while (dy != 0) {
        dirs[len][0] = (y += diry);
        dirs[len][1] = (x += 0   );
        dy--;
        len++;
    }
    
    dx = abs(x - endx);
    while (dx != 0) {
        dirs[len][0] = (y += 0   );
        dirs[len][1] = (x += dirx);
        dx--;
        len++;
    }
 
    realloc(dirs, len * sizeof(int[2]));

    return (Path) {.starty = starty, .startx = startx, .endy = endy, .endx = endx, .length = len, .dirs = dirs};
}

inline Room 
createRoom(int d, int cy, int cx, int minh, int maxh, int minw, int maxw)
{
    int h, w;
    h = randomInt(minh, maxh);
    w = randomInt(minw, maxw);
    return (Room) {.centery = cy, .centerx = cx, .height = h, .width = w, .starty = cy - h/2, .startx = cx - w/2, .depth = d};
}

void
generateBranches(Room *room, int depth, int chance)
{
    
// ! DIAGONALS =======
    // if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
    //                             ROOM_MIN_DELTA_Y, ROOM_MAX_DELTA_Y, ROOM_MIN_DELTA_X, ROOM_MAX_DELTA_X, 
    //                             ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
    //                             depth) > 0)
    //     generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    // if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
    //                             -ROOM_MIN_DELTA_Y, -ROOM_MAX_DELTA_Y, ROOM_MIN_DELTA_X, ROOM_MAX_DELTA_X, 
    //                             ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
    //                             depth) > 0)
    //     generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    // if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
    //                             ROOM_MIN_DELTA_Y, ROOM_MAX_DELTA_Y, -ROOM_MIN_DELTA_X, -ROOM_MAX_DELTA_X, 
    //                             ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
    //                             depth) > 0)
    //     generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    // if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
    //                             -ROOM_MIN_DELTA_Y, -ROOM_MAX_DELTA_Y, -ROOM_MIN_DELTA_X, -ROOM_MAX_DELTA_X, 
    //                             ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
    //                             depth) > 0)
    //     generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

// ! HALVES ========
    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                ROOM_MIN_DELTA_Y, ROOM_MAX_DELTA_Y, ROOM_MIN_DELTA_X/2, ROOM_MAX_DELTA_X/2, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                -ROOM_MIN_DELTA_Y/2, -ROOM_MAX_DELTA_Y/2, ROOM_MIN_DELTA_X, ROOM_MAX_DELTA_X, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                ROOM_MIN_DELTA_Y, ROOM_MAX_DELTA_Y, -ROOM_MIN_DELTA_X/2, -ROOM_MAX_DELTA_X/2, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                -ROOM_MIN_DELTA_Y/2, -ROOM_MAX_DELTA_Y/2, -ROOM_MIN_DELTA_X, -ROOM_MAX_DELTA_X, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

//! ZERO ========
    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                ROOM_MIN_DELTA_Y, ROOM_MAX_DELTA_Y, 0, 0, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                -ROOM_MIN_DELTA_Y, -ROOM_MAX_DELTA_Y, 0, 0, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                0, 0, -ROOM_MIN_DELTA_X, -ROOM_MAX_DELTA_X, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);

    if (path_counter < max_branches && generateRandomBranch(&paths[path_counter], &rooms[path_counter], chance, room->centery, room->centerx, 
                                0, 0, -ROOM_MIN_DELTA_X, -ROOM_MAX_DELTA_X, 
                                ROOM_MIN_HEIGHT, ROOM_MAX_HEIGHT, ROOM_MIN_WIDTH, ROOM_MAX_WIDTH, 
                                depth) > 0)
        generateBranches(&rooms[path_counter++], depth+1, chance * 2/3);
}

int 
generateRandomBranch(Path *path, Room *room, int chance, int cy, int cx, int mindy, int maxdy, int mindx, int maxdx, int minh, int maxh, int minw, int maxw, int depth)
{
    if (randomPercent(chance) == 0)
        return 0;
    Room r;
    Path p;

    int dy, dx;
    dy = ((mindy < 0) ? -1 : 1) * randomInt(abs(mindy), abs(maxdy));
    dx = ((mindx < 0) ? -1 : 1) * randomInt(abs(mindx), abs(maxdx));
    cy += dy;
    cx += dx;
    
    p = createPath(cy-dy, cx-dx, cy, cx);
    r = createRoom(depth, cy, cx, minh, maxh, minw, maxw);
    
    if (AABB(r))
        return 0;
    if (r.starty < 0 || r.startx < 0 || r.starty + r.height >= DEFAULT_LEVEL_HEIGHT || r.startx + r.width >= DEFAULT_LEVEL_WIDTH)
        return 0;
    if (depth > max_depth)
        return 0;
    *path = p;
    *room = r;
        
    return 1;
}

int
AABB(Room room)
{
    if (rectCollision(room.starty-3, room.height+3, start.starty-3, start.height+3, room.startx-3, room.width+3, start.startx-3, start.width+3))
        return 1;
    int i;
    for (i = 0; i < path_counter && rectCollision(room.starty-1, room.height+1, rooms[i].starty-1, rooms[i].height+1, room.startx-1, room.width+1, rooms[i].startx-1, rooms[i].width+1) == 0; i++)
        ;
    return !(i == path_counter);
}

int 
rectCollision(int t1, int h1, int t2, int h2, int l1, int w1, int l2, int w2)
{
    return (t1 + h1 > t2 && t1 < t2 + h2 && l2 < l1 + w1 && l2 + w2 > l1);
}

inline void 
addRoom(char **level, const Room *room, char c)
{
    int i = 0;
    for (int j = 0; j < room->width; j++)
        level[room->starty + i][room->startx + j] = (level[room->starty + i][room->startx + j] == '/') ? '/' : '#';
    i++;
    for (; i < room->height-1; i++) {
        level[room->starty + i][room->startx] = (level[room->starty + i][room->startx] == '/') ? '/' : '#';
        for (int j = 1; j < room->width-1; j++) {
            level[room->starty + i][room->startx + j] = c;
        }
        level[room->starty + i][room->startx + room->width - 1] = (level[room->starty + i][room->startx + room->width - 1] == '/') ? '/' : '#';
    }
    for (int j = 0; j < room->width; j++)
        level[room->starty + i][room->startx + j] = (level[room->starty + i][room->startx + j] == '/') ? '/' : '#';
        
}

inline void 
addPath(char **level, const Path *path, char c)
{
    for (int i = 0; i < path->length; i++)
        level[path->dirs[i][0]][path->dirs[i][1]] = c;
}

inline void
printLevel(FILE *f, const char **level)
{
    for (int row = 0; row < DEFAULT_LEVEL_HEIGHT; row++) {
        for (int col = 0; col < DEFAULT_LEVEL_WIDTH; col++)
            fputc(level[row][col], f);
        fputc('\n', f);
    }
}

inline void 
printPath(Path path)
{
    printf("start: (%d %d)\tend: (%d %d)\nlength: (%d)\n", path.starty, path.startx, path.endy, path.endx, path.length);
}

inline void 
printRoom(Room room)
{
    printf("dimensions: (%d %d) to (%d %d)\ncenter: (%d %d)\nwidth: %d\theight: %d\n", room.starty, room.startx, room.starty + room.height, room.startx + room.width, room.centery, room.centerx,  room.width, room.height);
}