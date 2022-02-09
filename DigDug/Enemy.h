#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	bool getCanFloat(); 
	int getCurrentPump();
	void die();
private:
	bool canFloat;
	int currentPump;
};

