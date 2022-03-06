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
	virtual void movement();

	int moveTowardPlayer();
	int moveAwayPlayer();
	int moveRandom();
	int moveFromRock(sf::Vector2f);
	int escapeLevel();

private:
	sf::Clock pumpClock;
	sf::Clock escapeTimer;
	bool sandCollided[4];
	bool canFloat;
	int currentPump;
	float speed;
};

