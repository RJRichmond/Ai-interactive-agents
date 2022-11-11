#include "Helicopter.h"

Helicopter::Helicopter()
{
	colourTint = sf::Color::White;
	Initialize();
}

Helicopter::~Helicopter()
{
}

void Helicopter::SeekTarget(sf::Vector2f Target)
{
	float MaxSpeed = 0.07f;

	sf::Vector2f TestTarget(400.f, 400.f);
	sf::Vector2f Steer(0.f, 0.f);

	sf::Vector2f CurrentPos = getPosition();
	sf::Vector2f ToLocation(Target - CurrentPos);

	float Magnitude = sqrt((ToLocation.x * ToLocation.x) + (ToLocation.y * ToLocation.y));
	ToLocation /= Magnitude;

	sf::Vector2f DesiredVelocity = (ToLocation *= MaxSpeed);

	Steer = DesiredVelocity - velocity;

	velocity += Steer;
	float RotationY = Target.x - CurrentPos.x;
	float RotationX = Target.y - CurrentPos.y;

	
	float angle = atan2(RotationX, RotationY);
	setRotation(angle * (180 / M_PI));
	setPosition(CurrentPos + velocity);
}

//Flee works the same as seek but with the to location reversed.
void Helicopter::FleeTarget(sf::Vector2f Target)
{
	float MaxSpeed = 0.07f;

	sf::Vector2f TestTarget(400.f, 400.f);
	sf::Vector2f Steer(0.f, 0.f);

	sf::Vector2f CurrentPos = getPosition();
	sf::Vector2f ToLocation(CurrentPos - Target);

	float Magnitude = sqrt((ToLocation.x * ToLocation.x) + (ToLocation.y * ToLocation.y));
	ToLocation /= Magnitude;

	sf::Vector2f DesiredVelocity = (ToLocation *= MaxSpeed);

	Steer = DesiredVelocity - velocity;

	velocity += Steer;

	float RotationY = Target.x - CurrentPos.x;
	float RotationX = Target.y - CurrentPos.y;

	//float angle = atan2f(velocity.y, velocity.x * (180 / M_PI));
	float angle = atan2(RotationX, RotationY);
	setRotation(angle * (180 / M_PI));
	setPosition(CurrentPos + velocity);
}

void Helicopter::Initialize()
{
	setPosition(800.0f, 800.0f);

	Square.setSize(sf::Vector2f(32.f, 32.f));
	Square.setFillColor(sf::Color::Blue);

	filename = "assets\\Helicopter.png";

	texture.loadFromFile(filename);
	texture.setSmooth(true);
	
	sprite.setTexture(texture);
	
	sprite.setColor(colourTint);

	sprite.setOrigin(Square.getLocalBounds().width / 2, Square.getLocalBounds().height / 2);

	ReachedLocation = false;

}
