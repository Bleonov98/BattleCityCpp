#include "Game.h"

int main()
{

	bool restart;

	do
	{
		restart = false;

		Game gm;
		gm.RunWorld(restart);
		
		Sleep(500);
	} while (restart);

	return 0;
}
