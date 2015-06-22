#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

class Player : public Object
{
	private:
		sf::Sprite spr;
		std::vector<sf::IntRect> const *animation;
		double moveSpeed, jumpSpeed, frame, angle;
		int scale;
		sf::Vector2f origin;

	public:
		Player(Room &room, double x, double y);

		// Accessors
		double getAngle() const;

		// Actions
		void move(int dir, sf::Vector2i m);
		void jump();
		void shoot();
		void setAnimation(std::vector<sf::IntRect> const &newAnim);
		void updateAnimation(sf::Time deltaTime);
		virtual void loopAroundMap() override;
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif