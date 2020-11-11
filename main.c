#include <curses.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND_SIZE 1000

enum KEYS {
	K_UP = 'w',
	K_RIGHT = 's',
	K_LEFT = 'a',
	K_DOWN = 'r',
	K_EXIT = 'e',
	K_COMMAND = '/'
};

typedef struct {
	int health;
	int symbol;
	int y;
	int x;
} Actor;

Actor player;
char command[COMMAND_SIZE];

void displayActor(const Actor *a);
void moveActor(Actor *a, int dy, int dx);
int getInput();
void parseCommand(const char *str);

int main() 
{
	initscr();
	raw();

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

void moveActor(Actor *a, int dy, int dx)
{
	a->y += dy;
	a->x += dx;
}

int getInput()
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
		exit(0);
		break;
	case K_COMMAND:
		getstr(command);
		parseCommand(command);
		break;
	default:
		return -1;
	}
	return 0;
}

void parseCommand(const char *str)
{
	if (strcmp(str, "exit") == 0)
		exit(0);
}
