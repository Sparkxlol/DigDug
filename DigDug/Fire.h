#pragma once
#include "GameObject.h"

class Fire : public GameObject
{
public:
	Fire();
	~Fire();
	Fire(sf::RenderWindow* win, Game* game);
	void shoot(sf::Vector2f, int);
	void update() override;
	void collide() override;
};

