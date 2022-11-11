#pragma once

#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES

#include <math.h>



class Bullet :public sf::Transformable 
{
public:
	Bullet();
	~Bullet();

	void FireBullet(sf::Vector2f Target);

	void Draw();

	sf::RectangleShape Square;

	bool ReachedLocation;

protected:
	
	sf::Vector2f SquareSize;

	sf::Sprite sprite;
	sf::String filename;
	sf::Vector2f velocity;
	sf::Color colourTint;
	sf::Texture texture;

	void Initialize();
};