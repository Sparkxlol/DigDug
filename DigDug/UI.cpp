#include "UI.h"

UI::UI(sf::RenderWindow* window)
	: win(window)
{
	// Sets each spritesheet to their corresponding size, and moves to their correct position.
	roundText.setupSprite("Images/roundSpritesheet.png",
		sf::Vector2i(40, 8), sf::Vector2i(40, 8));
	roundText.setPosition(12 * 16, 12 * 16);
	oneupText.setupSprite("Images/1upSpritesheet.png",
		sf::Vector2i(16, 8), sf::Vector2i(16, 8));
	oneupText.setPosition(12 * 16, 3 * 16);
	hiScoreText.setupSprite("Images/hiscoreSpritesheet.png",
		sf::Vector2i(48, 16), sf::Vector2i(48, 16));
	hiScoreText.setPosition(12 * 16, 16);

	for (int i = 0; i < 6; i++)
	{
		score[i].setupSprite("Images/numberSpritesheet.png",
			sf::Vector2i(80, 8), sf::Vector2i(8, 8));
		score[i].setPosition(12 * 16 + (i * 8), 16 * 4);
	}

	for (int i = 0; i < 6; i++)
	{
		hiScore[i].setupSprite("Images/numberSpritesheet.png",
			sf::Vector2i(80, 8), sf::Vector2i(8, 8));
		hiScore[i].setPosition(12 * 16 + (i * 8), 16 * 2);
	}

	for (int i = 0; i < 2; i++)
	{
		round[i].setupSprite("Images/numberSpritesheet.png",
			sf::Vector2i(80, 8), sf::Vector2i(8, 8));
		round[i].setPosition(14 * 16 + (i * 8), 16 * 13 - 8);
	}

	for (int i = 0; i < 12; i++)
	{
		flowers[i].setupSprite("Images/flowerSpritesheet.png",
			sf::Vector2i(48, 32), sf::Vector2i(16, 32));
		flowers[i].setPosition(i * 16, 1);
	}

	for (int i = 0; i < 3; i++)
	{
		lives[i].setupSprite("Images/livesSpritesheet.png",
			sf::Vector2i(48, 16), sf::Vector2i(16, 16));
		lives[i].setPosition(12 * 16, (9 + i) * 16);
	}

	background.setupSprite("Images/uiBackgroundSpritesheet.png",
		sf::Vector2i(192, 672), sf::Vector2i(192, 224));
	background.setPosition(sf::Vector2f(0, 0));

	// Sets initial lives to 2, score to 0,
	// highScore to previous score, and round to 1
	currentScore = 0;
	currentLives = 0;
	setupHighscore();
	addLives(2);
	addScore(0);
	setRound(1);
}


UI::~UI()
{
	// Deconstructor
	recordHighscore();
}


// Sets UI score to inputted value and if greater than high score, sets that.
void UI::addScore(int value)
{
	currentScore += value;
	changeSprites(score, 6, currentScore);
	if (value > highScore)
		setHighScore(value);
}


// Sets new high scores to inputted value
void UI::setHighScore(int value)
{
	highScore = value;
	changeSprites(hiScore, 6, value);
}


// Sets round to inputted value and flowers to correct amount based on round.
void UI::setRound(int roundNum)
{
	// Prevents negative/0 rounds and over 100 rounds.
	if (roundNum > 0 && roundNum < 100)
	{
		changeSprites(round, 2, roundNum);
		setFlowers(roundNum);
	}

	setupBackground(roundNum);
}


