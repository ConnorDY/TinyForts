#include "block.h"
#include "globals.h"

Block::Block(Room &room, double x, double y, double width, double height)
	: Object(
			room, x, y, width, height,
			0, 0, true,
			0, 0
	  ),
	  rectangle(sf::Vector2f(width, height)), point(sf::Vector2f(1, 1))
{
	rectangle.setFillColor(sf::Color(75, 125, 75, 150));
	point.setFillColor(sf::Color::White);
}


/* Accessors */
bool Block::canLoopAround() const
{
	return false;
}


/* Actions */
void Block::draw(sf::RenderWindow &window)
{
	#ifndef DEBUG_MODE
	return;
	#endif

	rectangle.setPosition(x, y);
	window.draw(rectangle);

	point.setPosition(x, y);
	window.draw(point);
}

void Block::onDeath()
{
}