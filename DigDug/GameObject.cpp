#include "GameObject.h"
#include "Game.h"


// GameObject default constructor which intializes pointers to nullptr
// and booleans to false.
GameObject::GameObject() : GameObject(nullptr, nullptr)
{ 
	isActive = false;
	canMove = false;
	direction = 0;
}


GameObject::~GameObject()
{
	for (int i = 0; i < 7; i++)
	{
		delete soundFiles.at(i);
		delete sounds.at(i);
	}
}


// Sets booleans to true and creates animator with passed in spritesheet.
GameObject::GameObject(sf::RenderWindow* win, Game* game)
	: window(win), game(game)
{
	isActive = false;
	canMove = false;
	direction = 0;


	soundNames.push_back("Sounds/in-game-music.ogg");
	soundNames.push_back("Sounds/shot.ogg");
	soundNames.push_back("Sounds/pump.ogg");
	soundNames.push_back("Sounds/digdug_death.ogg");
	soundNames.push_back("Sounds/monster_blow.ogg");
	soundNames.push_back("Sounds/round_clear.ogg");
	soundNames.push_back("Sounds/game_over.ogg");


	for (int i = 0; i < 7; i++)
	{
		soundFiles.push_back(new sf::SoundBuffer());
		sounds.push_back(new sf::Sound());
	}


	for (int i = 1; i < 7; i++)
	{
		if (!soundFiles.at(i)->loadFromFile(soundNames.at(i)))
			std::cout << "error loading in-game music" << std::endl; // error
	}







	/*
	//clear sound buffer and sound vectors
	for (int i = 0; i < 7; i++)
	{
		soundFiles.push_back(sf::SoundBuffer());
		sounds.push_back(sf::Sound());
	}

	//loads sound effects into sound buffer
	if (!soundFiles.at(0).loadFromFile("Sounds/in-game-music.ogg"))
		std::cout << "error loading in-game music" << std::endl; // error
	sounds.at(0).setBuffer(soundFiles.at(0));
	if (!soundFiles.at(1).loadFromFile("Sounds/shot.ogg"))
		std::cout << "error loading shot sound effect" << std::endl; // error
	sounds.at(1).setBuffer(soundFiles.at(1));
	if (!soundFiles.at(2).loadFromFile("Sounds/pump.ogg"))
		std::cout << "error loading pump sound effect" << std::endl; // error
	sounds.at(2).setBuffer(soundFiles.at(2));
	if (!soundFiles.at(3).loadFromFile("Sounds/digdug_death.ogg"))
		std::cout << "error loading digdug death sound effect" << std::endl; // error
	sounds.at(3).setBuffer(soundFiles.at(3));
	if (!soundFiles.at(4).loadFromFile("Sounds/monster_blow.ogg"))
		std::cout << "error loading monster blow sound effect" << std::endl; // error
	sounds.at(4).setBuffer(soundFiles.at(4));
	if (!soundFiles.at(5).loadFromFile("Sounds/round_clear.ogg"))
		std::cout << "error loading digdug death sound effect" << std::endl; // error
	sounds.at(5).setBuffer(soundFiles.at(5));
	if (!soundFiles.at(6).loadFromFile("Sounds/game_over.ogg"))
		std::cout << "error loading digdug death sound effect" << std::endl; // error
	sounds.at(6).setBuffer(soundFiles.at(6));
	*/
}


// Returns the direction of the animation and object
int GameObject::getDirection()
{
	return direction;
}


// Returns if object is alive(true)/dead(false)
bool GameObject::getActive()
{
	return isActive;
}


// Returns the position of the spritesheet.
sf::Vector2f GameObject::getPosition()
{
	return spritesheet.getPosition();
}


// Returns if the object can currently move.
bool GameObject::getCanMove()
{
	return canMove;
}



// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect& GameObject::getCollider()
{
	// Smaller than sprite to allow objects to not collide
	// with things with tiny extrusions. Ex. Dig Dug & Sand
	boundingBox = spritesheet.getGlobalBounds();
	boundingBox.left += .125f;
	boundingBox.width -= .25f;
	boundingBox.top += .125f;
	boundingBox.height -= .25f;

	return boundingBox;
}


// Sets the direction of the animation and object
void GameObject::setDirection(const int &dir)
{
	direction = dir;

	return;
}


// Sets if object is dead/alive.
void GameObject::setActive(const bool &active)
{
	isActive = active;

	return;
}


// Sets position of the spritesheet.
void GameObject::setPosition(const sf::Vector2f pos)
{
	spritesheet.setPosition(pos);

	return;
}


// Sets if object is currently allowed to move.
bool GameObject::setCanMove(const bool &a)
{
	canMove = a;

	return a;
}


// Move the object in x and y direction.
void GameObject::move(sf::Vector2f amount)
{
	//move "amount" in a direction
	spritesheet.move(amount);

	return;
}


// Draws the object onto the window.
void GameObject::drawObject()
{
	window->draw(spritesheet);

	return;
}


// Resets the object to default values.
void GameObject::reset(sf::Vector2f pos)
{
	setDirection(right);
	setActive(true);
	setPosition(pos);
	setCanMove(true);
}


void GameObject::playSound(const int choice)
{
	sounds.at(choice)->setBuffer(*soundFiles.at(choice));
	if (!soundFiles.at(choice)->loadFromFile(soundNames.at(choice)))
		std::cout << "error loading " << soundNames.at(choice) << std::endl; // error

	sounds.at(choice)->play();
}


void GameObject::stopSound(const int choice)
{
	sounds.at(choice)->stop();
	clearSound(choice);
}


void GameObject::pauseSound(const int choice)
{
	sounds.at(choice)->pause();
}


void GameObject::clearSound(const int choice)
{
	sounds.at(choice) = nullptr;
	soundFiles.at(choice) = nullptr;
}
