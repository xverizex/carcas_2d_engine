#include "game.h"
#include <clocale>

int main (int argc, char **argv)
{
	Game *game = new Game ();
	game->init ();
	game->loop ();
}
