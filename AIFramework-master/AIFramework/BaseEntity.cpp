#include "BaseEntity.h"
#include "UtilRandom.h"


std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity()
{
	
	filename = "assets\\boid.png"; 
	Initialize();
}

BaseEntity::BaseEntity(Grid* Grid, int number)
{
	// This will be the soldier entity class constructor. This should set up getting the grid reference as well as setting the sprite filenames correctly and the number of agent.
	colourTint = sf::Color::White;
	GridForLevel = Grid;
	numberOfAgent = number;
	if (numberOfAgent == 0) 
	{
		filename = "assets\\SoldierGreen.png";
	}
	else 
	{
		filename = "assets\\SoldierRed.png";
	}

	Initialize();
}




BaseEntity::BaseEntity(std::string file) : filename(file)
{
	// with just a texture file, we default to a white tint (so no tint at all).
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour)
{
	Initialize();
}

BaseEntity::~BaseEntity()
{
	
}

void BaseEntity::Think()
{
	
	
	//Checking to see if the path has been completed
	if (CurrentPathProgression == PathToTravel.size()) 
	{
		PathToTravel.clear();
		Soldier.OnEnter();
		Soldier.OnUpdate(GridForLevel,numberOfAgent);
		for (int i = 0; i < GridForLevel->CurrentPath.size(); i++) 
		{
			PathToTravel.push_back(GridForLevel->CurrentPath[i]);
		}
		for (int i = 0; i < PathToTravel.size(); i++)
		{
			GridForLevel->GetNode(PathToTravel[i]->GridPosX, PathToTravel[i]->GridPosY)->TravelCost += 1000;
		}
		CurrentPathProgression = 0;
	}
	else if (!PathToTravel.empty()) 
	{
		//This is where it travels along the path using seek. If it reaches the range of the midpoint of the current tile traveling to it updates the statemachines x and y as well as some locations on the grid
		//and then gets the next location to travel to.
		Seek(PathToTravel[CurrentPathProgression]->Midpoint);
		if (getPosition().x < (PathToTravel[CurrentPathProgression]->Midpoint.x + 5) && getPosition().x > (PathToTravel[CurrentPathProgression]->Midpoint.x - 5))
		{
			if (getPosition().y < (PathToTravel[CurrentPathProgression]->Midpoint.y + 5) && getPosition().y > (PathToTravel[CurrentPathProgression]->Midpoint.y - 5))
			{
				Soldier.GridPosY = PathToTravel[CurrentPathProgression]->GridPosX;
				Soldier.GridPosX = PathToTravel[CurrentPathProgression]->GridPosY;
				GridForLevel->AgentLocation[numberOfAgent] = GridForLevel->GetNode(Soldier.GridPosX, Soldier.GridPosY);
				GridForLevel->GetNode(PathToTravel[CurrentPathProgression]->GridPosX, PathToTravel[CurrentPathProgression]->GridPosY)->TravelCost = 1; // reset travel cost after there.
				CurrentPathProgression += 1;
				if (CurrentPathProgression == PathToTravel.size())
				{
					//Once its reached the final location it runs the current states on exit which is what is gonna run the switch state function.
					Soldier.OnExit();

					if (Soldier.ShouldFire && Bullets[1]->ReachedLocation) 
					{
						//If the state is to fire, it sets up all of the bullet positions and what its going to be traveling to as well as bool used to make sure it can work correctly.
						Bullets[1]->setPosition(GridForLevel->GetNode(Soldier.GridPosX, Soldier.GridPosY)->Midpoint);
						
						ShotLocations[0] = GridForLevel->AgentLocation[0];
						ShotLocations[1] = GridForLevel->AgentLocation[1];
						Soldier.ShouldFire = false;
						Bullets[1]->ReachedLocation = false;
					}
				}
			}
		}
	}
	//This is like the seeking of the agent but its the bullet instead, it seeks towards the location the agent was rather then their current location as overwise it would track them.
	if (!Bullets[1]->ReachedLocation) 
	{
		if (numberOfAgent == 0) 
		{
			if (Bullets[1]->getPosition().x < (ShotLocations[1]->Midpoint.x + 5) && Bullets[1]->getPosition().x >(ShotLocations[1]->Midpoint.x - 5))
			{
				if (Bullets[1]->getPosition().y < (ShotLocations[1]->Midpoint.y + 5) && Bullets[1]->getPosition().y >(ShotLocations[1]->Midpoint.y - 5))
				{
					Bullets[1]->ReachedLocation = true;
					//If it overlaps with the enemy soldier's position it minuses one from the health.
					if (GridForLevel->AgentLocation[0] == ShotLocations[0]) 
					{
						Soldier.Health -= 1;
					}
					//Setting the bullet offscreen again when not in use.
					Bullets[1]->setPosition(800.f, 800.f);
				}
			}
			Bullets[1]->FireBullet(ShotLocations[1]->Midpoint);
			
		}
		//This does the same thing but for the other agent changing the location.
		else 
		{
			if (Bullets[1]->getPosition().x < (ShotLocations[0]->Midpoint.x + 5) && Bullets[1]->getPosition().x >(ShotLocations[0]->Midpoint.x - 5))
			{
				if (Bullets[1]->getPosition().y < (ShotLocations[0]->Midpoint.y + 5) && Bullets[1]->getPosition().y >(ShotLocations[0]->Midpoint.y - 5))
				{
					Bullets[1]->ReachedLocation = true;
					if (GridForLevel->AgentLocation[1] == ShotLocations[1])
					{
						Soldier.Health -= 1;
					}
					Bullets[1]->setPosition(700.f, 500.f);
				}
			}
			Bullets[1]->FireBullet(ShotLocations[0]->Midpoint);
			
		}
		
	}
	
	// Setting Details for the Agent for the text to draw to screen when clicked.
	EntityDetails = "Current Agent = "+ std::to_string(numberOfAgent)+ "\n"+ "Current Grid Position :\nX :"+ std::to_string(Soldier.GridPosX)+ " Y :"+ std::to_string(Soldier.GridPosY)+ "\nAmmo : "+ std::to_string(Soldier.Ammo)+ " Health : "+ std::to_string(Soldier.Health)+ "\nCurrent State:\n"+Soldier.StateString + "\nCurrent Action :\n"+ Soldier.CurrentAction;

	
}

