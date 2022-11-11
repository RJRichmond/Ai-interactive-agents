#pragma once

#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES

#include <math.h>
//The helicopter is used to show off the steering behaviours as I use seek for movement but this also shows flee being used.
class Helicopter : public sf::Transformable
{
public:
	Helicopter();
	~Helicopter();

	void SeekTarget(sf::Vector2f Target);
	void FleeTarget(sf::Vector2f Target);

	bool isSeekingTarget;
	bool ReachedLocation;

	sf::Vector2f LocationToSeek;

	sf::RectangleShape Square;

	int AgentNumber;

	sf::Sprite sprite;
protected:
	
	sf::Vector2f SquareSize;

	sf::String filename;
	sf::Vector2f velocity;
	sf::Color colourTint;
	sf::Texture texture;

	void Initialize();
};