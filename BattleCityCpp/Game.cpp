#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					for (int i = 0; i < strnlen(area, (COLS+30)*ROWS); i++) {
						cout << area[i];
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::MainMenu()
{
	Sleep(100);
	int mX = COLS/2 - 3, mY = ROWS/2, SP = ROWS/2, MP = ROWS/2 + 2;

	SetPos(COLS / 2, ROWS / 2);
	cout << "SINGLEPLAYER";
	SetPos(COLS / 2, ROWS / 2 + 2);
	cout << "MULTIPLAYER";
	while (true) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			if (mY == SP) singlePlayer = true;
			else singlePlayer = false;
			break;
		}
		SetPos(mX, mY);
		cout << "  ";
		if (GetAsyncKeyState(VK_UP) & 0x8000) mY = SP;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) mY = MP;
		SetPos(mX, mY);
		cout << "->";
		Sleep(10);
	}

	SetPos(mX, SP);
	cout << "                    ";
	SetPos(mX, MP);
	cout << "                    ";
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();
	term.SetConsoleFont();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
	MainMenu();
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		SetPos(COLS / 2 - 1, 20);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(COLS / 2 + 6, 20);
		cout << "GAME OVER!";
	}

	SetPos(COLS / 2, 23);
	cout << "PRESS ENTER TO RESTART";
	SetPos(COLS / 2 + 2, 24);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawChanges()
{
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (prevBuf[y][x] == wData.vBuf[y][x]) {
				continue;
			}
			else {
				prevBuf[y][x] = wData.vBuf[y][x];

				SetPos(x, y);

				if ((prevBuf[y][x] >> 8) == Red) {
					printf(CSI "22;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "22;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "22;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "22;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "22;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "22;36m");
				}
				else if ((prevBuf[y][x] >> 8) == BrRed) {
					printf(CSI "22;91m");
				}
				else if ((prevBuf[y][x] >> 8) == BrGreen) {
					printf(CSI "22;92m");
				}
				else if ((prevBuf[y][x] >> 8) == BrBlue) {
					printf(CSI "22;94m");
				}
				else if ((prevBuf[y][x] >> 8) == BrCyan) {
					printf(CSI "22;96m");
				}
				else if ((prevBuf[y][x] >> 8) == BrPurple) {
					printf(CSI "22;95m");
				}
				else if ((prevBuf[y][x] >> 8) == BrYellow) {
					printf(CSI "22;93m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::DrawToMem()
{
	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	int tick = 0;

	if (singlePlayer) {
		while (worldIsRun) {

			DrawToMem();

			DrawChanges();

			Sleep(60);

			tick++;
		}
	}
	else {
		Sleep(100);
	}

	DrawEndInfo(restart);

	printf(CSI "?1049l");
}