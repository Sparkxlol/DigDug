#include "Score.h"


Score::Score(sf::RenderWindow* win, UI* const ui)
	: window(win), ui(ui)
{ 
	spritesheet.setupSprite("Images/scores1Spritesheet.png",
		sf::Vector2i(64, 112), sf::Vector2i(32, 16));
	isActive = false;
	deathTime = 2.5f;
	totalScore = 0;
}


Score::~Score()
{
	// Deconstructor
}


bool Score::getActive()
{
	return isActive;
}


void Score::setActive(const bool& active)
{
	isActive = active;
}


void Score::changeScore(sf::Vector2f pos, std::string type)
{
	totalScore = 0;
	int initialYPos = pos.y / 16;

	if (initialYPos < 5)
		totalScore += 200;
	else if (initialYPos < 8)
		totalScore += 300;
	else if (initialYPos < 11)
		totalScore += 400;
	else if (initialYPos < 14)
		totalScore += 500;

	if (type == "rock")
		totalScore *= 2;

	switch (totalScore)
	{
	case 200:
		spritesheet.loadSprite(0);
		break;
	case 300:
		spritesheet.loadSprite(2);
		break;
	case 400:
		spritesheet.loadSprite(4);
		break;
	case 500:
		spritesheet.loadSprite(6);
		break;
	case 600:
		spritesheet.loadSprite(8);
		break;
	case 800:
		spritesheet.loadSprite(10);
		break;
	case 1000:
		spritesheet.loadSprite(12);
		break;
	default:
		spritesheet.loadSprite(1);
	}

	spritesheet.setPosition(pos);
	clock.restart();
	setActive(true);
}


void Score::update()
{
	if (getActive())
	{
		if (clock.getElapsedTime().asSeconds() > 1.0f)
		{
			ui->addScore(totalScore);
			setActive(false);
		}
	}
}


void Score::drawObject()
{
	window->draw(spritesheet);
}
