#include "Game.h"

Game::Game() : Game(nullptr)
{ }


Game::~Game()
{
	// Delete digDug and UI
	// Delete all arrays using for/for-each loop
}


Game::Game(sf::RenderWindow* window)
	: window(window), currentLevel(1) 
{
	// Create UI and DigDug
	
	// Setup Textures
	// Setup Levels

	// Load Level 1, maybe Main Menu eventually.
}


void Game::setupObjects()
{
	// Creates sprites from file, and creates each of the GameObjects.
}


void Game::setupLevels()
{
	// Create levels and pass in position of level file.
}


void Game::loadLevel(int index)
{
	// Load level from level class at specified index.
}


bool Game::enemiesLeft()
{
	// Check through each array for any active enemies.
	return 0;
}


void Game::playerInputs()
{
	// Check for pause menu inputs eventually.
}


void Game::update()
{
	// Run all updates

	// Check if player is dead or if all enemies are dead,
	// and load current/next level.

	// Draw objects
}


void Game::drawObjects()
{
	// Run all draw object method.
}