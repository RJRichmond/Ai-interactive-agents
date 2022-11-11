#pragma once



#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "StateMachine.h"
#include "Grid.h"
#include "Bullet.h"



#define _USE_MATH_DEFINES

#include <math.h>

class BaseEntity : public sf::Transformable
{
	

public:
	
	static std::vector<BaseEntity*> Renderables;

	
public:
	
	explicit BaseEntity();
	
	explicit BaseEntity(Grid* Grid, int number); // This is the version of base entity which I will be using as it gets a reference to the grid.

	
	explicit BaseEntity(std::string file);
	
	explicit BaseEntity(std::string file, sf::Color colour);

	
	virtual ~BaseEntity();

	
	virtual void Think();

	
	sf::Sprite GetSprite() { return sprite; }

	
	void SetColour(sf::Color colour) { colourTint = colour; }

	
	void SetTextureFile(std::string file) { filename = file; }

	void Seek(sf::Vector2f Target);
	
	int GetHealth();

	void SetHealth();

	int GetNumberOfAgent();

	Bullet* Bullets[2];

	std::vector<GridNode*> PathToTravel;

	std::string EntityDetails;

protected:
	std::string filename;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Color colourTint;

	sf::Vector2f velocity;

	Grid* GridForLevel;

	GridNode* ShotLocations[2];

	int numberOfAgent = 0;

	int CurrentPathProgression = 0;

	StateMachine Soldier; // This is the statemachine which is going to drive our soldier entity.
	
protected:
	
	virtual void Initialize();
};


