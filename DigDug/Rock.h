#pragma once
#include "GameObject.h"

class Rock : public GameObject
{
public:
	Rock();
	~Rock();
	Rock(Spritesheet*, sf::Vector2f, sf::RenderWindow*);

	void startFall();
	void fall();

	virtual void update();
	virtual void collide();

private:
	bool isFalling;
};

