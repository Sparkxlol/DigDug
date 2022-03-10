#include "Game.h"
#include <fstream>

Game::Game() : Game(nullptr)
{ }


Game::~Game()
{
	delete digDug;

	for(int i = 0; i < fygars.size(); i++)
	{
		delete fygars.at(i);
	}

	for(int i = 0; i < pookas.size(); i++)
	{
		delete pookas.at(i);
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

	fygars.clear();
	pookas.clear();
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
	case Object::fygar:
		return fygars.size();
	case Object::pooka:
		return pookas.size();
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


DigDug* Game::getDigDugPointer()
{
	return digDug;
}


Sand* Game::getSandPointer(const int index)
{
	return sand.at(index);
}


Fygar* Game::getFygarPointer(const int index)
{
	return fygars.at(index);
}


Pooka* Game::getPookaPointer(const int index)
{
	return pookas.at(index);
}


Rock* Game::getRockPointer(const int index)
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

	for (int i = 0; i < 3; i++)
	{
		fygars.push_back(new Fygar(window, this));
		fygars.at(i)->setActive(false);
		pookas.push_back(new Pooka(window, this));
		pookas.at(i)->setActive(false);
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
}


GameObject& Game::getObject(const Game::Object& object, const int& index) const
{
	switch (object)
	{
	case Object::dig:
		return *digDug;
	case Object::fygar:
		return *fygars.at(index);
	case Object::pooka:
		return *pookas.at(index);
	case Object::rock:
		return *rocks.at(index);
	}
}


void Game::loadLevel(int index)
{
	// Load level from level class at specified index.
	std::ifstream levelFile(levelLocations.at(index));
	bool sandResetLevel = (currentLevel == index) ? false : true;

	if (!levelFile.is_open())
	{
		std::cerr << "Level at " << levelLocations.at(index) << "couldn't be opened!";
	}

	int value;
	int currentX = 0;
	int currentY = 32;
	int currentPooka = 0;
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
			pookas.at(currentPooka)->reset(sf::Vector2f(currentX, currentY));
			currentPooka++;

			if (sandResetLevel)
				sand.at(currentSand)->preReset(false, sf::Vector2f(currentX, currentY));
			currentSand++;
			break;
		case 4:
			fygars.at(currentFygar)->reset(sf::Vector2f(currentX, currentY));
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

	ui->setRound(index + 1);

	currentLevel = index;

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

	for (Pooka*& pooka : pookas)
	{
		if (pooka->getActive())
			enemiesLeft = true;
	}

	for (Fygar*& fygar : fygars)
	{
		if (fygar->getActive())
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

	for (auto& fygar : fygars)
	{
		if (fygar->getActive())
			fygar->update();
	}

	for (auto& pooka : pookas)
	{
		if (pooka->getActive())
			pooka->update();
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

	for (auto& fygar : fygars)
	{
		if (fygar->getActive())
			fygar->drawObject();
	}

	for (auto& pooka : pookas)
	{
		if (pooka->getActive())
			pooka->drawObject();
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