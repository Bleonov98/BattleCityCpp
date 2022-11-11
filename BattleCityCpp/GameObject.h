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


	bool IsObjectDelete();

protected:

	int _x, _y, _color, _speed, _width, _height;
	bool _deleteObject = false;

	wd* wData;

	void DeleteObject();

	virtual ~GameObject() {
		delete this;
	}

};



// - - - - - - - - - - OBJECTS TYPE - - - - - - - - - -



class DynamicObject : public GameObject 
{
public:
	DynamicObject(wd* wData, int x, int y, int color, int speed) : GameObject(wData, x, y, color, speed) {}


	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;

	virtual void MoveObject() = 0;


	int GetDirection();

protected:

	void CheckNextStep();

	int _dir = UP;
};



class StaticObject : public GameObject
{

};


//          --- DYNAMIC OBJECTS ---
// ----------------- CHAR -----------------------



class Character: public DynamicObject
{
public:

	Character(wd* wData, int x, int y, int color, int speed) : DynamicObject(wData, x, y, color, speed) {
		_speed = 3;
	}


	void SetType(int type);

	int GetType();


	int GetHp();

	int GetGunType();


	void Hit();

protected:

	int _type = REGULAR, _gunType = SINGLESHOT, _hp = 25;

	void Death();

};



class Player : public Character
{
public:

	Player(wd* wData, int x, int y, int color, int speed) : Character(wData, x, y, color, speed) {}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

private:

	void Control();

	char16_t regularSprite[5][3][4]{
		{
			u" | ",
			u"| |",
			u"|-|"
		},
		{
			u"-- ",
			u"| -",
			u"-- ",
		},
		{
			u"|-|",
			u"| |",
			u" | ",
		},
		{
			u" --",
			u"- |",
			u" --",
		},
		{
			u"|-|",
			u"|=|",
			u"|-|",
		}
	};
};



class Enemy : public Character
{

};


// --------------   BULLET  ----------------


class Bullet : public DynamicObject
{

};




//          --- STATIC OBJECTS ---



class Wall : public StaticObject 
{

};



class Bonus : public StaticObject 
{

};