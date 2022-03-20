#include "Score.h"


Score::Score(sf::RenderWindow* win, UI* const ui)
	: window(win), ui(ui)
{ 
	spritesheet1.setupSprite("Images/scores1Spritesheet.png",
		sf::Vector2i(64, 112), sf::Vector2i(32, 16));
	spritesheet2.setupSprite("Images/scores2Spritesheet.png",
		sf::Vector2i(64, 96), sf::Vector2i(32, 16));
	isActive = false;
	deathTime = 2.5f;
	totalScore = 0;
}


Score::~Score()
{
	// Deconstructor
}


// Returns if a score is currently active.
bool Score::getActive()
{
	return isActive;
}


// Sets the score active.
void Score::setActive(const bool& active)
{
	isActive = active;
}


// Changes the score appearance based on the position and type given.
void Score::changeScore(sf::Vector2f pos, std::string type)
{
	totalScore = 0;
	int initialYPos = pos.y / 16;

	// The score is found based on depth of enemy and score.
	if (initialYPos < 5)
		totalScore += 200;
	else if (initialYPos < 8)
		totalScore += 300;
	else if (initialYPos < 11)
		totalScore += 400;
	else if (initialYPos < 14)
		totalScore += 500;

	// If enemy is crushed by rock score is x2.
	if (type == "rock")
		totalScore *= 2;

	// A sprite is loaded for each possible score.
	switch (totalScore)
	{
	case 200:
		spritesheet1.loadSprite(0);
		break;
	case 300:
		spritesheet1.loadSprite(2);
		break;
	case 400:
		spritesheet1.loadSprite(4);
		break;
	case 500:
		spritesheet1.loadSprite(6);
		break;
	case 600:
		spritesheet1.loadSprite(8);
		break;
	case 800:
		spritesheet1.loadSprite(10);
		break;
	case 1000:
		spritesheet1.loadSprite(12);
		break;
	default:
		spritesheet1.loadSprite(1);
	}

	type = "enemy";
	spritesheet1.setPosition(pos);
	clock.restart();
	setActive(true);
}


void Score::changeScore(sf::Vector2f pos, int fruitIndex)
{
	switch (fruitIndex)
	{
	case 0:
		spritesheet2.loadSprite(0);
		totalScore = 400;
		break;
	case 1:
		spritesheet2.loadSprite(2);
		totalScore = 600;
		break;
	case 2:
		spritesheet2.loadSprite(4);
		totalScore = 800;
		break;
	case 3:
		spritesheet2.loadSprite(6);
		totalScore = 1000;
		break;
	case 4:
		spritesheet2.loadSprite(8);
		totalScore = 2000;
		break;
	case 5:
		spritesheet2.loadSprite(10);
		totalScore = 3000;
		break;
	case 6:
		spritesheet2.loadSprite(1);
		totalScore = 4000;
		break;
	case 7:
		spritesheet2.loadSprite(3);
		totalScore = 5000;
		break;
	case 8:
		spritesheet2.loadSprite(5);
		totalScore = 6000;
		break;
	case 9:
		spritesheet2.loadSprite(7);
		totalScore = 7000;
		break;
	default:
		spritesheet2.loadSprite(9);
		totalScore = 8000;
		break;
	}

	type = "fruit";
	spritesheet2.setPosition(pos);
	setActive(true);
	clock.restart();
}


// If score is ever set active, the clock is reset for
// its deletion after 1 seconds of activity.
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


// Draws the score.
void Score::drawObject()
{
	if (type == "enemy")
		window->draw(spritesheet1);
	else
		window->draw(spritesheet2);
}
