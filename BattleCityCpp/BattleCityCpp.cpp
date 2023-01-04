#include "Game.h"

int main()
{
	bool restart = false;
	Game gm;

	do
	{
		gm.RunWorld(restart);
	} while (restart);

	return 0;
}
