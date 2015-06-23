#ifndef BLOCK_H
#define BLOCK_H

#include "object.h"

class Block : public Object
{
	private:
		sf::RectangleShape rectangle, point;
	public:
		explicit Block(Room &room, double x = -128, double y = -128, double width = 32, double height = 32);

		// Accessors
		virtual bool canLoopAround() const override;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void onDeath() override;
};

#endif