//This is my seek function and it is used for this agent, my bullets and the helicopter.
void BaseEntity::Seek(sf::Vector2f Target)
{
	float MaxSpeed = 0.05f;

	sf::Vector2f TestTarget(400.f,400.f);
	sf::Vector2f Steer(0.f,0.f);

	sf::Vector2f CurrentPos = getPosition();
	sf::Vector2f ToLocation(Target - CurrentPos);

	float Magnitude = sqrt((ToLocation.x * ToLocation.x) + (ToLocation.y * ToLocation.y));
	ToLocation /= Magnitude;

	sf::Vector2f DesiredVelocity = (ToLocation *= MaxSpeed);

	Steer = DesiredVelocity - velocity;

	velocity += Steer;

	//These need to be swapped to make it so that the rotation works properly.
	float RotationY = Target.x - CurrentPos.x;
	float RotationX = Target.y - CurrentPos.y;

	
	float angle = atan2(RotationX, RotationY);
	setRotation(angle * (180/M_PI));
	setPosition(CurrentPos + velocity);
}

//These are get functions for the details or checking if statements.
int BaseEntity::GetHealth()
{
	return Soldier.Health;
}

void BaseEntity::SetHealth()
{
	Soldier.Health = 3;
}

int BaseEntity::GetNumberOfAgent()
{
	return numberOfAgent;
}

//The initialize sets up the agent such as position based on number of agent as well as x and y, it sets up the sprite and all transformable variables.
void BaseEntity::Initialize()
{
	
	if (numberOfAgent == 0) 
	{
		setPosition(0.0f, 0.0f);
	}
	else 
	{
		setPosition(600.f, 600.f);
		Soldier.GridPosX = 9;
		Soldier.GridPosY = 9;
		Soldier.TimesTakenCover = 2;
	}
	
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	
	sprite.setTexture(texture);
	sprite.setColor(colourTint);
	
	float angle = UtilRandom::instance()->GetRange(0, 361);
	
	setRotation(angle);
	
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	
	//Setting up the bullets, the reason a array is used is I wanted to make it so that multiple bullets could be fired in succession of if multiple were fired close to each other.
	//But it ended up feeling like cluttered, but in case I wanted to change it so that it does use multiple bullets at some point I left it as an array.
	for (int i = 0; i < 2; i++) 
	{
		Bullets[i] = new Bullet();
		
	}

	Renderables.push_back(this);
}