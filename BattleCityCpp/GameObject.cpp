#include "GameObject.h"

// ---------------------------------------------------------------------

int GameObject::GetX()
{
	return _x;
}

int GameObject::GetY()
{
	return _y;
}

int GameObject::GetSpeed()
{
	return _speed;
}

int GameObject::GetWidth()
{
	return _width;
}

int GameObject::GetHeight()
{
	return _height;
}

void GameObject::SetX(int x)
{
	_x = x;
}

void GameObject::SetY(int y)
{
	_y = y;
}

void GameObject::DeleteObject()
{
	_deleteObject = true;
}

bool GameObject::IsObjectDelete()
{
	return _deleteObject;
}


// - - - - - - - - - - - - - DYNAMIC OBJECTS - - - - - - - - - - - - - - - -


int DynamicObject::GetDirection()
{
	return _dir;
}

void DynamicObject::SetDirection(int dir)
{
	_dir = dir;
}

void DynamicObject::CheckNextStep()
{
	multimap<bool, pair<int, int>> iceCrd;
	int checkInt = 0;

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (wData->grid[_y + i][_x + j] == ICE) iceCrd.emplace(true, make_pair(_y + i, _x + j));
			else iceCrd.emplace(false, make_pair(_y + i, _x + j));
		}
	}

	auto it = iceCrd.find(true);

	if (it != iceCrd.end()) _speed = 6;
	else _speed = _nativeSpeed;

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_dir == UP && (wData->grid[_y][_x + j] > BASE || wData->grid[_y][_x + j] == BRICK || wData->grid[_y][_x + j] == WATER || wData->grid[_y][_x + j] == STEEL
				|| wData->grid[_y][_x + j] == BASE)) _y++;
			else if (_dir == RIGHT && (wData->grid[_y + i][_x + _width] > BASE || wData->grid[_y + i][_x + _width - 1] == BRICK || wData->grid[_y + i][_x + _width - 1] == WATER
				|| wData->grid[_y + i][_x + _width - 1] == STEEL || wData->grid[_y + i][_x + _width - 1] == BASE)) _x--;
			else if (_dir == BOT && (wData->grid[_y + _height - 1][_x + j] > BASE || wData->grid[_y + _height - 1][_x + j] == BRICK || wData->grid[_y + _height - 1][_x + j] == WATER
				|| wData->grid[_y + _height - 1][_x + j] == STEEL || wData->grid[_y + _height - 1][_x + j] == BASE)) _y--;
			else if (_dir == LEFT && (wData->grid[_y + i][_x] > BASE || wData->grid[_y + i][_x] == BRICK || wData->grid[_y + i][_x] == WATER || wData->grid[_y + i][_x] == STEEL
				|| wData->grid[_y + i][_x] == BASE)) _x++;
		}
	}
}   


// - - - - - - - - - - - - - - -  CHARACTERS - - - - - - - - - - - - - - - -


void Character::Shot(vector <GameObject*>& allObjList, vector <Bullet*>& bulletList, Bullet* bullet, int ownerType)
{
	if (_ammo == 0) {
		return;
	}
	_ammo--;

	if (_gunType <= SINGLESHOT) {
		bullet = new Bullet(wData, _x + _width / 2, _y + _height / 2, Red);
		bullet->SetDirection(_dir);
		bullet->SetBulletPower(STANDART, 2);
		allObjList.push_back(bullet);
		bulletList.push_back(bullet);
	}
	else if (_gunType >= FASTSHOT && _gunType < STRONGSHOT) {
		bullet = new Bullet(wData, _x + _width / 2, _y + _height / 2, Red);
		bullet->SetDirection(_dir);
		bullet->SetBulletPower(STANDART, 1);
		allObjList.push_back(bullet);
		bulletList.push_back(bullet);
	}
	else if (_gunType >= STRONGSHOT) {
		bullet = new Bullet(wData, _x + _width / 2, _y + _height / 2, Red);
		bullet->SetDirection(_dir);
		bullet->SetBulletPower(HIGH, 1);
		allObjList.push_back(bullet);
		bulletList.push_back(bullet);
	}
	bullet->SetOwner(GetID(), ownerType);
}

