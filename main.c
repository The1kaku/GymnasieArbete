#include <curses.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
char command[COMMAND_SIZE];
char *cmdptr = command;
Level activeLevel;
Camera activeCamera = { .y = 5, .x = 5, .height = 20, .width = 20 };

WINDOW* levelWin;
WINDOW* playerWin;

void displayLevel(const Level, const Camera);
void displayPlayer(const Actor *a);
void moveActor(Actor *a, const int dy, const int dx);
int collideActor(const Actor *a, const int dy, const int dx);
int getInput(void);
char *sgetword(char *str);
int parseCommand(const char *str);
int exitCommand(int time);
int moveCommand(int time);

Command C_EXIT = { "exit", &exitCommand, 0 };
Command C_MOVE = { "move", &moveCommand, 10 };

Command activeCommands[COMMAND_BUF_SIZE];
Command *end = &activeCommands[0];

int main(void) 
{
	initscr();
	cbreak();
	noecho();

	*end++ = C_EXIT;
	*end++ = C_MOVE;

	for (int i = 0; i < L_HEIGHT; i++)
		for (int j = 0; j < L_WIDTH; j++)
			activeLevel[i][j] = 'a' + i;
	levelWin = newwin(20, 20, 4, 3);
	playerWin = newwin(3, 20, 0, 0);

	while (1) {
		while (getInput() < 0)
			;
		wclear(levelWin);
		wclear(playerWin);
		displayLevel(activeLevel, activeCamera);
		displayPlayer(&player);
		wrefresh(levelWin);
		wrefresh(playerWin);
	}

	return 0;
}

void displayPlayer(const Actor *a)
{
	wprintw(playerWin, "Y:%d\tX:%d\nSYM:%c\nHP:%d\tCOL:%c\n", a->y, a->x, a->symbol, a->health, collideActor(a, 0, 0));
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

int getInput(void)
{
	int c = getch();
	
	switch(c) {
	case K_UP:
		moveActor(&player, -1, 0);
		break;
	case K_RIGHT:
		moveActor(&player, 0, 1);
		break;
	case K_LEFT:
		moveActor(&player, 0, -1);
		break;
	case K_DOWN:
		moveActor(&player, 1, 0);
		break;
	case K_EXIT:
		exitCommand(0);
		break;
	case K_COMMAND:
		nocbreak();
		echo();
		addch('/');
		getstr(command);
		parseCommand(sgetword(cmdptr = command));
		noecho();
		cbreak();
		break;
	default:
		return -1;
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
