#include "bullet.h"
#include "block.h"
#include "room.h"

#define _USE_MATH_DEFINES
#include <math.h>

Bullet::Bullet(Room &room, double x, double y, double s, double a)
	: Object(
			room, x, y, 6, 6,
			0, 0, false,
			0.00007, 0.1
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
	std::vector<Object*> col = allCollisions(x + dx, y + dy);
	for (Object *obj : col)
	{
		if (dynamic_cast<Block*>(obj) != nullptr)
		{
			kill();
			return;
		}
	}

	Object::update(deltaTime);
}

void Bullet::onDeath()
{
}