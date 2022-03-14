#include "Game.h"
#include <fstream>

Game::Game() : Game(nullptr)
{ }


Game::~Game()
{
	delete digDug;

	for(int i = 0; i < enemies.size(); i++)
	{
		delete enemies.at(i);
	}

	for(int i = 0; i < rocks.size(); i++)
	{
		delete rocks.at(i);
	}

	for(int i = 0; i < sand.size(); i++)
	{
		delete sand.at(i);
	}

	for(int i = 0; i < scores.size(); i++)
	{
		delete scores.at(i);
	}

	delete ui;

	enemies.clear();
	rocks.clear();
	sand.clear();
	scores.clear();
}


Game::Game(sf::RenderWindow* window)
	: window(window), currentLevel(1), currentLives(2)
{
	// Create UI and DigDug
	setupObjects();
	setupLevels();
	loadLevel(0);
}


int Game::getArrLength(const Game::Object& object) const
{
	switch (object)
	{
	case Object::dig:
		return 1;
	case Object::enemy:
		return enemies.size();
	case Object::rock:
		return rocks.size();
	case Object::sandPath:
		return sand.size();
	case Object::sandSand:
		return sand.size();
	}
}


bool Game::checkCollision(const sf::FloatRect& collider, 
	const Game::Object& object, const int& index) const
{
	switch (object)
	{
	case Game::Object::sandPath:
		if (sand.at(index)->getBackActive())
			return collider.intersects(sand.at(index)->getBackCollider());
		break;
	case Game::Object::sandSand:
		if (sand.at(index)->getTopActive())
			return collider.intersects(sand.at(index)->getForeCollider());
		break;
	default:
		if (getObject(object, index).getActive())
			return collider.intersects(getObject(object, index).getCollider());
		break;
	}

	return false;
}


DigDug* const Game::getDigDugPointer()
{
	return digDug;
}


Sand* const Game::getSandPointer(const int index)
{
	return sand.at(index);
}


Enemy* const Game::getEnemyPointer(const int index)
{
	return enemies.at(index);
}


Rock* const Game::getRockPointer(const int index)
{
	return rocks.at(index);
}


void Game::createScore(sf::Vector2f pos, std::string type)
{
	for (int i = 0; i < scores.size(); i++)
	{
		if (!scores.at(i)->getActive())
		{
			scores.at(i)->changeScore(pos, type);
			return;
		}
	}
}


bool Game::getActive(const Game::Object& object, const int& index) const
{
	return getObject(object, index).getActive();
}


void Game::setupObjects()
{
	digDug = new DigDug(window, this);
	digDug->setActive(false);

	ui = new UI(window);

	for (int i = 0; i < 4; i++)
	{
		enemies.push_back(new Fygar(window, this));
		enemies.at(i)->setActive(false);
	}

	for (int i = 4; i < 8; i++)
	{
		enemies.push_back(new Pooka(window, this));
		enemies.at(i)->setActive(false);
	}

	for (int i = 0; i < 5; i++)
	{
		rocks.push_back(new Rock(window, this));
		rocks.at(i)->setActive(false);
	}

	for (int i = 0; i < 224; i++)
	{
		sand.push_back(new Sand(window, this));
		sand.at(i)->setActive(false);
	}

	for (int i = 0; i < 7; i++)
	{
		scores.push_back(new Score(window, ui));
		scores.at(i)->setActive(false);
	}
}


void Game::setupLevels()
{
	// Create levels and pass in position of level file.
	levelLocations.push_back("Levels/Level1.txt");
	levelLocations.push_back("Levels/Level2.txt");
	levelLocations.push_back("Levels/Level3.txt");
	levelLocations.push_back("Levels/Level4.txt");
	levelLocations.push_back("Levels/Level5.txt");
	levelLocations.push_back("Levels/Level6.txt");
	levelLocations.push_back("Levels/Level7.txt");
	levelLocations.push_back("Levels/Level8.txt");
	levelLocations.push_back("Levels/Level9.txt");
	levelLocations.push_back("Levels/Level10.txt");
	levelLocations.push_back("Levels/Level11.txt");
	levelLocations.push_back("Levels/Level12.txt");
}


