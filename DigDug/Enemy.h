#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	Enemy(sf::RenderWindow*, Game*);

	void changeCurrentPump(int);

	virtual void collide() = 0;
	virtual void update() = 0;
	virtual void movement() = 0;

private:
	bool canFloat;
	int currentPump;
};

