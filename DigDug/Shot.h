#pragma once
#include "GameObject.h"

class Shot : public GameObject
{
public:
	Shot(Spritesheet*, sf::Vector2f, sf::RenderWindow*);
	~Shot();
	void shoot(sf::Vector2f, int);
	void update();
	void collide();
};

