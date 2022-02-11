#pragma once
#include "Spritesheet.h"
#include "GameObject.h"
#include "Shot.h"

class DigDug : public GameObject
{
public:
	DigDug();
	~DigDug();
	DigDug(Spritesheet*);

	//Shot& getShot();
	//void shoot();
	void update();
	void playerInput();
private:
	bool canMove;
	//Shot shot;
	float speed;
};

