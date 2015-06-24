#include "player.h"
#include "room.h"
#include "bullet.h"
#include "globals.h"
#include "network_player.h"
#include "network_bullet.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ARM_X 2
#define ARM_Y 15

namespace
{
	double const ANIMATION_SPEED = 10;

	struct Animations
	{
		std::vector<sf::IntRect>
			STILL, WALKING;

		Animations()
		{
			STILL.emplace_back(0, 0, 32, 32);

			WALKING.push_back(STILL[0]);
			WALKING.emplace_back(32, 0, 32, 32);
			WALKING.push_back(STILL[0]);
			WALKING.emplace_back(64, 0, 32, 32);
		}
	};

	Animations ANIM;
}

Player::Player(Room &room, double x, double y)
	: Object(
			room, x, y, 12, 25,
			0, 0, false,
			0.0014, 0.4
	  ),
	animation(&ANIM.STILL), frame(0), scale(1), angle(0),
	moveSpeed(0.17), jumpSpeed(0.51), numBullets(0)
{
	spr.setTexture(room.textureManager.getRef("player"));
	spr.setColor(sf::Color(255, 0, 0));
	origin = sf::Vector2f(13, 29);

	setDepth(-1);
}


/* Accessors */
network_player Player::getNetworkPlayer() const
{
	network_player p;
	p.x = x;
	p.y = y;
	p.dx = dx;
	p.dy = dy;
	p.angle = angle;
	p.frame = floor(frame);
	p.scale = scale;
	return p;
}


/* Actions */
void Player::respawn()
{
	x = MAP_SPAWN_X;
	y = MAP_SPAWN_Y;
}

void Player::move(int dir, sf::Vector2i m)
{
	if (m.x < x + (width / 2)) scale = -1;
	else scale = 1;

	dx = dir * moveSpeed;
	angle = atan2(ARM_Y + y - m.y, (scale * ARM_X) + x - m.x) * 180.0 / M_PI;
}

void Player::jump()
{
	if (dy == 0) dy = -jumpSpeed;
}

network_bullet Player::shoot()
{
	network_bullet b;
	b.x = x + (scale * ARM_X);
	b.y = y + ARM_Y;
	b.speed = .3;
	b.angle = angle + 180.0;

	Bullet *bul = new Bullet(room, b.x, b.y, b.speed, b.angle);

	b.ptr = bul;
	b._id.id = numBullets;
	b._id.owner = -1;

	numBullets++;

	room.spawn(bul);

	return b;
}

void Player::setAnimation(std::vector<sf::IntRect> const &newAnim)
{
	if (animation != &newAnim)
	{
		frame = 0.0f;
		animation = &newAnim;
	}
}

void Player::updateAnimation(sf::Time deltaTime)
{
	int frames = animation->size();

	// Adjust frame
	if (frames > 1)
	{
		frame += deltaTime.asSeconds() * ANIMATION_SPEED;
		frame = fmodf(frame, (float)frames); // Loop animation if it plays past "frames"
	}
}

void Player::loopAroundMap()
{
	if (x < -(width / 2)) x += ROOM_WIDTH;
	else if (x > ROOM_WIDTH - (width / 2)) x -= ROOM_WIDTH;

	if (y < BAR_HEIGHT - (height / 2)) y += ROOM_HEIGHT - BAR_HEIGHT;
	else if (y > ROOM_HEIGHT - (height / 2)) y -= ROOM_HEIGHT - BAR_HEIGHT;
}

void Player::draw(sf::RenderWindow &window)
{
	for (unsigned int i = 0; i < 9; i++)
	{
		double xx = x, yy = y;

		switch (i)
		{
			case 1:
				xx += VIEW_WIDTH;
				break;
			case 2:
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 3:
				xx += VIEW_WIDTH;
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 4:
				xx -= VIEW_WIDTH;
				break;
			case 5:
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 6:
				xx -= VIEW_WIDTH;
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 7:
				xx += VIEW_WIDTH;
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 8:
				xx -= VIEW_WIDTH;
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
		}

		#ifdef DEBUG_MODE
		// Debug
		sf::RectangleShape r(sf::Vector2f(width, height));
		r.setPosition(xx, yy);
		window.draw(r);
		#endif

		//Arm
		spr.setTextureRect(sf::IntRect(110, 14, 10, 6));
		spr.setOrigin(0, 3);
		spr.setScale(1, 1);
		spr.setRotation(angle + 180);
		spr.setPosition(xx + (width / 2) + (scale * ARM_X), ceil(yy) + ARM_Y);
		window.draw(spr);

		// Body
		spr.setTextureRect((*animation)[(int)frame]);
		spr.setOrigin(origin);
		spr.setScale(sf::Vector2f(scale, 1));
		spr.setRotation(0);
		spr.setPosition(xx + (width / 2), ceil(yy) + height);
		window.draw(spr);
	}
}

void Player::drawOther(sf::RenderWindow &window, double x_, double y_, double angle_, int frame_, int scale_)
{
	for (unsigned int i = 0; i < 9; i++)
	{
		double xx = x_, yy = y_;

		switch (i)
		{
			case 1:
				xx += VIEW_WIDTH;
				break;
			case 2:
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 3:
				xx += VIEW_WIDTH;
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 4:
				xx -= VIEW_WIDTH;
				break;
			case 5:
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 6:
				xx -= VIEW_WIDTH;
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 7:
				xx += VIEW_WIDTH;
				yy -= VIEW_HEIGHT - BAR_HEIGHT;
				break;
			case 8:
				xx -= VIEW_WIDTH;
				yy += VIEW_HEIGHT - BAR_HEIGHT;
				break;
		}

		//Arm
		spr.setTextureRect(sf::IntRect(110, 14, 10, 6));
		spr.setOrigin(0, 3);
		spr.setScale(1, 1);
		spr.setRotation(angle_ + 180);
		spr.setPosition(xx + (width / 2) + (scale_ * ARM_X), ceil(yy) + ARM_Y);
		window.draw(spr);

		// Body
		spr.setTextureRect(ANIM.WALKING[frame_]);
		spr.setOrigin(origin);
		spr.setScale(sf::Vector2f(scale_, 1));
		spr.setRotation(0);
		spr.setPosition(xx + (width / 2), ceil(yy) + height);
		window.draw(spr);
	}
}

void Player::update(sf::Time deltaTime)
{
	applyGravityOnce(deltaTime);
	applyVelocityOnce(deltaTime, dx, dy);
	loopAroundMap();
	pushOutOfSolids();

	if (dy < 0 && !placeFree(x, y - 1)) dy = 0;

	if (dx != 0) setAnimation(ANIM.WALKING);
	else setAnimation(ANIM.STILL);

	updateAnimation(deltaTime);
}

void Player::onDeath()
{
}