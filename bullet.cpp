#include "bullet.h"
#include "player.h"
#include "block.h"
#include "room.h"

#define _USE_MATH_DEFINES
#include <math.h>

Bullet::Bullet(Room &room, double x, double y, double s, double a)
	: Object(
			room, x, y, 6, 6,
			0, 0, false,
			0.00008, 0.15
	  ),
	speed(s), angle(a)
{
	spr.setTexture(room.textureManager.getRef("bullet"));
	spr.setColor(sf::Color(255, 0, 0));
	spr.setRotation(angle);
	spr.setOrigin(sf::Vector2f(4, 3));

	double rad = angle * M_PI / 180.0;
	dx = speed * cos(rad);
	dy = speed * sin(rad);
}

void Bullet::draw(sf::RenderWindow &window)
{
	if (timer.getElapsedTime().asMilliseconds() < 60) return;

	spr.setPosition(x, y);
	window.draw(spr);
}

void Bullet::update(sf::Time deltaTime)
{
	double mstime = deltaTime.asMilliseconds();

	// Gravity
	dy += gravity * mstime;
	if (dy > maxFallSpeed) dy = maxFallSpeed;

	// Update position
	if (placeFree(x + (dx * mstime), y + (dy * mstime)))
	{
		x += dx * mstime;
		y += dy * mstime;
	}
	else
	{
		kill();
		return;
	}

	loopAroundMap();

	if (timer.getElapsedTime().asMilliseconds() < 75) return;

	std::vector<Object*> col = allCollisions(x, y);
	for (Object* obj : col)
	{
		Player* plyr = dynamic_cast<Player*>(obj);
		if (plyr != nullptr)
		{
			plyr->respawn();
			kill();
			return;
		}
	}
}

void Bullet::onDeath()
{
}
