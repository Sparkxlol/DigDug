#pragma once
#include "Spritesheet.h"
#include <fstream>

class UI
{
public:
	UI(sf::RenderWindow*);
	~UI();

	void addScore(int);
	void setRound(int);
	void addLives(int);
	void setHighScore(int);
	void drawObject();

private:
	void setupHighscore();
	void recordHighscore();
	void changeSprites(Spritesheet[], int, int);
	void setFlowers(int);

	int highScore;
	int currentScore;
	int currentLives;
	sf::RenderWindow* win;
	Spritesheet hiScoreText;
	Spritesheet roundText;
	Spritesheet oneupText;
	Spritesheet score[6];
	Spritesheet hiScore[6];
	Spritesheet round[2];
	Spritesheet lives[3];
	Spritesheet flowers[12];
};

