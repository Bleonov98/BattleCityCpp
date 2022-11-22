#pragma once
#include "Tools.h"

class GameObject
{
public:
	GameObject(wd* wData, int x, int y, int color) {
		this->wData = wData;
		_x = x, _y = y, _color = color;
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

	int _x, _y, _color, _speed = 3, _width, _height, _id;
	bool _deleteObject = false;

	wd* wData;

	void DeleteObject();

	virtual ~GameObject() {
		delete this;
	}

};



// - - - - - - - - - - - -

class Bullet;
class Wall;
class Bonus;

// - - - - - - - - - - OBJECTS TYPE - - - - - - - - - -


class DynamicObject : public GameObject 
{
public:

	DynamicObject(wd* wData, int x, int y, int color) : GameObject(wData, x, y, color) {}


	virtual void MoveObject() = 0;


	int GetDirection();

	void SetDirection(int dir);


protected:

	void CheckNextStep();

	int _dir = UP;
};


//          --- DYNAMIC OBJECTS ---
// ----------------- CHAR -----------------------



class Character: public DynamicObject
{
public:

	Character(wd* wData, int x, int y, int color) : DynamicObject(wData, x, y, color) {
		_width = 3;
		_height = 3;
	}


	void Shot(vector <GameObject*> &allObjList, vector <Bullet*> &bulletList, Bullet* bullet, int ownerType);


	void SetType(int type);

	int GetType();


	int GetHp();


	int GetGunType();

	void ReloadGun();


	void SetID(int id);

	int GetID();


	void Hit();


protected:

	int _type = REGULAR, _gunType = SINGLESHOT, _hp = 25, _ammo = 1;

	void Death();

};



class Player : public Character
{
public:

	Player(wd* wData, int x, int y, int color) : Character(wData, x, y, color) {}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;


	void PowerUP();


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
	char16_t midSprite[5][3][4]{
		{
			u" | ",
			u"|-|",
			u"|-|"
		},
		{
			u"-- ",
			u"||-",
			u"-- "
		},
		{
			u"|-|",
			u"|-|",
			u" | ",
		},
		{
			u" --",
			u"-||",
			u" --"
		},
		{
			u"|-|",
			u"|=|",
			u"|-|",
		}
	};
	char16_t armoredSprite[5][3][4]{
		{
			u" | ",
			u"#-#",
			u"#-#"
		},
		{
			u"## ",
			u"||-",
			u"## "
		},
		{
			u"#-#",
			u"#-#",
			u" | ",
		},
		{
			u" ##",
			u"-||",
			u" ##"
		},
		{
			u"#-#",
			u"#=#",
			u"#-#",
		}
	};
};



class Enemy : public Character
{

};


// --------------   BULLET  ----------------


class Bullet : public DynamicObject
{
public:

	Bullet(wd* wData, int x, int y, int color) : DynamicObject(wData, x, y, color) {}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject() override;

	
	void SetBulletPower(int power, int speed);

	int GetBulletPower();


	void SetOwner(int ownerID, int ownerType);

	int GetOwner();


private:

	void Trajectory();

	int _power = STANDART, _ownerID, _ownerType;

};




//          --- STATIC OBJECTS ---



class Wall : public GameObject
{
public:

	Wall(wd* wData, int x, int y, int color) : GameObject(wData, x, y, color) {
		_height = 4;
		_width = 4;
	}

	void DrawObject() override;

	void EraseObject() override;


	void ChangeWallPos();

	void ChangeWallType();


	bool SetWallPos();

	int GetWallType();


	


private:

	int _type = BRICK;

};


class Bonus : public GameObject 
{
public:

	Bonus(wd* wData, int x, int y, int color) : GameObject(wData, x, y, color) {
		_height = 3;
		_width = 3;
	}


	void DrawObject() override;

	void EraseObject() override;


	void SetBonusType(int type);

	int GetBonusType();


private:

	char16_t starBonus[3][4]{
		u"---",
		u"|*|",
		u"---"
	};
	char16_t bombBonus[3][4]{
		u"---",
		u"|#|",
		u"---"
	};
	char16_t helmetBonus[3][4]{
		u"---",
		u"|@|",
		u"---"
	};
	char16_t timeBonus[3][4]{
		u"---",
		u"|%|",
		u"---"
	};

	int _type = STAR;

};