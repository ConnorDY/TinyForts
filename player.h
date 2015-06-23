#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

struct network_player;
struct network_bullet;

class Player : public Object
{
	private:
		sf::Sprite spr;
		std::vector<sf::IntRect> const *animation;
		double moveSpeed, jumpSpeed, frame, angle;
		int scale;
		sf::Vector2f origin;
		unsigned int numBullets;

	public:
		Player(Room &room, double x, double y);

		// Accessors
		network_player getNetworkPlayer() const;

		// Actions
		void respawn();
		void move(int dir, sf::Vector2i m);
		void jump();
		network_bullet shoot();
		void setAnimation(std::vector<sf::IntRect> const &newAnim);
		void updateAnimation(sf::Time deltaTime);
		virtual void loopAroundMap() override;
		virtual void draw(sf::RenderWindow &window) override;
		virtual void drawOther(sf::RenderWindow &window, double x, double y, double angle, int frame, int scale);
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif