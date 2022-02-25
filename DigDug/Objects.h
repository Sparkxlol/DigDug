#pragma once
#include <vector> 
#include "DigDug.h"
#include "Fygar.h"
#include "Pooka.h"
#include "Rock.h"
#include "Sand.h"
#include "Score.h"


class Objects
{
public:
	Objects();
	~Objects();

	sf::FloatRect& getCollider(int, int); // Used by objects
	bool getActive(int);

	void drawObjects(); // Used by game
	void updates();
	void setUpLevel(int);


private:
	DigDug* digDug;
	std::vector<Fygar*> fygars;
	std::vector<Pooka*> pookas;
	std::vector<Rock*> rocks;
	std::vector<Sand*> sand;
	std::vector<Score*> scores;
	sf::RenderWindow* window;
};

