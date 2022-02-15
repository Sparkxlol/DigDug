#pragma once
#include "GameObject.h"
#include "Shot.h"

class DigDug : public GameObject
{
public:
	DigDug();
	~DigDug();
	DigDug(Spritesheet*, Spritesheet*, sf::Vector2f, sf::RenderWindow*);

	void shoot();
	void playerInput();
	void die();

	void update() override;
	void drawObject() override;
	void collide() override;

private:
	int input;
	Shot shot;
	float speed;
};
