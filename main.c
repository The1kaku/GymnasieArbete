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

Actor player;
char command[COMMAND_SIZE];
char *cmdptr = command;

void displayActor(const Actor *a);
void moveActor(Actor *a, const int dy, const int dx);
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

	player.y = 0;
	player.x = 0;
	player.symbol = '@';
	player.health = 20;

	while (1) {
		while (getInput() < 0)
			;
		clear();
		displayActor(&player);
		refresh();
	}

	printw("PRESS ANY KEY TO EXIT");
	getch();
	endwin();

	return 0;
}

void displayActor(const Actor *a)
{
	printw("Y:%d\tX:%d\nSYM:%c\nHP:%d\n", a->y, a->x, a->symbol, a->health);
}

void moveActor(Actor *a, const int dy, const int dx)
{
	a->y += dy;
	a->x += dx;
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
