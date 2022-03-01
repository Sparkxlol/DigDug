#pragma once
#include "GameObject.h"

class Rock : public GameObject
{
public:
	Rock();
	~Rock();
	Rock(sf::RenderWindow*, Game* game);

	bool getFall() const;

	sf::FloatRect& getCollider();

	void startFall();
	void die();

	virtual void update();
	virtual void collide();

private:
	bool isFalling;
};

