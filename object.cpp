#include "object.h"
#include "room.h"
#include "globals.h"

Object::Object(Room &room, double x, double y, double width, double height, double dx, double dy, bool solid, double gravity, double maxFallSpeed)
	: room(room), x(x), y(y),
	  dx(dx), dy(dy),
	  width(width), height(height),
	  gravity(gravity),
	  maxFallSpeed(maxFallSpeed),
	  depth(0),
	  solid(solid),
	  del(false),
	  destroyedByServer(false)
{
}

Object::~Object()
{

}

/* Accessors */
object_id Object::getId() const { return _id; }
int Object::getDepth() const { return depth; }
bool Object::isSolid() const { return solid; }
bool Object::shouldDelete() const { return del; }
double Object::getX() const { return x; }
double Object::getY() const { return y; }

sf::FloatRect Object::getRect() const
{
	return sf::FloatRect(x, y, width, height);
}

bool Object::placeFree(double xx, double yy) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	//if (room.exceedsHorizontalBounds(temp_rect)) return false;

	auto const &objects = room.getObjects();
	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect()) && this->canCollideWith(elem) && elem->canCollideWith(this);
	});
}

std::vector<Object*> Object::allCollisions(double xx, double yy) const
{
	std::vector<Object*> result;
	auto myRect = getRect();
	sf::FloatRect temp_rect(xx + myRect.left - x, yy + myRect.top - y, myRect.width, myRect.height);

	auto const &objects = room.getObjects();
	std::copy_if(objects.begin(), objects.end(), std::back_inserter(result), [&](Object* const &elem)
	{
		return this != elem && temp_rect.intersects(elem->getRect());
	});
	return result;
}

bool Object::canCollideWith(Object const*) const
{
	return true;
}

bool Object::canLoopAround() const
{
	return true;
}


/* Mutators*/
void Object::setId(object_id id_n)
{
	_id = id_n;
}

void Object::setX(double xx)
{
	x = xx;
}

void Object::setY(double yy)
{
	y = yy;
}

void Object::setDepth(int d)
{
	depth = d;
}


/* Actions */
void Object::update(sf::Time deltaTime)
{
	applyGravityOnce(deltaTime);
	applyVelocityOnce(deltaTime, dx, dy);
	loopAroundMap();
}

void Object::applyGravityOnce(sf::Time deltaTime)
{
	if (gravity == 0) return;	

	double mstime = deltaTime.asMicroseconds() / 1000.0f;

	if (placeFree(x, y + 1)) dy += gravity * mstime;
	else if (dy > 0.0f) dy = 0.0f;

	if (dy > maxFallSpeed) dy = maxFallSpeed;
}

void Object::applyVelocityOnce(sf::Time deltaTime, double velX, double velY)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0f;

	// Update Y
	for (double i = fabs(velY) * mstime; i > 0; i--)
	{
		double j = copysign(i, velY);
		if (placeFree(x, y + j))
		{
			y += j;
			break;
		}
	}

	bool shouldSlopeWalk = false;//velY == 0 && !placeFree(x, y + 1);
	// Update X
	for (double i = fabs(velX) * mstime; i > 0; i--)
	{
		double xAdjust = copysign(i, velX);

		// We search for a y-adjustment, these are our bounds
		double yAdjustStart = 0;
		double yAdjustEnd = 0;

		if (shouldSlopeWalk)
		{
			yAdjustStart = round(maxFallSpeed * mstime);
			yAdjustEnd = -4;
		}

		// Find an appropriate y-adjustment for slope walking
		for (double yAdjust = yAdjustStart; yAdjust >= yAdjustEnd; yAdjust--)
		{
			if (placeFree(x + xAdjust, y + yAdjust))
			{
				x += xAdjust;
				y += yAdjust;

				return;
			}
		}
	}
}

void Object::loopAroundMap()
{
	if (!canLoopAround()) return;

	if (x < -1) x += room.getWidth();
	else if (x > room.getWidth() + 1) x -= room.getWidth();

	if (y < -1) y += room.getHeight();
	else if (y > room.getHeight() + 1) y -= room.getHeight();
}

void Object::pushOutOfSolids()
{
	// Move out of any solid objects
	for (auto obj : allCollisions(x, y))
		if (obj->isSolid())
		{
			auto objbbox = obj->getRect();
			y = std::min<int>(y, objbbox.top - height);
		}
}

void Object::kill()
{
	if (del) return;
	onDeath();
	del = true;
}
