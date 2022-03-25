#pragma once
#include "Spritesheet.h"
#include <SFML/Graphics.hpp>
#include <fstream>

class UI
{
public:
	UI(sf::RenderWindow*);
	~UI();

	void addScore(int);
	void resetScore();
	void setRound(int);
	void setLives(int);
	void setHighScore(int);
	void setupMainMenu();
	void drawObject();

	int getLives();
	bool getMenuActive();
	void update();

private:
	void setupBackground(int);
	void setupHighscore();
	void recordHighscore();
	void changeSprites(Spritesheet[], int, int);
	void setFlowers(int);

	sf::Clock mainMenuTimer;
	sf::Clock oneupTimer;
	bool settingUpMainMenu;
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
	Spritesheet background;
	Spritesheet menu;
};

