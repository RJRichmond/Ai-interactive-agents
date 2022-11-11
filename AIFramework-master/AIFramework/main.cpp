#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "BaseEntity.h"
#include "UtilRandom.h"
#include "Grid.h"
#include "Helicopter.h"

int main()
{
	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Framework");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	std::cout << "Constructed SFML Window" << std::endl;

	Grid LevelGrid;
	LevelGrid.CreateGrid();

	Helicopter Heli;

	sf::Font font;

	if (!font.loadFromFile("assets\\BebasNeue-Regular.ttf")) 
	{
	
	}

	sf::Text text;

	text.setFont(font);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
	text.setPosition(610.f, 30.f);

	//Make 2 soldier agents.
	for (int i = 0; i < 2; i++) 
	{
		
		//Make the soldier agent
		BaseEntity *boid = new BaseEntity(&LevelGrid, i);

	}

	

	// our game loop
	while (window.isOpen())
	{
		// poll for events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window before drawing anything to it
		window.clear();

		LevelGrid.Draw(window);

		
		// update all our entities
		for ( auto entity : BaseEntity::Renderables )
		{
			//Update the soldiers and the helicopter
			entity->Think();
			if (Heli.isSeekingTarget)
			{
				if (!Heli.ReachedLocation)
				{
					Heli.SeekTarget(Heli.LocationToSeek);
					if (Heli.getPosition().x < Heli.LocationToSeek.x + 5 && Heli.getPosition().x > Heli.LocationToSeek.x - 5)
					{
						if (Heli.getPosition().y < Heli.LocationToSeek.y + 5 && Heli.getPosition().y > Heli.LocationToSeek.y - 5)
						{
							
							if (entity->GetNumberOfAgent() == Heli.AgentNumber) 
							{
								Heli.ReachedLocation = true;
								entity->SetHealth();
							}
						}
					}
				}
				else
				{
					Heli.FleeTarget(Heli.LocationToSeek);
					if (Heli.getPosition().x < Heli.LocationToSeek.x + 1000 && Heli.getPosition().x > Heli.LocationToSeek.x + 400)
					{
						if (Heli.getPosition().y <= Heli.LocationToSeek.y + 1000 && Heli.getPosition().y >= Heli.LocationToSeek.y + 400)
						{
							Heli.isSeekingTarget = false;
							Heli.setPosition(700.f, 500.f);
						}
					}
				}

				
			}
			else 
			{
				if (entity->GetHealth() <= 0)
				{
					Heli.LocationToSeek = LevelGrid.AgentLocation[entity->GetNumberOfAgent()]->Midpoint;
					Heli.AgentNumber = entity->GetNumberOfAgent();
					Heli.isSeekingTarget = true;
					Heli.ReachedLocation = false;
					Heli.setPosition(800.f, 800.f);

				}
			}
			

			//We can check here if the sprite has been clicked on.
			auto mouse_pos = sf::Mouse::getPosition(window);
			auto translated_pos = window.mapPixelToCoords(mouse_pos);
			
			if (entity->getPosition().x <= translated_pos.x + 32 && entity->getPosition().x >= translated_pos.x - 32)
			{
				
				if (entity->getPosition().y <= translated_pos.y + 32 && entity->getPosition().y >= translated_pos.y - 32)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
					{
						text.setString(entity->EntityDetails);
					}
			
				}
				
			}
		}

		// This is where everything is being drawn
		for (auto entity : BaseEntity::Renderables)
		{
			window.draw(entity->GetSprite(), entity->getTransform());
			for (int i = 0; i < 2; i++) 
			{
				window.draw(entity->Bullets[i]->Square, entity->Bullets[i]->getTransform());
			}
		}
		window.draw(Heli.sprite, Heli.getTransform());

		window.draw(text);

		// swap the frame buffers
		window.display();
	}

	LevelGrid.DestroyGrid();

	return 0;
}
