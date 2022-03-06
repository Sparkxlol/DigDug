#pragma once
#include "Enemy.h"

class Game;

class Pooka : public Enemy
{
public:
	Pooka();
	~Pooka();
	Pooka(sf::RenderWindow*, Game*);
	void update() override;
};