void Character::SetType(int type)
{
	_type = type;

	if (_type == ARMORED) _hp = 50;
}

int Character::GetType()
{
	return _type;
}

int Character::GetHp()
{
	return _hp;
}

int Character::GetGunType()
{
	return _gunType;
}

void Character::ReloadGun()
{
	_ammo++;
}

void Character::Hit()
{
	_hp -= 25;
	if (_hp <= 0) {
		Death();
	}
}

void Character::Death()
{
	EraseObject();
	DeleteObject();
}

void Character::SetID(int id)
{
	_id = id;
}

int Character::GetID()
{
	return _id;
}

// ----------- ENEMY -------------

void Enemy::DrawObject()
{
	if (_type == REGULAR) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = regularSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTEREN;
			}
		}
	}
	else if (_type == FAST) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = midSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTEREN;
			}
		}
	}
	else if (_type == ARMORED) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = armoredSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTEREN;
			}
		}
	}

	if (_keepBonus) {
		if (_bonusTick % 10 == 0) _color = Purple;
		else if (_bonusTick % 5 == 0) _color = _nativeColor;

		_bonusTick++;
	}
}

void Enemy::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u' ';
			wData->grid[_y + i][_x + j] = 0;
		}
	}
}

void Enemy::MoveObject(int button)
{
	EraseObject();
	MoveEnemy();
}

bool Enemy::CheckArea()
{
	visibleArea.clear();

	for (int R = 1; R < VISIBLE_RADIUS; ++R)
	{
		int x = 0;
		int y = R;

		int delta = 1 - 2 * R;
		int err = 0;

		while (y >= x) {
			if (_x + x > 0 && _y + y > 0) visibleArea.push_back(make_pair(_x + x, _y + y));
			if (_x + x > 0 && _y - y > 0) visibleArea.push_back(make_pair(_x + x, _y - y));
			if (_x - x > 0 && _y + y > 0) visibleArea.push_back(make_pair(_x - x, _y + y));
			if (_x - x > 0 && _y - y > 0) visibleArea.push_back(make_pair(_x - x, _y - y));

			if (_x + y > 0 && _y + x > 0) visibleArea.push_back(make_pair(_x + y, _y + x));
			if (_x + y > 0 && _y - x > 0) visibleArea.push_back(make_pair(_x + y, _y - x));
			if (_x - y > 0 && _y + x > 0) visibleArea.push_back(make_pair(_x - y, _y + x));
			if (_x - y > 0 && _y - x > 0) visibleArea.push_back(make_pair(_x - y, _y - x));
			

			err = 2 * (delta + y) - 1;

			if ((delta < 0) && (err <= 0)) {
				delta += 2 * ++x + 1;
				continue;
			}
			if ((delta > 0) && (err > 0)) {
				delta -= 2 * --y + 1;
				continue;
			}

			delta += 2 * (++x - --y);
		}
	}

	for (int i = 0; i < visibleArea.size(); i++)
	{
		if (wData->grid[visibleArea[i].second][visibleArea[i].first] == BASE) {
			basePos.first = COLS / 2, basePos.second = visibleArea[i].second;
			return true;
		}
	}
	return false;
}

bool Enemy::CheckAhead() 
{

	aheadArea.clear();

	int startX = _x + 1, startY = _y + 1;

	for (int i = 0; i < VISIBLE_DISTANCE; ++i)
	{
		if (_dir == UP && startY - i > 2) aheadArea.push_back(make_pair(startX, startY - i));
		else if (_dir == RIGHT && startX + i < COLS) aheadArea.push_back(make_pair(startX + i, startY));
		else if (_dir == BOT && startY + i < ROWS) aheadArea.push_back(make_pair(startX, startY + i));
		else if (_dir == LEFT && startX - i > 2) aheadArea.push_back(make_pair(startX - i, startY));
	}

	for (int i = 0; i < aheadArea.size(); i++)
	{
		if (wData->grid[aheadArea[i].second][aheadArea[i].first] == BRICK ||
			wData->grid[aheadArea[i].second][aheadArea[i].first] == CHARACTERPL ||
			wData->grid[aheadArea[i].second][aheadArea[i].first] == BASE) return true;
	}

	return false;
}