// Sets the current amount of lives shown in the UI to inputted value.
void UI::addLives(int live)
{
	currentLives += live;

	// Prevents 0 or more than 9 lives from showing, 0 dealt with in game.
	if (currentLives > 0 && currentLives <= 9)
	{
		// 3 x 3 grid of lives with 3 seperate images
		// Height is amount of images and width is lives per image used.
		int height = currentLives / 3 + 1;
		int width = currentLives % 3;

		for (int i = 0; i < 3; i++)
		{
			// If height is > 1 make full rect with all 3 lives shown.
			if (height > 1) 
				lives[i].setTextureRect(sf::IntRect(0, 0, 48, 16));
			else if (height > 0) // If height is at 1 show only width lives.
			{
				lives[i].setTextureRect(sf::IntRect(0, 0, width * 16, 16));
			}
			else // Hide image if height is 0.
				lives[i].setTextureRect(sf::IntRect(0, 0, 0, 0));
			height--;
		}
	}
}



// Sets flowers shown on screen that represent the current round.
void UI::setFlowers(int currentRound)
{
	// Big flowers is 10 rounds, while small is 1 round.
	int bigFlowersLeft = currentRound / 10;
	int smallFlowersLeft;

	// Makes sure that big flowers have priority over small flowers.
	if (currentRound % 10 + bigFlowersLeft > 12)
		smallFlowersLeft = 12 - bigFlowersLeft;
	else
		smallFlowersLeft = currentRound % 10;
	

	for (int i = 11; i >= 0; i--)
	{
		// Creates small flowers first, then big flowers, then nothing.
		if (smallFlowersLeft > 0)
		{
			flowers[i].setColor(sf::Color::White);
			flowers[i].loadSprite(0);
			smallFlowersLeft--;
		}
		else if (bigFlowersLeft > 0)
		{
			flowers[i].setColor(sf::Color::White);
			flowers[i].loadSprite(2);
			bigFlowersLeft--;
		}
		else
			flowers[i].setColor(sf::Color::Transparent);
	}
}


// Draws all sprites in UI 
void UI::drawObject()
{
	win->draw(roundText);
	win->draw(hiScoreText);

	for (int i = 0; i < 6; i++)
	{
		win->draw(score[i]);
		win->draw(hiScore[i]);
	}

	for (int i = 0; i < 2; i++)
		win->draw(round[i]);

	for (int i = 0; i < 3; i++)
		win->draw(lives[i]);

	for (int i = 0; i < 12; i++)
		win->draw(flowers[i]);

	win->draw(background);
}


// Changes numbers based sprites based on value.
// Passes in spritesheet numbers, the size of the arr and the wanted value.
void UI::changeSprites(Spritesheet arr[], int size, int value)
{
	int digit = size - 1;

	if (value > 999999) // Rolls over if score is too high.
		value = 0;

	if (value == 0) // If value is initially 0 set to load 0.
	{
		arr[size - 1].loadSprite(0);
		arr[digit].setColor(sf::Color::White);
		digit--;
	}

	// Sets each digit to white if value is greater than 0
	// dividing value by 10 each time. This finds how long
	// the number is allowing to only draw non-0 digits.
	for (int i = digit; i >= 0; i--)
	{
		if (value > 0)
		{
			arr[digit].loadSprite(value % 10);
			arr[digit].setColor(sf::Color::White);
			value /= 10;
		}
		else
			arr[digit].setColor(sf::Color::Transparent);
		digit--;
	}
}


// Sets up the background of the texture based on level.
void UI::setupBackground(int round)
{
	int lowerRound = round % 12;

	if (lowerRound >= 1 || lowerRound < 5)
		background.loadSprite(0);
	else if (lowerRound >= 5 || lowerRound < 9)
		background.loadSprite(1);
	else
		background.loadSprite(2);
}


// Gets the high score from the txt file and sets high score to it.
void UI::setupHighscore()
{
	std::fstream file("highscores.txt", std::ios::in | std::ios::out | std::ios::app);
	file << ""; // Creates new file if nothing exists.

	int datum = 0;
	file >> datum;
	setHighScore(datum);

	file.close();
}


// Sets high score in txt file to correct high score.
void UI::recordHighscore()
{
	std::fstream file("highscores.txt");
	if (!file.is_open())
	{
		std::cerr << "Couldn't open highscore file!\n";
		return;
	}

	file << highScore;

	file.close();
}