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

void DynamicObject::SetNewPosition(int x, int y)
{
	_x = nX, _y = nY;
}

void DynamicObject::MoveObject()
{
	EraseObject();
	SetNewPosition(nX, nY);
	DrawObject();
}

int DynamicObject::GetDirection()
{
	return _dir;
}