void Enemy::MoveToBase()
{
	if (_y <= basePos.second - (VISIBLE_RADIUS - 10) && _x + 1 < basePos.first) {
		_dir = RIGHT;
		_x++;
	}
	else if (_y <= basePos.second - (VISIBLE_RADIUS - 10) && _x + 1 > basePos.first) {
		_dir = LEFT;
		_x--;
	}
	else if (_x + 1 == basePos.first) _dir = BOT;
	else if (_y > basePos.second - (VISIBLE_RADIUS / 2) && _y < ROWS - _height - 1) {
		_dir = BOT;
		_y++;
	}
	else if (_y == basePos.second && _x + 1 < basePos.first) _dir = RIGHT;
	else if (_y == basePos.second && _x + 1 > basePos.first) _dir = LEFT;

	CheckNextStep();
}

void Enemy::ChangeDir()
{
	srand(time(NULL));
	prevDir = _dir;

	_dir = rand() % STOP;
	while (_dir == prevDir) _dir = rand() % STOP;


}

void Enemy::MoveEnemy()
{
	if (CheckArea()) {
		MoveToBase();
		return;
	}

	if (_dir == UP) _y--;
	else if (_dir == BOT) _y++;
	else if (_dir == RIGHT) _x++;
	else if (_dir == LEFT) _x--;

	mTick++;

	CheckNextStep();

	if (mTick % 20 == 0) ChangeDir();
}

// ----------- PLAYER ------------


void Player::DrawObject()
{
	if (_type == REGULAR) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = regularSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTERPL;
			}
		}
	}
	else if (_type == FAST) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = midSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTERPL;
			}
		}
	}
	else if (_type == ARMORED) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = armoredSprite[_dir][i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = CHARACTERPL;
			}
		}
	}


	if (_armorBonus) {
		if (_bonusTick % 10 == 0) _color = Yellow;
		else if (_bonusTick % 5 == 0) _color = _nativeColor;
		
		_bonusTick++;
	}
}

void Player::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u' ';
			wData->grid[_y + i][_x + j] = 0;
		}
	}
}

void Player::Control(int button)
{
	if (button == UPKEY) { 
		_dir = UP;
		_y--;
	}
	else if (button == RIGHTKEY) {
		_dir = RIGHT;
		_x++;
	} 
	else if (button == DOWNKEY) {
		_dir = BOT;
		_y++;
	}
	else if (button == LEFTKEY) {
		_dir = LEFT;
		_x--;
	}

	CheckNextStep();
}

void Player::Death()
{
	_life--;
	if (_life <= 0) {
		EraseObject();
		DeleteObject();
	}
	else {
		EraseObject();
		_x = COLS / 2 - 15;
		_y = ROWS - 4;
		_hp = 25;
	}
}

void Player::MoveObject(int button)
{
	EraseObject();
	Control(button);
}

void Player::PowerUP()
{
	if (_gunType < STRONGSHOT) _gunType++;
	if (_type < ARMORED) _type++;

	if (_type == FAST) _speed--;
	if (_gunType == DOUBLESHOT) _ammo++;
}

void Player::GetArmor()
{
	_hp += 25;
	_armorBonus = true;
}

void Player::PickBonus(Bonus* bonus, vector<Enemy*>& enemyList)
{
	if (bonus->GetBonusType() == STAR) PowerUP();
	else if (bonus->GetBonusType() == BOMB) DestroyEnemy(enemyList);
	else if (bonus->GetBonusType() == HELMET) GetArmor();
	else if (bonus->GetBonusType() == TIME) FreezeEnemy(enemyList);

	bonus->DeleteObject();
	bonus->EraseObject();
}

void Player::DestroyEnemy(vector<Enemy*>& enemyList)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->Death();
	}
}

void Player::FreezeEnemy(vector<Enemy*>& enemyList)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->SetSpeed(99999);
	}
}

// ------------- BULLET ----------

void Bullet::DrawObject() 
{
	wData->vBuf[_y][_x] = u'o' | (_color << 8);
}

void Bullet::EraseObject()
{
	wData->vBuf[_y][_x] = u' ';
}

void Bullet::MoveObject(int button)
{
	EraseObject();
	Trajectory();
}

