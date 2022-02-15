#pragma once
#include "GameObject.h"

class Shot : public GameObject
{
public:
	Shot();
	~Shot();
	Shot(Spritesheet* s, sf::Vector2f playerPos, sf::RenderWindow* win);
	void shoot(sf::Vector2f, int);
	void update() override;
	void collide() override;
};

