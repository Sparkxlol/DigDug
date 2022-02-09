#pragma once
#include "Fire.h"

class Fygar : public Enemy
{
public:
	Fygar();
	breathFire();
	Fire* getFire();
	void update();
private:
	Fire* fire;
};

