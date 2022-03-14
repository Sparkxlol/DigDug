#include "Game.h"
#include <fstream>

Game::Game() : Game(nullptr)
{ }


// Deletes all objects from the game and clears each array.
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


// Sets up objects, levels and loads the first level.
Game::Game(sf::RenderWindow* window)
	: window(window), currentLevel(1), currentLives(2)
{
	// Create UI and DigDug
	setupObjects();
	setupLevels();
	loadLevel(0);
}


// Returns the length of a GameObject array, mainly for collision.
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


// Checks the collision using the passed collider
// and the specified index and object. Only checks active objects.
bool Game::checkCollision(const sf::FloatRect& collider, 
	const Game::Object& object, const int& index) const
{
	switch (object)
	{
	case Game::Object::sandPath: // Checks path of sand for collisions.
		if (sand.at(index)->getBackActive())
			return collider.intersects(sand.at(index)->getBackCollider());
		break;
	case Game::Object::sandSand: // Checks top of sand for collisions.
		if (sand.at(index)->getTopActive()) 
			return collider.intersects(sand.at(index)->getForeCollider());
		break;
	default:
		if (getObject(object, index).getActive()) // Checks all other objects.
			return collider.intersects(getObject(object, index).getCollider());
		break;
	}

	return false;
}


// Returns a const pointer to digDug.
DigDug* const Game::getDigDugPointer()
{
	return digDug;
}


// Returns a const pointer to specified sand.
Sand* const Game::getSandPointer(const int index)
{
	return sand.at(index);
}


// Returns a const pointer to specified enemy.
Enemy* const Game::getEnemyPointer(const int index)
{
	return enemies.at(index);
}


// Returns a const pointer to specified rock.
Rock* const Game::getRockPointer(const int index)
{
	return rocks.at(index);
}


// Creates a score based on the passed position and type, making sure
// it is not overriding a existing score.
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


// Returns the activity of a specified object and index.
bool Game::getActive(const Game::Object& object, const int& index) const
{
	return getObject(object, index).getActive();
}


// Creates each object that the game may ever need, and sets it unactive.
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


// Adds all 12 levels to the game from text files.
void Game::setupLevels()
{
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


// Returns the object pointer based on object and index.
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


// Loads the level from the information in a text file and
// places objects, resetting sand if necessary (on new level).
void Game::loadLevel(int currentLevel)
{
	// If level is 99, reset back to 0 to prevent triple digits.
	if (currentLevel > 98)
		currentLevel = 0;

	// Load level from level class at specified index, resetting at 12.
	int index = currentLevel % 12;
	std::ifstream levelFile(levelLocations.at(index));
	// Resets sand only if the passed level is not the same as the current level.
	bool sandResetLevel = (this->currentLevel == currentLevel) ? false : true;

	// If level cannot be opened, output an error.
	if (!levelFile.is_open())
	{
		std::cerr << "Level at " << levelLocations.at(index) << "couldn't be opened!";
		return;
	}

	int value; // Corresponds to each object/value in the text file.
	// 0 - Blank Sand, 1 - Sand, 2 - DigDug, 3 - Pooka, 4 - Fygar, 5 - Rock.
	int currentX = 0; // Specifies current x position of window.
	int currentY = 32; // Specifies current y position of window.

	// Currents used for the position in the  
	// array where an object should be changed.
	int currentPooka = 4; // Begins pookas at 4 because of enemies array.
	int currentFygar = 0;
	int currentRock = 0;
	int currentSand = 0;

	// Reset is used to change position of object and reset it to defaults.
	// If preReset includes a false parameter
	// sand is not full, otherwise sand is full.

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
			// If not a recognized character, do nothing.
			break;
		}

		// If currentX is at the 12 character in the text file,
		// increment currentY and reset currentX.
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
	
	// Sets round to passed level.
	ui->setRound(currentLevel);

	this->currentLevel = currentLevel;

	// Uses the reset of active sands after a prereset
	// due to reset needing to know the positions of surrounding sand.
	for (Sand*& s : sand)
	{
		if (s->getBackActive())
		{
			s->reset(currentLevel, sandResetLevel);
		}
	}
}


// Returns true if enemies are left in the array.
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


// Updates all objects.
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

	// Draw all objects
	drawObjects();
}


// Updates each active object.
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


// Draws each active object.
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