#include "GameObject.h"

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

void DynamicObject::CheckNextStep()
{
	if (_dir == UP && wData->grid[_y][_x] > 3) _y++;
	else if (_dir == RIGHT && wData->grid[_y][_x + _width] > 3) _x--;
	else if (_dir == BOT && wData->grid[_y + _height - 1][_x] > 3) _y--;
	else if (_dir == LEFT && wData->grid[_y][_x] > 3) _x++;
}

void Character::SetType(int type)
{
	_type = type;
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

// ----------- PLAYER ------------


void Player::DrawObject()
{
	if (_type == REGULAR) {
		_width = 3;
		_height = 3;
	}

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = regularSprite[_dir][i][j] | (_color << 8);
		}
	}
}

void Player::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			wData->vBuf[_y + i][_x + j] = u' ';
		}
	}
}

void Player::Control()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000) { 
		_dir = UP;
		_y--;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		_dir = RIGHT;
		_x++;
	} 
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		_dir = BOT;
		_y++;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		_dir = LEFT;
		_x--;
	}
	CheckNextStep();
}

void Player::MoveObject()
{
	EraseObject();
	Control();
}

