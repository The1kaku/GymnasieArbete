#include <curses.h>

#include "main.h"
#include "display.h"
#include "input.h"
#include "commands.h"

int 
main(void)
{
    player = createActor(1, 1, 20, '@', 1);
    level = readLevelFromFile("levels/level.txt");

    if (level == NULL) {
        return -1;
    }

    initDisplay();
    endwin();

    setCamera(0, 0);

    do {
        clearDisplay();
        focusCameraOnActor(player);
        addLevel(level);
        addActor(player);
        addCamera();
        refreshDisplay();
    } while (readInput(player) >= 0);

    deleteDisplay();
    endwin();

    return 0;
}