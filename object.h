#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#include "object_id.h"
class Room;

class Object
{
	public:
		explicit Object(
				Room &room,
				double x = 0, double y = 0,
				double width = 0, double height = 0,
				double dx = 0, double dy = 0,
				bool solid = true,
				double gravity = 0,
				double maxFallSpeed = 0
		);
		virtual ~Object();

		// Accessors
		object_id getId() const;
		int getDepth() const;
		bool isSolid() const;
		bool shouldDelete() const;
		double getX() const;
		double getY() const;
		virtual bool canLoopAround() const;
		virtual bool canCollideWith(const Object* obj) const;

		virtual sf::FloatRect getRect() const;
		bool placeFree(double xx, double yy) const;
		std::vector<Object*> allCollisions(double xx, double yy) const;

		// Mutators
		void setId(object_id id_n);
		void setX(double xx);
		void setY(double yy);
		void setDepth(int depth);

		// Actions
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime);
		virtual void kill();
		virtual void onDeath() = 0;
		void applyGravityOnce(sf::Time deltaTime);
		void applyVelocityOnce(sf::Time deltaTime, double velX, double velY);
		virtual void loopAroundMap();
		void pushOutOfSolids();

		// Variables
		bool destroyedByServer;

	protected:
		Room &room;
		sf::Sprite sprite;
		double x, y, width, height, dx, dy, gravity, maxFallSpeed;
		int depth;
		bool solid, del;
		object_id _id;
};

#endif