#include "GameObject.h"


// GameObject default constructor which intializes pointers to nullptr
// and booleans to false.
GameObject::GameObject() : GameObject(nullptr, sf::Vector2f(0, 0), nullptr)
{ 
	isActive = false;
	canMove = false;
}


GameObject::~GameObject()
{
}


// Sets booleans to true and creates animator with passed in spritesheet.
GameObject::GameObject(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
	: anim(s), window(win), spritesheet(s), position(pos)
{
	isActive = true;
	canMove = false;
}


// Returns if object is alive(true)/dead(false)
bool GameObject::getActive()
{
	return isActive;
}


// Returns the position of the spritesheet.
sf::Vector2f GameObject::getPosition()
{
	return position;
}


// Returns if the object can currently move.
bool GameObject::getCanMove()
{
	return canMove;
}


// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect* GameObject::getCollider()
{
	return &boundingBox;
}


// Sets if object is dead/alive.
void GameObject::setActive(const bool &a)
{
	isActive = a;

	return;
}


// Sets position of the spritesheet.
void GameObject::setPosition(const sf::Vector2f a)
{
	position = a;

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
	window->draw(*spritesheet);

	return;
}