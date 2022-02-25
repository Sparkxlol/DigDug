#pragma once
#include "GameObject.h"

class Rock : public GameObject
{
public:
	Rock();
	~Rock();
	Rock(sf::RenderWindow*, Game* game);

	void startFall();
	void fall();

	virtual void update();
	virtual void collide();

private:
	bool isFalling;
};

