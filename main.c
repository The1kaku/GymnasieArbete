#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_MONSTERS 10
#define COMMAND_SIZE 1000
#define COMMAND_BUF_SIZE 100

enum KEYS {
	K_UP = 'w',
	K_RIGHT = 's',
	K_LEFT = 'a',
	K_DOWN = 'r',
	K_EXIT = 'e',
	K_COMMAND = '/'
};

enum LEVEL {
	L_WIDTH = 80,
	L_HEIGHT = 80
};

typedef char Level[L_HEIGHT][L_WIDTH];

typedef struct {
	int y;
	int x;
	int height;
	int width;
} Camera;

typedef struct {
	char *str;
	int (*fun)(int);
	int time;
} Command;


typedef struct {
	int health;
	int symbol;
	int y;
	int x;
} Actor;

int EXIT_CODE = 0;

Actor player = { .health = 20, .symbol = '@', .y = 0, .x = 0 };
Actor *activeMonsters[MAX_MONSTERS];
char command[COMMAND_SIZE];
char *cmdptr = command;
Level activeLevel;
Camera activeCamera = { .y = 0, .x = 0, .height = 20, .width = 20 };

WINDOW* levelWin;
WINDOW* playerWin;

void displayLevel(const Level, const Camera);
void displayPlayer(const Actor *a);
void displayActor(const Actor *a);
void displayMonsters(void);
void moveActor(Actor *a, const int dy, const int dx);
int collideActor(const Actor *a, const int dy, const int dx);
int **getRoomBorders(const Actor *a);
int getInput(void);
int readLevelFromFile(const char *file);
char *sgetword(char *str);
int parseCommand(const char *str);
int exitCommand(int time);
int moveCommand(int time);
int processAis(int time);

Command C_EXIT = { "exit", &exitCommand, 0 };
Command C_MOVE = { "move", &moveCommand, 10 };

Command activeCommands[COMMAND_BUF_SIZE];
Command *end = &activeCommands[0];

int main(void) 
{
	int time;
	initscr();
	cbreak();
	noecho();

	endwin();
	*end++ = C_EXIT;
	*end++ = C_MOVE;

	if (readLevelFromFile("level.txt") < 0) {
		printf("Error reading file");
		return -1;
	}

	levelWin = newwin(20, 20, 4, 3);
	playerWin = newwin(3, 20, 0, 0);

	Actor undead = { .health = 20, .symbol = 'U', .y = 5, .x = 5 };
	activeMonsters[0] = &undead;

	while (1) {
		while ((time = getInput()) < 0)
			;
		processAis(time);
		wclear(levelWin);
		wclear(playerWin);
		displayLevel(activeLevel, activeCamera);
		displayMonsters();
		displayPlayer(&player);
		wrefresh(levelWin);
		wrefresh(playerWin);
	}

	return 0;
}

void displayPlayer(const Actor *a)
{
	int **room = getRoomBorders(a);
	wprintw(playerWin, "Y:%d\tX:%d\tROOM:{%d,%d}to{%d,%d}\nSYM:%c\nHP:%d\tCOL:%c\n", a->y, a->x, room[0][0], room[0][1], room[1][0], room[1][1], a->symbol, a->health, collideActor(a, 0, 0));
	displayActor(a);
}

void displayActor(const Actor *a)
{
	mvwaddch(levelWin, a->y - activeCamera.y, a->x - activeCamera.x, a->symbol);
}

void displayLevel(const Level l, const Camera c)
{
	int i, j;
	for (i = 0; i < c.y; i++)
		;
	wmove(levelWin, 0, 0);
	for (; i < c.y + c.height; i++) {
		for (j = 0; j < c.x; j++)
			;
		for (; j < c.x + c.width; j++)
			waddch(levelWin, l[i][j]);
	}
}

void displayMonsters(void)
{
	for (int i = 0; i < MAX_MONSTERS; i++)
		if (activeMonsters[i] != NULL && isalpha(activeMonsters[i]->symbol)) {
			displayActor(activeMonsters[i]);
		}
}

void moveActor(Actor *a, const int dy, const int dx)
{
	if (collideActor(a, dy, dx) == 'f')
		return;
	a->y += dy;
	a->x += dx;
}

int collideActor(const Actor *a, const int dy, const int dx)
{
	return activeLevel[a->y + dy][a->x + dx];
}

