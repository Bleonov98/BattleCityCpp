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
	int mX = COLS / 2 - 3, startPosY = ROWS / 2, endPosY = startPosY + 4;
	int mY = startPosY;
	bool choose = false;
	do
	{
		SetPos(COLS / 2, startPosY);
		cout << "SINGLEPLAYER";
		SetPos(COLS / 2, startPosY + 2);
		cout << "MULTIPLAYER";
		SetPos(COLS / 2 + 4, startPosY + 4);
		cout << "EXIT";
		Sleep(250);
		while (true) {
			if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
				if (mY == startPosY) {
					choose = true;
					singlePlayer = true;
				}
				else if (mY == startPosY + 2) singlePlayer = false;
				else if (mY == startPosY + 4) {
					worldIsRun = false;
					exit = true;

					SetPos(mX, startPosY);
					cout << "                    ";
					SetPos(mX, startPosY + 2);
					cout << "                    ";
					SetPos(mX, startPosY + 4);
					cout << "                    ";

					return;
				}
				break;
			}
			SetPos(mX, mY);
			cout << "  ";
			if ((GetAsyncKeyState(VK_UP) & 0x8000) && mY > startPosY) mY -= 2;
			if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && mY < endPosY) mY += 2;
			SetPos(mX, mY);
			cout << "->";
			Sleep(60);
		}
		SetPos(mX, startPosY);
		cout << "                    ";
		SetPos(mX, startPosY + 2);
		cout << "                    ";
		SetPos(mX, startPosY + 4);
		cout << "                    ";
		Sleep(250);
		if (!singlePlayer) {
			SetPos(COLS / 2, startPosY);
			cout << "CREATE SERVER";
			SetPos(COLS / 2 + 1, startPosY + 2);
			cout << "JOIN SERVER";
			SetPos(COLS / 2 + 4, startPosY + 4);
			cout << "BACK";

			while (true) {
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
					if (mY == startPosY) {
						choose = true;
						create = true;
					}
					else if (mY == startPosY + 2) {
						joinS = true;
						choose = true;

						SetPos(mX, startPosY);
						cout << "                    ";
						SetPos(mX, startPosY + 2);
						cout << "                    ";
						SetPos(mX, startPosY + 4);
						cout << "                    ";

						Sleep(100);

						SetPos(COLS / 2 - 10, ROWS / 2);
						cout << "ENTER IP: ";
						FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
						cin >> ipAdd;
						SetPos(COLS / 2 - 10, ROWS / 2);
						cout << "          ";
					}
					break;
				}
				SetPos(mX, mY);
				cout << "  ";
				if ((GetAsyncKeyState(VK_UP) & 0x8000) && mY > startPosY) mY -= 2;
				if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && mY < endPosY) mY += 2;
				SetPos(mX, mY);
				cout << "->";
				Sleep(60);
			}
		}
		SetPos(mX, startPosY);
		cout << "                    ";
		SetPos(mX, startPosY + 2);
		cout << "                    ";
		SetPos(mX, startPosY + 4);
		cout << "                    ";
	} while (!choose);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();
	term.SetConsoleFont();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
	MainMenu();
	Preparing();
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		SetPos(COLS / 3 + 3, 24);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(COLS / 3 + 12, 24);
		cout << "GAME OVER!";
	}
	SetPos(COLS / 3 + 8, 28);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
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
					printf(CSI "22;37m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::SetGridState()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i < 2 || i >= ROWS - 1 || j < 2 || j > COLS) {
				wData.grid[i][j] = 99;
				continue;
			}	
			wData.grid[i][j] = 0;
		}
	}
}

void Game::Preparing()
{
	SetGridState();
	CreateMap();

	characterList.resize(50);

	Sleep(100);
}

void Game::SpawnPlayer(int& objectID, int x, int y, int color)
{
	player = new Player(&wData, x, y, color);

	allObjectList.push_back(player);
	playerList.push_back(player);

	characterList[objectID] = player;
	player->SetID(objectID);
	objectID++;
}

void Game::CreateMap()
{
	wall = new Wall(&wData, COLS / 2, ROWS / 2, BrRed);

	while (true)
	{
		wall->EraseObject();
		wall->ChangeWallPos();
		wall->DrawObject();

		for (int i = 0; i < wallList.size(); i++)
		{
			wallList[i]->DrawObject();
		}

		DrawChanges();

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			wall->EraseObject();
			break;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			if (wall->SetWallPos()) {
				wallList.push_back(wall);
				wall = new Wall(&wData, COLS / 2, ROWS / 2, BrRed);
			}
		}

		Sleep(100);
	}
}

void Game::DrawToMem()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			characterList[bulletList[i]->GetOwner()]->ReloadGun();

			bulletList.erase(bulletList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			allObjectList.erase(allObjectList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}

	for (int i = 0; i < wallList.size(); i++)
	{
		if (wallList[i]->GetWallType() == ICE || wallList[i]->GetWallType() == GRASS) wallList[i]->DrawObject();
	}
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	int tick = 0, charID = 0;

	SpawnPlayer(charID, COLS / 2 - 15, ROWS - 4, Red);
	SpawnPlayer(charID, COLS / 2 + 15, ROWS - 4, Green);

	for (int i = 0; i < STRONGSHOT; i++)
	{
		playerList[0]->PowerUP();
	}

	if (singlePlayer) {
		while (worldIsRun) {

			for (int i = 0; i < playerList.size(); i++)
			{
				if (tick % playerList[i]->GetSpeed() == 0) {

					playerList[i]->MoveObject();

					if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
						playerList[i]->Shot(allObjectList, bulletList, bullet, PLAYER);
					}

				}
			}

			for (int i = 0; i < bulletList.size(); i++)
			{
				if (tick % bulletList[i]->GetSpeed() == 0) {
					bulletList[i]->MoveObject();
				}
			}

			if (tick % 1000 == 0) {
				bonus = new Bonus(&wData, 2 + rand() % (COLS - 6), 2 + rand() % (ROWS - 5), BrYellow);
				bonus->SetBonusType(rand() % TIME);
				bonusList.push_back(bonus);
				allObjectList.push_back(bonus);
			}

			DrawToMem();

			DrawChanges();

			Sleep(15);

			tick++;
		}
	}
	else {
		Sleep(100);
	}

	DrawEndInfo(restart);

	printf(CSI "?1049l");
}