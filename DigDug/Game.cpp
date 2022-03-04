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
	digDug->setActive(true);

	ui = new UI(window);

	for (int i = 0; i < 3; i++)
	{
		fygars.push_back(new Fygar(window, this));
		pookas.push_back(new Pooka(window, this));
	}

	for (int i = 0; i < 5; i++)
	{
		rocks.push_back(new Rock(window, this));
	}

	for (int i = 0; i < 224; i++)
	{
		sand.push_back(new Sand(window));
	}

	for (int i = 0; i < 7; i++)
	{
		scores.push_back(new Score(window, ui));
	}
}


void Game::setupLevels()
{
	// Create levels and pass in position of level file.
	levelLocations.push_back("Levels/Level1.txt");
	sandLocations.push_back("Levels/Sand1.txt");
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
	std::ifstream sandFile(sandLocations.at(index));

	if (!levelFile.is_open() || !sandFile.is_open())
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
		case 2:
			digDug->setActive(true);
			digDug->setPosition(sf::Vector2f(currentX, currentY));
			break;
		case 3:
			pookas.at(currentPooka)->setActive(true);
			pookas.at(currentPooka)->setPosition(sf::Vector2f(currentX, currentY));
			currentPooka++;
			break;
		case 4:
			fygars.at(currentFygar)->setActive(true);
			fygars.at(currentFygar)->setPosition(sf::Vector2f(currentX, currentY));
			currentFygar++;
			break;
		case 5:
			rocks.at(currentRock)->setActive(true);
			rocks.at(currentRock)->setPosition(sf::Vector2f(currentX, currentY));
			currentRock++;
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


	currentX = 0;
	currentY = 32;

	while (sandFile >> value)
	{
		sand.at(currentSand)->reset(0, value, sf::Vector2f(currentX, currentY), 0, 0);

		sand.at(currentSand)->setActive(true);
		currentSand++;

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
}


bool Game::enemiesLeft()
{
	// Check through each array for any active enemies.
	return 0;
}


void Game::update()
{
	// Run all updates
	updateObjects();

	// Check if player is dead or if all enemies are dead,
	// and load current/next level.
	//

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