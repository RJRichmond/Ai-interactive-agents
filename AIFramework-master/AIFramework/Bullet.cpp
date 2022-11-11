#include "Bullet.h"
#include "UtilRandom.h"

Bullet::Bullet()
{
	
	colourTint = sf::Color::White;
	Initialize();
}

Bullet::~Bullet()
{
}

void Bullet::FireBullet(sf::Vector2f Target)
{
	
	float MaxSpeed = 0.2f;

	sf::Vector2f TestTarget(400.f, 400.f);
	sf::Vector2f Steer(0.f, 0.f);

	sf::Vector2f CurrentPos = getPosition();
	sf::Vector2f ToLocation(Target - CurrentPos);

	float Magnitude = sqrt((ToLocation.x * ToLocation.x) + (ToLocation.y * ToLocation.y));
	ToLocation /= Magnitude;

	sf::Vector2f DesiredVelocity = (ToLocation *= MaxSpeed);

	Steer = DesiredVelocity - velocity;

	velocity += Steer;

	float angle = atan2f(velocity.y, velocity.x * (180 / M_PI));
	setRotation(angle);
	setPosition(CurrentPos + velocity);

	
}

void Bullet::Draw()
{
}

void Bullet::Initialize()
{
	setPosition(800.0f, 800.0f);
	
	Square.setSize(sf::Vector2f(10.f, 10.f));
	Square.setFillColor(sf::Color::Yellow);
	
	Square.setOrigin(Square.getLocalBounds().width / 2, Square.getLocalBounds().height / 2);

	ReachedLocation = true;

	
}
