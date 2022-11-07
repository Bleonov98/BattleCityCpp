#pragma once
#include "Tools.h"

class GameObject
{
public:
	GameObject(wd* wData, int x, int y, int color, int speed) {
		this->wData = wData;
		_x = x, _y = y, _speed = speed, _color = color;
	}

	int GetX();

	int GetY();

	int GetSpeed();

	int GetWidth();

	int GetHeight();



	void SetX(int x);

	void SetY(int y);



	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;



	void DeleteObject();

	bool IsObjectDelete();

protected:

	int _x, _y, _color, _speed, _width, _height;
	bool _deleteObject = false;

	wd* wData;

	virtual ~GameObject() {
		delete this;
	}

};



// - - - - - - - - - - DYNAMIC OBJECTS - - - - - - - - - -



class DynamicObject : public GameObject 
{
public:
	DynamicObject(wd* wData, int x, int y, int color, int speed) : GameObject(wData, x, y, color, speed) {}


	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;



	void SetNewPosition(int x, int y);

	void MoveObject();


	int GetDirection();

protected:

	int _dir = UP, nX, nY;
};



class Player : public DynamicObject 
{
	Player(wd* wData, int x, int y, int color, int speed) : DynamicObject(wData, x, y, color, speed) {
		
	}

	char16_t regularSprite[4][3][4]{
		{
			u" | ",
			u"| |",
			u"---"
		},
		{
			u"|- ",
			u"| =",
			u"|- ",
		},
		{
			u"---",
			u"| |",
			u" | ",
		},
		{
			u" -|",
			u"= |",
			u" -|",
		}
	};

	int _type = REGULAR;
};



class Enemy : public DynamicObject 
{

};



class Bullet : public DynamicObject
{

};



// --------------- STATIC OBJECTS -------------------



class StaticObject : public GameObject 
{

};



class Wall : public StaticObject 
{

};



class Bonus : public StaticObject 
{

};