GameObject& Game::getObject(const Game::Object& object, const int& index) const
{
	switch (object)
	{
	case Object::dig:
		return *digDug;
	case Object::enemy:
		return *enemies.at(index);
	case Object::rock:
		return *rocks.at(index);
	}
}


void Game::loadLevel(int currentLevel)
{
	if (currentLevel > 98)
		currentLevel = 0;

	// Load level from level class at specified index, resetting at 12.
	int index = currentLevel % 12;
	std::ifstream levelFile(levelLocations.at(index));
	bool sandResetLevel = (this->currentLevel == currentLevel) ? false : true;

	if (!levelFile.is_open())
	{
		std::cerr << "Level at " << levelLocations.at(index) << "couldn't be opened!";
	}

	int value;
	int currentX = 0;
	int currentY = 32;
	int currentPooka = 4;
	int currentFygar = 0;
	int currentRock = 0;
	int currentSand = 0;

	while (levelFile >> value)
	{
		switch (value)
		{
		case 0:
			if (sandResetLevel)
				sand.at(currentSand)->preReset(false, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 1:
			if (sandResetLevel)
				sand.at(currentSand)->preReset(true, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 2:
			digDug->reset(sf::Vector2f(currentX, currentY));

			if (sandResetLevel)
				sand.at(currentSand)->preReset(false, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 3:
			enemies.at(currentPooka)->reset(sf::Vector2f(currentX, currentY));
			currentPooka++;

			if (sandResetLevel)
				sand.at(currentSand)->preReset(false, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 4:
			enemies.at(currentFygar)->reset(sf::Vector2f(currentX, currentY));
			currentFygar++;

			if (sandResetLevel)
				sand.at(currentSand)->preReset(false, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 5:
			rocks.at(currentRock)->reset(sf::Vector2f(currentX, currentY));
			currentRock++;

			if (sandResetLevel)
				sand.at(currentSand)->preReset(true, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		default:
			// Do nothing
			break;
		}

		currentX /= 16;
		currentY /= 16;

		if (currentX < 11)
			currentX++;
		else
		{
			currentX = 0;
			currentY++;
		}

		currentX *= 16;
		currentY *= 16;
	}

	levelFile.close();

	ui->setRound(currentLevel);

	this->currentLevel = currentLevel;

	for (Sand*& s : sand)
	{
		if (s->getBackActive())
		{
			s->reset(currentLevel, sandResetLevel);
		}
	}
}


bool Game::enemiesLeft()
{
	int enemiesLeft = false;

	for (Enemy*& enemy : enemies)
	{
		if (enemy->getActive())
			enemiesLeft = true;
	}

	return enemiesLeft;
}


void Game::update()
{
	// Run all updates
	updateObjects();

	// Check if player is dead or if all enemies are dead,
	// and load current/next level.
	if (!digDug->getActive())
		loadLevel(currentLevel);
	else if (!enemiesLeft())
		loadLevel(currentLevel + 1);

	// Draw objects
	drawObjects();
}


void Game::updateObjects()
{
	if (digDug->getActive())
		digDug->update();

	for (auto& enemy : enemies)
	{
		if (enemy->getActive())
			enemy->update();
	}

	for (auto& rock : rocks)
	{
		if (rock->getActive())
			rock->update();
	}

	for (auto& score : scores)
	{
		if (score->getActive())
			score->update();
	}
}


void Game::drawObjects()
{
	// Run all draw object method.

	ui->drawObject();

	for (auto& s : sand)
	{
		if (s->getBackActive())
			s->drawObject();
	}

	if (digDug->getActive())
		digDug->drawObject();

	for (auto& enemy : enemies)
	{
		if (enemy->getActive())
			enemy->drawObject();
	}

	for (auto& rock : rocks)
	{
		if (rock->getActive())
			rock->drawObject();
	}

	for (auto& score : scores)
	{
		if (score->getActive())
			score->drawObject();
	}
}