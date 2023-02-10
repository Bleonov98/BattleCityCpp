#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(int& button)
{
	while (true) {
		if (GetAsyncKeyState(VK_UP) & 0x8000) button = UPKEY;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) button = RIGHTKEY;
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) button = DOWNKEY;
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000) button = LEFTKEY;
		else if (GetAsyncKeyState(VK_SPACE) & 0x8000) button = SPACEKEY;
		else if (GetAsyncKeyState(VK_RETURN) & 0x8000) button = RETURNKEY;
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) button = ESCKEY;
		else button = NOKEY;

		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void Game::DrawMovie()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	int mRows = 31, mCols = 122, currentX = 1, currentY = 1;

	int filesCnt = 22;
	LPSTR prevMovie;

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

	for (int resId = 110; resId < 110 + filesCnt; resId++)
	{
		HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(resId), L"TEXT"); // find res by id (IDR_TEXT3 to IDR_TEXT33)

		if (hResource)
		{
			HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

			if (hLoadedResource)
			{
				LPSTR movie = (LPSTR)LockResource(hLoadedResource);

				if (movie)
				{
					DWORD dwResourceSize = SizeofResource(hInstance, hResource);

					if (0 != dwResourceSize)
					{
						for (int i = 0; i < strnlen(movie, 120 * 32); i++) {

							if (currentX > mCols) currentY++, currentX = 1;

							SetPos(currentX, currentY);
							currentX++;

							if (resId)
							//if (movie[i] == prevMovie[i]) continue;
							cout << movie[i];
						}
						prevMovie = movie;
						currentY = 1, currentX = 1;
					}
				}
			}
		}
		Sleep(150);
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	system("cls");
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

	DrawMovie();
	DrawArea();
	MainMenu();
	Preparing();
}

void Game::DrawEndInfo(bool& restart, int &button)
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
	SetPos(COLS / 3 + 8, 30);
	cout << "PRESS R TO RESTART";

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			restart = false;
			break;
		}
		else if (GetAsyncKeyState(0x52) & 0x8000) {
			restart = true;
			break;
		}
	}
	
}

void Game::DrawInfo()
{
	if (playerList.empty()) return;
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	cout << playerList[0]->GetLifes();
	SetPos(COLS + 10, 8);
	cout << "LEFT: " << enemyCnt;
}

void Game::DrawChanges()
{
	do
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
		DrawInfo();
	} while (worldIsRun);
	
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
	Player* player = new Player(&wData, x, y, color);

	allObjectList.push_back(player);
	playerList.push_back(player);

	characterList[objectID] = player;
	player->SetID(objectID);
	objectID++;
}

void Game::SpawnEnemy(int& objectID, int x, int y)
{
	Enemy* enemy = new Enemy(&wData, x, y, BrCyan);

	characterList[objectID] = enemy;
	enemy->SetID(objectID);
	enemy->SetType(rand() % (ARMORED + 1));
	objectID++;
	enemyCnt--;

	if (enemyCnt % 3 == 0) enemy->SetBonusKeeper();

	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::SpawnBonus()
{
	Bonus* bonus = new Bonus(&wData, 2 + rand() % (COLS - 6), 2 + rand() % (ROWS - 5), BrYellow);
	bonus->SetBonusType(rand() % (TIME + 1));
	bonusList.push_back(bonus);
	allObjectList.push_back(bonus);
}

void Game::BonusCollision()
{
	for (int i = 0; i < playerList.size(); i++)
	{
		for (int j = 0; j < bonusList.size(); j++)
		{

			for (int pHeight = 0; pHeight < playerList[i]->GetHeight(); pHeight++)
			{
				for (int pWidth = 0; pWidth < playerList[i]->GetWidth(); pWidth++)
				{

					for (int bHeight = 0; bHeight < bonusList[j]->GetHeight(); bHeight++)
					{
						for (int bWidth = 0; bWidth < bonusList[j]->GetWidth(); bWidth++)
						{

							if (playerList[i]->GetX() + pWidth == bonusList[j]->GetX() + bWidth
								&& playerList[i]->GetY() + pHeight == bonusList[j]->GetY() + bHeight) {
								playerList[i]->PickBonus(bonusList[j], enemyList);
								return;
							}
						}
					}
				}
			}
		}
	}
}

void Game::WallCollision()
{
	for (int wall = 0; wall < wallList.size(); wall++)
	{
		if (wallList[wall]->GetWallType() == WATER || wallList[wall]->GetWallType() == GRASS || wallList[wall]->GetWallType() == ICE) continue;

		for (int bullet = 0; bullet < bulletList.size(); bullet++)
		{
			for (int wlHeight = 0; wlHeight < wallList[wall]->GetHeight(); wlHeight++)
			{
				for (int wlWidth = 0; wlWidth < wallList[wall]->GetWidth(); wlWidth++)
				{
					if (wallList[wall]->GetY() + wlHeight == bulletList[bullet]->GetY() && wallList[wall]->GetX() + wlWidth == bulletList[bullet]->GetX()) {

						if (wallList[wall]->GetWallType() == BASE) {
							worldIsRun = false, gameEnd = true, win = false;
							PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);

							wallList[wall]->DeleteObject();

							return;
						}

						bulletList[bullet]->DeleteObject();
						bulletList[bullet]->EraseObject();

						wallList[wall]->DestroyWall(bulletList[bullet]->GetDirection(), bulletList[bullet]->GetBulletPower());
					}
				}
			}
		}

	}
}

