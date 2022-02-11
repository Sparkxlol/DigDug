#include "GameObject.h"

GameObject::GameObject()
{
	spritesheet = nullptr;
	anim = nullptr;
 	window = nullptr;
}


GameObject::~GameObject()
{
}


GameObject::GameObject(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
{
	//set position, active, and move
	position = pos;
	isActive = true;
	canMove = true;
	window = win;
	spritesheet = s;
}


bool GameObject::getActive()
{
	return isActive;
}


sf::Vector2f GameObject::getPosition()
{
	return position;
}


bool GameObject::getCanMove()
{
	return canMove;
}


sf::FloatRect* GameObject::getCollider()
{
	return &boundingBox;
}

sf::RenderWindow* GameObject::getWindow()
{
	return window;
}

void GameObject::setActive(const bool &a)
{
	isActive = a;

	return;
}


void GameObject::setPosition(const sf::Vector2f a)
{
	position = a;

	return;
}


bool GameObject::setCanMove(const bool &a)
{
	canMove = a;

	return a;
}

	
void GameObject::move(sf::Vector2f amount)
{
	//move "amount" in a direction
	spritesheet->move(amount);
	return;
}

void GameObject::drawObject()
{
	window->draw(*spritesheet);

	return;
}