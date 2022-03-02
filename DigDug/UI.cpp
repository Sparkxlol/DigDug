#include "UI.h"

UI::UI(sf::RenderWindow* window, int highScore)
	: win(window)
{
	font.loadFromFile("Font/namco.ttf");

	hiScore.setFont(font);
	hiScore.setPosition(sf::Vector2f(16 * 12, 16));
	hiScore.setString(std::to_string(highScore));
	hiScore.setCharacterSize(12);

	round.setFont(font);
	round.setPosition(sf::Vector2f(16 * 12, 16 * 12));
	round.setCharacterSize(12);

	score.setFont(font);
	score.setPosition(sf::Vector2f(16 * 12, 16 * 2));
	score.setCharacterSize(12);

	for (auto& sprite : flowers)
	{
		sprite.setupSprite("Images/flowerSpritesheet.png", sf::Vector2i(48, 32), sf::Vector2i(16, 32));
		sprite.loadSprite(3);
	}

	for (int i = 0; i < 3; i++)
	{
		lives[i].setupSprite("Images/livesSpritesheet.png", sf::Vector2i(48, 16), sf::Vector2i(16, 16));
		lives[i].setPosition(12 * 16, (9 + i) * 16);
	}

	setLives(2);
	setScore(0);
	setRound(1);
}


UI::~UI()
{
	// Deconstructor
}


void UI::setScore(int value)
{
	score.setString(std::to_string(value));
}


void UI::setRound(int round)
{
	currentRound = round;
	this->round.setString(std::to_string(round));
	setFlowers();
}


void UI::setLives(int curLives)
{
	if (curLives != 0)
	{
		currentLives = curLives;
		int height = curLives / 3;
		int width = curLives - 1 % 3;

		for (int i = 0; i < 3; i++)
		{
			if (height > 1)
				lives[i].setTextureRect(sf::IntRect(0, 0, 48, 16));
			else if (height > 0)
				lives[i].setTextureRect(sf::IntRect(0, 0, width, 16));
			else
				lives[i].setTextureRect(sf::IntRect(0, 0, 0, 0));
			height--;
		}
	}
}


void UI::setFlowers()
{
	int initialX = 11 * 16;
	int initialY = 0;

	for (int i = 0; i < currentRound; i++)
	{
		flowers[i].setPosition(sf::Vector2f(initialX - (i * 16), initialY));
	}
}


void UI::drawObject()
{
	win->draw(score);
	win->draw(round);
	win->draw(hiScore);

	// Could change to draw only currentLives / 3 sprites.
	for (auto& sprite : lives)
		win->draw(sprite);

	for (int i = 0; i < currentRound; i++)
	{
		win->draw(flowers[i]);
	}
}
