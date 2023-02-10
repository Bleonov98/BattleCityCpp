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

	void SetSpeed(int speed) { _speed = speed; };


	virtual void DrawObject() = 0;

	virtual void EraseObject() = 0;


	void DeleteObject();

	bool IsObjectDelete();

protected:

	int _x, _y, _color, _speed = 3, _width, _height, _id;
	bool _deleteObject = false;

	wd* wData;

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

	DynamicObject(wd* wData, int x, int y, int color) : GameObject(wData, x, y, color) {
		_nativeSpeed = _speed;
	}


	virtual void MoveObject(int button) = 0;


	int GetDirection();

	void SetDirection(int dir);


protected:

	void CheckNextStep();

	int _dir = UP, _nativeSpeed;
};


//          --- DYNAMIC OBJECTS ---
// ----------------- CHAR -----------------------



class Character: public DynamicObject
{
public:

	Character(wd* wData, int x, int y, int color) : DynamicObject(wData, x, y, color) {
		_width = 3;
		_height = 3;
		_nativeColor = color;
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

	virtual void Death();


protected:

	int _type = REGULAR, _gunType = SINGLESHOT, _hp = 25, _ammo = 1, _bonusTick = 0, _nativeColor;

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
public:

	Enemy(wd* wData, int x, int y, int color) : Character(wData, x, y, color) {
		_dir = RIGHT;
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject(int button) override;

	bool CheckAhead();

	void SetBonusKeeper() { _keepBonus = true; }

	bool BonusKeeper() { return _keepBonus; }

private:

	void MoveEnemy();

	void ChangeDir();

	bool CheckArea();

	void MoveToBase();

	int mTick = 0, prevDir = rand() % STOP;
	bool _keepBonus = false;
	pair<int, int> basePos;
 
	const int VISIBLE_RADIUS = 23;
	const int VISIBLE_DISTANCE = 45;

	vecPairInt visibleArea;
	vecPairInt aheadArea;
};

class Player : public Character
{
public:

	Player(wd* wData, int x, int y, int color) : Character(wData, x, y, color) {}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject(int button) override;

	int GetLifes() { return _life; }

	// -- bonuses --

	void PowerUP();

	void GetArmor();

	void PickBonus(Bonus* bonus, vector<Enemy*>& enemyList);

	void DestroyEnemy(vector<Enemy*>& enemyList);

	void FreezeEnemy(vector<Enemy*>& enemyList);

	// -- -- -- -- --

private:

	void Death();

	void Control(int button);

	int _life = 3;
	bool _armorBonus = false;
};


// --------------   BULLET  ----------------


class Bullet : public DynamicObject
{
public:

	Bullet(wd* wData, int x, int y, int color) : DynamicObject(wData, x, y, color) {
		_height = 1;
		_width = 1;
	}

	void DrawObject() override;

	void EraseObject() override;

	void MoveObject(int button) override;

	
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
		_height = 6;
		_width = 6;
	}

	void DrawObject() override;

	void EraseObject() override;


	bool SetWallPos();

	void SetWallType(int type);

	int GetWallType();

	void DestroyWall(int direction, int power);

private:

	int _type = BRICK, _side;

	char16_t baseSprite[3][4]{
		u" # ",
		u"###",
		u"###"
	};

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
