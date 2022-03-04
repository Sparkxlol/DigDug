#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	Enemy(sf::RenderWindow*, Game*);

	void changeCurrentPump(int);
	int getCurrentPump();
	bool getSandCollided();
	float getSpeed();

	void pumpUpdate();
	void die(std::string);

	virtual void collide();
	virtual void update() = 0;
	virtual void movement() = 0;

private:
	sf::Clock pumpClock;
	bool canFloat;
	int currentPump;
	int sandCollided;
	float speed;
};

