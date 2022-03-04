#pragma once
#include "GameObject.h"

class Shot : public GameObject
{
public:
	Shot();
	~Shot();
	Shot(sf::RenderWindow*, Game*);
	void shoot(sf::Vector2f, int);
	void update() override;
	void collide() override;
private:
	int currentMask;
	sf::Vector2f playerPos;
};

