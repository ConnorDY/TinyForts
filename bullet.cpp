#include "bullet.h"
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
	spr.setPosition(x, y);
	window.draw(spr);
}

void Bullet::update(sf::Time deltaTime)
{
	// Gravity
	applyGravityOnce(deltaTime);
	if (dy == 0) kill();

	double mstime = deltaTime.asMicroseconds() / 1000.0f;

	// Update Y
	if (placeFree(x, y + (dx * mstime))) y += dy * mstime;
	else
	{
		kill();
		return;
	}

	// Update X
	if (placeFree(x + (dx * mstime), y)) x += dx * mstime;
	else
	{
		kill();
		return;
	}

	loopAroundMap();
}

void Bullet::onDeath()
{
}