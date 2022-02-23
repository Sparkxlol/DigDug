#include "GameObject.h"


// GameObject default constructor which intializes pointers to nullptr
// and booleans to false.
GameObject::GameObject() : GameObject(nullptr, sf::Vector2f(0, 0), nullptr)
{ 
	isActive = false;
	canMove = false;
	direction = 0;
}


GameObject::~GameObject()
{
}


// Sets booleans to true and creates animator with passed in spritesheet.
GameObject::GameObject(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
	: anim(s), window(win), spritesheet(s)
{
	setPosition(pos);

	isActive = true;
	canMove = false;
	direction = 0;
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
	return spritesheet->getPosition();
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
	boundingBox = spritesheet->getGlobalBounds();

	return boundingBox;
}


/*
// Returns the animator to set animations / frame changes
Animator* GameObject::getAnimator()
{
	return &anim;
}


// Returns the window to allow overridden drawObjects to function
sf::RenderWindow* GameObject::getWindow()
{
	return window;
}
*/


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
	spritesheet->setPosition(pos);

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
	spritesheet->move(amount);

	return;
}



// Draws the object onto the window.
void GameObject::drawObject()
{
	if(isActive)
		window->draw(*spritesheet);

	return;
}