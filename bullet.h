#ifndef BULLET_H
#define BULLET_H

#include "object.h"

class Bullet : public Object
{
	private:
		sf::Sprite spr;
		double speed, angle;

	public:
		Bullet(Room &room, double x, double y, double speed, double angle);

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif