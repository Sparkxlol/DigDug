#pragma once
#include "GameObject.h"

class Fire : public GameObject
{
public:
	Fire();
	~Fire();
	Fire(Spritesheet* s, sf::Vector2f playerPos, sf::RenderWindow* win);
	void shoot(sf::Vector2f, int);
	void update() override;
	void collide() override;
};

