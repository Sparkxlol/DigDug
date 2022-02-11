#pragma once
#include "GameObject.h"

class Rock : public GameObject
{
public:
	Rock();
	~Rock();

	void fall();
	bool getIsFalling();

	void update();

private:
	bool isFalling;
};