int **getRoomBorders(const Actor *a)
{
	int **ret;
	ret = (int **) malloc(2 * sizeof(int*));
	for (int i = 0; i < 2; i++)
		ret[i] = (int *) malloc(2 * sizeof(int));

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			ret[i][j] = -1;
	if (activeLevel[a->y][a->x] != '.')
		return ret;

	int i, j;
	for (i = a->y; i > 0; i--)
		if (activeLevel[i][a->x] != '.') {
			--i;
			break;
		}
	for (j = a->x; j > 0; j--)
		if (activeLevel[a->y][j] != '.') {
			--j;
			break;
		}
	ret[0][0] = i;
	ret[0][1] = j;

	for (i = a->y; i < L_HEIGHT; i++)
		if (activeLevel[i][a->x] != '.') {
			break;
		}
	for (j = a->x; j < L_WIDTH; j++)
		if (activeLevel[a->y][j] != '.') {
			break;
		}
	ret[1][0] = i;
	ret[1][1] = j;

	return ret;
}

int getInput(void)
{
	int t, c = getch();
	char cmd[COMMAND_SIZE];
	t = 0;
	
	switch(c) {
	case K_UP:
		strcpy(cmd, "up 1");
		cmdptr = cmd;
		t = moveCommand(C_MOVE.time);
		cmdptr = command;
		return t;
	case K_RIGHT:
		strcpy(cmd, "right 1");
		cmdptr = cmd;
		t = moveCommand(C_MOVE.time);
		cmdptr = command;
		return t;
	case K_LEFT:
		strcpy(cmd, "left 1");
		cmdptr = cmd;
		t = moveCommand(C_MOVE.time);
		cmdptr = command;
		return t;
	case K_DOWN:
		strcpy(cmd, "down 1");
		cmdptr = cmd;
		t = moveCommand(C_MOVE.time);
		cmdptr = command;
		return t;
	case K_EXIT:
		exitCommand(0);
		return t;
	case K_COMMAND:
		nocbreak();
		echo();
		addch('/');
		getstr(command);
		t = parseCommand(sgetword(cmdptr = command));
		noecho();
		cbreak();
		return t;
	default:
		return 0;
	}
}

int readLevelFromFile(const char *file)
{
	FILE *in = fopen(file, "r");
	char *emptyRow = (char *) calloc(L_WIDTH, sizeof(char));
	for (int i = 0; i < L_WIDTH; i++)
		emptyRow[i] = ' ';

	if (in == NULL)
		return -1;

	for (int i = 0; i < L_HEIGHT; i++) {
		if (fgets(&activeLevel[i][0], L_WIDTH, in) != NULL) {
			strncpy(strchr(activeLevel[i], '\n'), emptyRow, L_WIDTH - strlen(activeLevel[i]));
		} else {
			strcpy(activeLevel[i], emptyRow);
		}
	}
	return 0;
}

char *sgetword(char *str)
{
	char *begin;
	char *end;
	for (begin = str; isspace(*begin); ++begin)
		;

	for (end = begin; !isspace(*end); ++end)
		;

	*end = '\0';
	
	cmdptr = ++end;
	return begin;
}

int parseCommand(const char *str)
{
	Command *begin;
	for (begin = &activeCommands[0]; begin != end; begin++)
		if (strcmp(str, begin->str) == 0) {
			cbreak();
			return (begin->fun)(begin->time);
		}
	return -1;
}

int exitCommand(int time)
{
	endwin();
	exit(EXIT_CODE);
}

int moveCommand(int time)
{
	int delta = 0;
	int dir;
	char *directions[4] = {
		"left",
		"right",
		"up",
		"down"
	};

	char *dirstr = sgetword(cmdptr);
	for (dir = 0; dir < 4; dir++) {
		if (strcmp(dirstr, directions[dir]) == 0) {
			delta = 1;
			break;
		}
	}

	char *numstr = sgetword(cmdptr);
	sscanf(numstr, "%d", &delta);

	if (dir < 4 && delta != 0)
		switch (dir) {
		case 0:
			moveActor(&player, 0, -delta);
			break;
		case 1:
			moveActor(&player, 0, delta);
			break;
		case 2:
			moveActor(&player, -delta, 0);
			break;
		case 3:
			moveActor(&player, delta, 0);
			break;
		}

	return delta * time;
}

int processAi(Actor *a, int time);

int processAis(int time)
{
	int **room = getRoomBorders(&player);
	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (activeMonsters[i] == NULL)
			continue;
		if (activeMonsters[i]->y > room[0][0] && activeMonsters[i]->y < room[1][0] && activeMonsters[i]->x > room[0][1] && activeMonsters[i]->x < room[1][0])
			processAi(activeMonsters[i], time);
	}
	return 0;
}

int processAi(Actor *a, int time)
{
	int dy, dx;
	dy = player.y - a->y;
	dx = player.x - a->x;
	if (abs(dy) + abs(dx) <= 1)
		return 0;
	if (abs(dy) > abs(dx))
		moveActor(a, (dy > 0) ? 1 : -1, 0);
	else 
		moveActor(a, 0, (dx > 0) ? 1 : -1);
	return 0;
}
