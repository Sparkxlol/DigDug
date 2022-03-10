#pragma once
#include "Fire.h"
#include "Enemy.h"


class Fygar : public Enemy
{
public:
	Fygar();
	~Fygar();
	Fygar(sf::RenderWindow*, Game* game);

	void update() override;
	void drawObject() override;
	void movement();
	void reset(sf::Vector2f) override;

	void shootFire();
private:
	Fire fire;
};

