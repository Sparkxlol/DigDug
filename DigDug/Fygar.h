#pragma once
#include "Fire.h"
#include "Enemy.h"


class Fygar : public Enemy
{
public:
	Fygar();
	~Fygar();
	Fygar(sf::RenderWindow*, Game* game);
	void shootFire();
	void update() override;
	void drawObject() override;
	void collide() override;
	void movement() override;
private:
	Fire fire;
};