void Game::BulletCollision()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		for (int j = 0; j < bulletList.size(); j++)
		{
			if (j == i) continue;
			if (bulletList[i]->GetOwner() == bulletList[j]->GetOwner()) continue;
			if  ( 
					( bulletList[i]->GetX() == bulletList[j]->GetX() && bulletList[i]->GetY() == bulletList[j]->GetY() ) ||
					( bulletList[i]->GetDirection() == UP && (bulletList[i]->GetX() == bulletList[j]->GetX() && bulletList[i]->GetY() - 1 == bulletList[j]->GetY()) ) || 
					( bulletList[i]->GetDirection() == BOT && (bulletList[i]->GetX() == bulletList[j]->GetX() && bulletList[i]->GetY() + 1 == bulletList[j]->GetY()) ) ||
					( bulletList[i]->GetDirection() == RIGHT && (bulletList[i]->GetX() + 1 == bulletList[j]->GetX() && bulletList[i]->GetY() == bulletList[j]->GetY()) ) ||
					( bulletList[i]->GetDirection() == LEFT && (bulletList[i]->GetX() - 1 == bulletList[j]->GetX() && bulletList[i]->GetY() == bulletList[j]->GetY()) ) 
				)
			{
				bulletList[i]->DeleteObject();
				bulletList[j]->DeleteObject();

				bulletList[i]->EraseObject();
				break;
			}
		}
	}

	bool brk = false;

	for (int bullet = 0; bullet < bulletList.size(); bullet++)
	{
		brk = false;
		for (int enemy = 0; enemy < enemyList.size(); enemy++)
		{
			if (bulletList[bullet]->GetOwner() != PLAYER) break;

			for (int eHeight = 0; eHeight < enemyList[enemy]->GetHeight(); eHeight++)
			{
				for (int eWidth = 0; eWidth < enemyList[enemy]->GetWidth(); eWidth++)
				{

					if (bulletList[bullet]->GetX() == enemyList[enemy]->GetX() + eWidth && bulletList[bullet]->GetY() == enemyList[enemy]->GetY() + eHeight) {
						enemyList[enemy]->Hit();
						bulletList[bullet]->DeleteObject();

						score += 25;
						if (enemyList[enemy]->BonusKeeper()) SpawnBonus();

						brk = true;
					}

					if (brk) break;
				}
				if (brk) break;
			}
			if (brk) break;
		}
	}

	for (int bullet = 0; bullet < bulletList.size(); bullet++)
	{
		brk = false;
		for (int player = 0; player < playerList.size(); player++)
		{
			if (bulletList[bullet]->GetOwner() == PLAYER) break;

			for (int pHeight = 0; pHeight < playerList[player]->GetHeight(); pHeight++)
			{
				for (int pWidth = 0; pWidth < playerList[player]->GetWidth(); pWidth++)
				{

					if (bulletList[bullet]->GetX() == playerList[player]->GetX() + pWidth && bulletList[bullet]->GetY() == playerList[player]->GetY() + pHeight) {
						playerList[player]->Hit();
						bulletList[bullet]->DeleteObject();

						brk = true;
					}
					if (brk) break;
				}
				if (brk) break;
			}
			if (brk) break;
		}
	}
}

void Game::CheckCollision()
{
	BonusCollision();
	WallCollision();
	BulletCollision();
}

void Game::CreateMap()
{
	if (level == 0) {
		SetWall(5, 5, BRICK);
		SetWall(5, 11, BRICK);
		SetWall(5, 17, BRICK);

		SetWall(14, 5, BRICK);
		SetWall(14, 11, BRICK);
		SetWall(14, 17, BRICK);

		SetWall(23, 5, BRICK);
		SetWall(23, 11, BRICK);
		SetWall(23, 17, BRICK);

		SetWall(32, 5, BRICK);
		SetWall(32, 11, BRICK);
		SetWall(32, 17, BRICK);

		SetWall(41, 5, BRICK);
		SetWall(41, 11, BRICK);
		SetWall(41, 17, BRICK);

		SetWall(50, 5, BRICK);
		SetWall(50, 11, BRICK);
		SetWall(50, 17, BRICK);

		SetWall(59, 5, BRICK);
		SetWall(59, 11, BRICK);
		SetWall(59, 17, BRICK);

		SetWall(68, 5, BRICK);
		SetWall(68, 11, BRICK);
		SetWall(68, 17, BRICK);

		SetWall(77, 5, BRICK);
		SetWall(77, 11, BRICK);
		SetWall(77, 17, BRICK);


		SetWall(5, 23, WATER);
		SetWall(11, 23, WATER);
		SetWall(17, 23, WATER);
		SetWall(23, 23, WATER);

		SetWall(59, 23, WATER);
		SetWall(65, 23, WATER);
		SetWall(71, 23, WATER);
		SetWall(77, 23, WATER);

		SetWall(83, 5, GRASS);
		SetWall(83, 11, GRASS);
		SetWall(83, 17, GRASS);
		SetWall(83, 23, GRASS);
		SetWall(83, 29, GRASS);
		SetWall(83, 35, GRASS);
		SetWall(83, 40, GRASS);


		SetWall(2, ROWS - 13, ICE);
		SetWall(8, ROWS - 13, ICE);
		SetWall(14, ROWS - 13, ICE);
		SetWall(20, ROWS - 13, ICE);
		SetWall(26, ROWS - 13, ICE);
		SetWall(32, ROWS - 13, ICE);
		SetWall(38, ROWS - 13, ICE);
		SetWall(44, ROWS - 13, ICE);
		SetWall(50, ROWS - 13, ICE);
		SetWall(56, ROWS - 13, ICE);
		SetWall(62, ROWS - 13, ICE);
		SetWall(68, ROWS - 13, ICE);

		SetWall(74, ROWS - 13, STEEL);
		SetWall(74, ROWS - 7, STEEL);

		SetWall(COLS / 2 - 1, ROWS - 4, BASE);
		SetWall(COLS / 2 - 4, ROWS - 6, BASEBRICK);
	}
	else if (level == 1) {

	}
}

