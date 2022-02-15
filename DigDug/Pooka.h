#pragma once
#include "Enemy.h"

class Pooka : public Enemy
{
public:
	Pooka();
	~Pooka();
	Pooka(Spritesheet*, sf::Vector2f, sf::RenderWindow*);
	void update() override;
	void collide() override;
	void movement() override;
};

