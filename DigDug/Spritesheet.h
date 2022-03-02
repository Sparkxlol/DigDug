#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Spritesheet : public sf::Sprite
{
public:
	Spritesheet();
	void setupSprite(std::string, sf::Vector2i, sf::Vector2i);
	virtual bool loadSprite(int currentSprite);
	void setSize(sf::Vector2i, sf::Vector2i, int);

private:
	sf::Texture texture;
	sf::Vector2i totalSize;
	sf::Vector2i spriteSize;
	std::string textureFile;
	sf::Vector2i startPos;
};