void Game::ClearData()
{
	system("cls");

	DrawArea();
	SetGridState();

	allObjectList.clear();
	wallList.clear();
	playerList.clear();
	enemyList.clear();
	characterList.clear();
	bonusList.clear();
	bulletList.clear();

	characterList.resize(50);
}

void Game::SetWall(int x, int y, int type)
{
	Wall* wall = new Wall(&wData, x, y, BrRed);

	wallList.push_back(wall);
	wallList.back()->SetWallType(type);
	wallList.back()->SetWallPos();
}

void Game::DrawToMem()
{
	for (int i = 0; i < wallList.size(); i++)
	{
		wallList[i]->DrawObject();

		if (wallList[i]->IsObjectDelete()) {
			wallList.erase(wallList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < bonusList.size(); i++)
	{
		if (bonusList[i]->IsObjectDelete()) {
			bonusList.erase(bonusList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			characterList[bulletList[i]->GetOwner()]->ReloadGun();

			bulletList.erase(bulletList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->IsObjectDelete()) {
			vector <Enemy*>::iterator eIt;
			eIt = enemyList.begin() + i;
			enemyList.erase(enemyList.begin() + i);
			i = -1;
		}
	}

	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->IsObjectDelete()) {
			auto pIt = playerList.begin() + i;
			playerList.erase(pIt);
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
		if (wallList[i]->GetWallType() == GRASS) wallList[i]->DrawObject();
	}
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	int tick = 0, charID = 0, spawnTick = 1, button = NOKEY;

	thread pressKeys([&] {
		HotKeys(button);
		});
	pressKeys.detach();

	do
	{
		SpawnPlayer(charID, COLS / 2 - 15, ROWS - 4, Red);
		DrawToMem();

		thread drawing([&] {
			DrawChanges();
		});

		Sleep(1500);

		while (worldIsRun) {

			for (int i = 0; i < playerList.size(); i++)
			{
				if (tick % playerList[i]->GetSpeed() == 0) {
					playerList[i]->MoveObject(button);

					if (button == SPACEKEY) {
						playerList[i]->Shot(allObjectList, bulletList, bullet, PLAYER);
					}
				}
			} // player move

			if (enemyList.size() < 3 && tick % 95 == 0 && enemyCnt != 0) {
				int eX, eY;

				if (spawnTick == 1) eX = 2, eY = 2;
				else if (spawnTick == 2) eX = COLS / 2, eY = 2;
				else if (spawnTick == 3) eX = COLS - 4, eY = 2;

				SpawnEnemy(charID, eX, eY);
				spawnTick++;
				if (spawnTick > 3) spawnTick = 1;
			}

			for (int i = 0; i < enemyList.size(); i++)
			{
				if (tick % enemyList[i]->GetSpeed() == 0) enemyList[i]->MoveObject(NOKEY);
				if (enemyList[i]->CheckAhead()) enemyList[i]->Shot(allObjectList, bulletList, bullet, ENEMY);
			}

			for (int i = 0; i < bulletList.size(); i++)
			{
				if (tick % bulletList[i]->GetSpeed() == 0) {
					bulletList[i]->MoveObject(NOKEY);
				}
			} // bullet move

			CheckCollision();

			DrawToMem();

			Sleep(20);

			tick++;

			if (button == ESCKEY) worldIsRun = false;
			if (playerList.size() <= 0) worldIsRun = false, win = false, gameEnd = true;
			if (enemyCnt == 0 && enemyList.empty()) worldIsRun = false, win = true;
		}

		drawing.join();

		if (!gameEnd) {
			tick = 0, charID = 0, spawnTick = 1, enemyCnt = 9, win = false, worldIsRun = true, level++;

			ClearData();

			CreateMap();
		}

	} while (!gameEnd);

	DrawEndInfo(restart, button);

	printf(CSI "?1049l");
}