void Bullet::SetBulletPower(int power, int speed)
{
	_power = power;
	_speed = speed;
}

int Bullet::GetBulletPower()
{
	return _power;
}

void Bullet::SetOwner(int objectID, int ownerType)
{
	_ownerID = objectID;
	_ownerType = ownerType;
}

int Bullet::GetOwner()
{
	return _ownerID;
}

void Bullet::Trajectory()
{
	if (_dir == UP) _y--;
	else if (_dir == RIGHT) _x++;
	else if (_dir == BOT) _y++;
	else if (_dir == LEFT) _x--;

	if (_x <= 1 || _x >= COLS - 1 || _y >= ROWS - 1 || _y <= 1) DeleteObject();
}


// -------------- WALL -----------------


void Wall::DrawObject()
{
	if (_type == BASE) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = baseSprite[i][j] | (_color << 8);
				wData->grid[_y + i][_x + j] = _type;
			}
		}

		return;
	}

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u'#' | (_color << 8);
			wData->grid[_y + i][_x + j] = _type;
		}
	}
}

void Wall::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u' ';
			wData->grid[_y + i][_x + j] = 0;
		}
	}
}

bool Wall::SetWallPos()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (wData->grid[_y + i][_x + j] != 0) return false;
		}
	}

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->grid[_y + i][_x + j] = _type;
		}
	}

	return true;
}

int Wall::GetWallType()
{
	return _type;
}

void Wall::DestroyWall(int direction, int power)
{
	if (direction == UP) _side = BOT;
	else if (direction == BOT) _side = UP;
	else if (direction == RIGHT) _side = LEFT;
	else if (direction == LEFT) _side = RIGHT;

	int damageSize = 0;
	if (_type == BRICK) {
		if (power == STANDART) damageSize = 1;
		else damageSize = 2;
	}
	else {
		if (power == STANDART) return;
		else damageSize = 1;
	}

	for (int size = 0; size < damageSize; size++)
	{
		if (_side == BOT) {
			_height--;

			for (int i = 0; i < _width; i++)
			{
				wData->grid[_y + _height][_x + i] = 0;
				wData->vBuf[_y + _height][_x + i] = u' ';
			}
		}
		else if (_side == UP) {
			_height--;

			for (int i = 0; i < _width; i++)
			{
				wData->grid[_y][_x + i] = 0;
				wData->vBuf[_y][_x + i] = u' ';
			}
			_y++;
		}
		else if (_side == RIGHT) {
			_width--;

			for (int i = 0; i < _height; i++)
			{
				wData->grid[_y + i][_x + _width] = 0;
				wData->vBuf[_y + i][_x + _width] = u' ';
			}
		}
		else if (_side == LEFT) {
			_width--;

			for (int i = 0; i < _height; i++)
			{
				wData->grid[_y + i][_x] = 0;
				wData->vBuf[_y + i][_x] = u' ';
			}
			_x++;
		}
	}


	if (_height == 0 || _width == 0) DeleteObject();
}

void Wall::SetWallType(int type)
{
	_type = type;

	if (_type > BASE) _type = BRICK;

	if (_type == BRICK) _color = BrRed;
	else if (_type == WATER) _color = Blue;
	else if (_type == GRASS) _color = BrGreen;
	else if (_type == STEEL) _color = White;
	else if (_type == ICE) _color = BrCyan;
	else if (_type == BASE) _color = Yellow, _width = 3, _height = 3;
}

// ------------- BONUS -----------------


void Bonus::DrawObject()
{
	if (_type == STAR) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = starBonus[i][j] | (_color << 8);
			}
		}
	}
	else if (_type == BOMB) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = bombBonus[i][j] | (_color << 8);
			}
		}
	}
	else if (_type == HELMET) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = helmetBonus[i][j] | (_color << 8);
			}
		}
	}
	else if (_type == TIME) {
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				wData->vBuf[_y + i][_x + j] = timeBonus[i][j] | (_color << 8);
			}
		}
	}
}

void Bonus::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u' ';
		}
	}
}

void Bonus::SetBonusType(int type)
{
	_type = type;
}

int Bonus::GetBonusType()
{
	return _type;
}
