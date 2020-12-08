#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_ROOM_MIN 5
#define DEFAULT_ROOM_MAX 8
#define DEFAULT_LEVEL_WIDTH 40
#define DEFAULT_LEVEL_HEIGHT 40
#define DEFAULT_ROOM_COUNT 12

#define MAX_TRIES 10000

typedef char **Level;
typedef char *LevelRow;
typedef char LevelUnit;

typedef int Point[2];

Level allocLevel(const unsigned height, const unsigned width);
unsigned int randomNum(register unsigned min, register unsigned max);
Level readRoom(const unsigned int num, const unsigned int size);
Level readLevelFromFile(const char *f, const unsigned int height, const unsigned int width);
int rectCollision(const register int t1, const register int h1, const register int t2, const register int h2, const register int l1, const register int w1, const register int l2, const register int w2);
Point *BFS(Level level, const int ystart, const int xstart, const int yend, const int xend, const unsigned int level_height, const unsigned int level_width, int *length);

unsigned int level_num, room_count, level_height, level_width, room_min, room_max;
Level level;

int 
main(int argc, char *argv[])
{
    Level *rooms;
    Point *doors, *doors_last;

    srand(time(NULL));

    level = NULL;
    level_num = 1;
    room_count = (argc >= 2) ? strtoul(argv[1], NULL, 10) : DEFAULT_ROOM_COUNT;
    level_height = (argc >= 3) ? strtoul(argv[2], NULL, 10) : DEFAULT_LEVEL_HEIGHT;
    level_width = (argc >= 4) ? strtoul(argv[3], NULL, 10) : DEFAULT_LEVEL_WIDTH;
    room_max = (argc >= 5) ? strtoul(argv[4], NULL, 10) : DEFAULT_ROOM_MAX;
    room_min = (argc >= 6) ? strtoul(argv[5], NULL, 10) : DEFAULT_ROOM_MIN;

    level = allocLevel(level_height, level_width);
    rooms = (Level *) malloc(room_count * sizeof(Level));

    for (int i = 0; i < room_count; i++) {
        int y, x, size;
        size = randomNum(room_min, room_max);
        rooms[i] = readRoom(level_num, size);
    
        y = randomNum(0, level_height - size);
        x = randomNum(0, level_width - size);

        for (int row = 0; row < size; row++)
            for (int col = 0; col < size; col++) {
                int old = level[y+row][x+col];
                int new = rooms[i][row][col];

                if (new == '#') {
                    switch (old) {
                    case '#':
                        level[y+row][x+col] = '#';
                        break;
                    case ' ':
                        level[y+row][x+col] = '#';
                        break;
                    case '.':
                        level[y+row][x+col] = '.';
                        break;
                    case '/':
                        level[y+row][x+col] = '/';
                        break;
                    default:
                        level[y+row][x+col] = '&';
                        break;
                    }
                } else if (new == '.') {
                    switch (old) {
                    case '#':
                        level[y+row][x+col] = '.';
                        break;
                    case ' ':
                        level[y+row][x+col] = '.';
                        break;
                    case '.':
                        level[y+row][x+col] = '.';
                        break;
                    case '/':
                        level[y+row][x+col] = '.';
                        break;
                    default:
                        level[y+row][x+col] = '&';
                        break;
                    }
                } else if (new == '/') {
                    switch (old) {
                    case '#':
                        level[y+row][x+col] = '/';
                        break;
                    case '/':
                        level[y+row][x+col] = '/';
                        break;
                    case '.':
                        level[y+row][x+col] = '.';
                        break;
                    default:
                        level[y+row][x+col] = '/';
                        break;
                    }
                } else {
                    level[y+row][x+col] = new;
                }
            }

                // if ((rooms[i][row][col] == '#' || rooms[i][row][col] == '/') && level[y+row][x+col] == '#') {
                //     level[y+row][x+col] = '.';
                // } else {
                //     level[y+row][x+col] = (level[y+row][x+col] == '.') ? level[y+row][x+col] : rooms[i][row][col];
                // }
                
    }   

    doors = (Point *) malloc(room_count * 4 * sizeof(Point));
    doors_last = doors;
    for (int i = 0; i < level_height; i++)
        for (int j = 0; j < level_width; j++) 
            if (level[i][j] == '/') {
                (*doors_last)[0] = i;
                (*doors_last)[1] = j;
                doors_last++;
            }

    printf("%d\n", doors_last - doors);
    
    for (Point *door = doors; door < doors_last; door++) {
        int closestDistance = __INT_MAX__;
        Point *closest, *path;
        int length, *checked, *checked_last;
        free(checked);
        checked = (int *) malloc((doors_last-doors) * sizeof(int));
        checked_last = &checked[0];
        for (int i = 0; i < doors_last - doors; i++) {
            Point *beg;
            for (beg = doors+i; beg < doors_last; beg++) {
                int checked_exist;
                checked_exist = 0;
                for (int *checked_beg = checked; checked_beg < checked_last; checked_beg++)
                    if (beg == &doors[*checked_beg]) {
                        checked_exist = 1;
                        break;
                    }
                if (checked_exist == 0 && abs((*beg)[0] - (*door)[0]) + abs((*beg)[1] - (*door)[1]) < closestDistance && abs((*beg)[0] - (*door)[0]) + abs((*beg)[1] - (*door)[1]) > 0) {
                    
                    closestDistance = abs((*beg)[0] - (*door)[0]) + abs((*beg)[1] - (*door)[1]);
                    closest = beg;
                }
            }
            
            path = BFS(level, (*door)[0], (*door)[1], (*closest)[0], (*closest)[1], level_height, level_width, &length);
            if (length > 0) {
                for (int i = 0; i < length; i++)
                    level[path[i][0]][path[i][1]] = '/';
                break;
            }
            *checked_last++ = abs(beg - doors_last);
            //! MAKE IT WORK BY PUTTING IT IN A LOOP TO TRY TO GET TO ANOTHER PATH! THIS WAY IT WILL CERTAINLY FIND ONE!!!!
            //! ALSO ANTOHER IMPORTANT THING NEED TO DO IS TO FIX SO THAT DOORS CANNOT SPAWN INDOORS....


            //!
            //! BIIIIG PROBLEM! NEED to connect everything to the main room. Need to reconsider the pathing. Its not just about connecting all of the rooms but
            //! I also need to connect them to a main room and make sure the exit room is working as it should...   
            //! THIS IS BAAAAD!!!! 
            //!
        }
    }


    for (int i = 0; i < level_height; i++) {
        printf("%s\n", level[i]);
    }
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

inline unsigned int
randomNum(register unsigned int min, register unsigned int max)
{
    return (rand() % (max - min + 1)) + min;
}

Level
readRoom(const unsigned int num, const unsigned int size)
{
    char *room_filestr;
    room_filestr = (char *) malloc(50 * sizeof(char));
    sprintf(room_filestr, "rooms/%d/%da", num, size);
    return readLevelFromFile(room_filestr, size+2, size+2);
}

Level
readLevelFromFile(const char *f, const unsigned int height, const unsigned int width)
{
    FILE *fp;
    char *emptyRow, **level;

    fp = fopen(f, "r");
    if (fp == NULL) {
        printf("Failed to open file \"%s\" reason:\n%s\n", f, strerror(errno));
        return NULL;
    }

    emptyRow = (char *) calloc(width + 1, sizeof(char));
    for (int i = 0; i < width; i++)
        emptyRow[i] = ' ';
    emptyRow[width] = '\0';

    level = (char **) calloc(height, sizeof(char *));
    for (int i = 0; i < height; i++) {
        level[i] = (char *) calloc(width + 1, sizeof(char));
        if (!ferror(fp) && !feof(fp)) {
            char *nl;
            fgets(level[i], width, fp);
            if ((nl = strchr(level[i], '\n')) != NULL)
                *nl = ' ';
            strncat(level[i], emptyRow, width - strlen(level[i]));
            level[i][width] = '\0';
        } else {
            strcpy(level[i], emptyRow);
        }
    }
    return level;
}

int 
rectCollision(const register int t1, const register int h1, const register int t2, const register int h2, const register int l1, const register int w1, const register int l2, const register int w2)
{
    return (t1 + h1 > t2 && t1 < t2 + h2 && l2 < l1 + w1 && l2 + w2 > l1);
}

typedef struct Node {
    int y, x;
    struct Node *parent;
    int has_parent;
} Node;

Node *solve(Level level, Node *s, Node *e, const unsigned int level_height, const unsigned int level_width);
int reconstructPath(Point *path, Node *s, Node *e, Node *l);
Node *createNode(const int y, const int x, Node *parent);
Node **allocQueue(size_t size);
void enqueue(Node *n);
Node *dequeue(void);
int is_queue_empty(void);

Point *
BFS(Level level, const int ystart, const int xstart, const int yend, const int xend, const unsigned int level_height, const unsigned int level_width, int *length)
{
    Node *start, *last, *end;
    Point *path;
    end = createNode(yend, xend, NULL);
    start = createNode(ystart, xstart, NULL);
    last = solve(level, start, end, level_height, level_width);

    path = (Point *) malloc(level_height * level_width * sizeof(Point));
    *length = reconstructPath(path, start, end, last);
    return path;
}

Node *
solve(Level level, Node *s, Node *e, const unsigned int level_height, const unsigned int level_width)
{
    Node *node, **neigbhours;
    int neighbour_count, **visited;
    allocQueue(level_height * level_width * sizeof(Node *));
    enqueue(s);

    visited = (int **) malloc(level_height * sizeof(int *));
    for (int i = 0; i < level_height; i++)
        visited[i] = (int *) calloc(level_width, sizeof(int));
    visited[s->y][s->x] = 1;

    // printf("----==== Beginning solve at: %d %d ====----\n", s->y, s->x);

    while (!is_queue_empty()) {
        node = dequeue();
        //printf("Searching node: %d %d\n", node->y, node->x);
        if (node->y == e->y && node->x == e->x) {
            // printf("\tFound target!\n");
            break;
        }
        if (level[node->y][node->x] == '/' && (node->y != s->y && node->y != s->x)) {
            // printf("\tFound valid path!\n");
            break;
        }
        neighbour_count = 0;
        neigbhours = (Node **) malloc(4 * sizeof(Node *));

        if (node->x < level_width-1)
            if (level[node->y][node->x+1] == ' ' || level[node->y][node->x+1] == '/')
                neigbhours[neighbour_count++] = createNode(node->y, node->x+1, node);
        if (node->x > 0)
            if (level[node->y][node->x-1] == ' ' || level[node->y][node->x-1] == '/')
                neigbhours[neighbour_count++] = createNode(node->y, node->x-1, node);

        if (node->y < level_height-1)
            if (level[node->y+1][node->x] == ' ' || level[node->y+1][node->x] == '/')
                neigbhours[neighbour_count++] = createNode(node->y+1, node->x, node);

        if (node->y > 0)
            if (level[node->y-1][node->x] == ' ' || level[node->y-1][node->x] == '/')
                neigbhours[neighbour_count++] = createNode(node->y-1, node->x, node);

        for (int i = 0; i < neighbour_count; i++) {
            if (visited[neigbhours[i]->y][neigbhours[i]->x] == 0) {
                // printf("\t::Found neighbouring %d %d\n", neigbhours[i]->y, neigbhours[i]->x);
                enqueue(neigbhours[i]);
                visited[neigbhours[i]->y][neigbhours[i]->x] = 1;
            }
        }
    }

    return node;
}

int 
reconstructPath(Point *path, Node *s, Node *e, Node *l)
{
    Node *node;
    int i = 0;

    if (l->y != e->y || l->x != e->x)
        return -1;
    
    for (node = l; node->has_parent == 1; i++) {
        Node *parent = node->parent;
        path[i][0] = node->y;
        path[i][1] = node->x;
        node = parent;
    }

    // for (Point *beg = path, *end = &path[i]; beg < end; beg++, end--) {
    //     int ytemp, xtemp;
    //     ytemp = (*beg)[0];
    //     xtemp = (*beg)[1];
    //     (*beg)[0] = (*end)[0];
    //     (*beg)[1] = (*end)[1];
    //     (*end)[0] = ytemp;
    //     (*end)[1] = xtemp;
    // }
    return i;
}

Node *
createNode(const int y, const int x, Node *parent)
{
    Node *ret = (Node *) malloc(sizeof(Node));
    ret->y = y;
    ret->x = x;
    ret->parent = parent;
    ret->has_parent = (parent == NULL) ? 0 : 1;
    return ret;
}

size_t queue_size;
Node **queue;
Node **queue_end;

Node **
allocQueue(size_t size)
{
    free(queue);
    queue = (Node **) malloc(size);
    queue_size = size;
    return queue_end = queue;
}

void
enqueue(Node *n)
{
    if (queue_end >= &queue[queue_size-1])
        queue = (Node **) realloc(queue, queue_size * 2);
    *queue_end++ = n;
}

Node *
dequeue(void)
{
    return (queue == queue_end) ? NULL : *queue++;
}

int 
is_queue_empty(void)
{
    return (queue == queue_end);
}