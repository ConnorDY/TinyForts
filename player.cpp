#include "player.h"
#include "room.h"
#include "bullet.h"
#include "globals.h"
#include "network_player.h"
#include "network_bullet.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ARM_X 1
#define ARM_Y 12

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
	moveSpeed(0.17), jumpSpeed(0.51), numBullets(0), clientId(0)
{
	spr.setTexture(room.textureManager.getRef("player"));
	//spr.setColor(sf::Color(255, 0, 0));
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

unsigned int Player::getClientId() const
{
	return clientId;
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
	b._id.id = numBullets;
	b._id.owner = clientId;

	Bullet *bul = new Bullet(room, b.x, b.y, b.speed, b.angle);
	bul->setId(b._id);

	numBullets++;

	room.spawn(bul);

	return b;
}

void Player::setClientId(unsigned int i)
{
	clientId = i;
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
	if (x < -(width / 2)) x += room.getWidth();
	else if (x > room.getWidth() - (width / 2)) x -= room.getWidth();

	if (y < -(height / 2)) y += room.getHeight();
	else if (y > room.getHeight() - (height / 2)) y -= room.getHeight();
}

void Player::draw(sf::RenderWindow &window)
{
	#ifdef DEBUG_MODE
	// Debug
	sf::RectangleShape r(sf::Vector2f(width, height));
	r.setPosition(x, y);
	window.draw(r);
	#endif

	// Skin
	int skin = 67;
	int col = floor((double)skin / 31);

	//Arm
	sf::IntRect frameRect = sf::IntRect(110, 14, 10, 6);
	frameRect.top += 32 * (skin % 31);
	frameRect.left += 128 * col;

	spr.setTextureRect(frameRect);
	spr.setOrigin(1, 3);
	spr.setScale(1, 1);
	spr.setRotation(angle + 180);
	spr.setPosition(x + (width / 2) + (scale * ARM_X), ceil(y) + ARM_Y);
	window.draw(spr);

	// Body
	frameRect = (*animation)[(int)frame];
	frameRect.top += 32 * (skin % 31);
	frameRect.left += 128 * col;

	spr.setTextureRect(frameRect);
	spr.setOrigin(origin);
	spr.setScale(sf::Vector2f(scale, 1));
	spr.setRotation(0);
	spr.setPosition(x + (width / 2), ceil(y) + height);
	window.draw(spr);
}

void Player::drawOther(sf::RenderWindow &window, double x_, double y_, double angle_, int frame_, int scale_)
{
	//Arm
	spr.setTextureRect(sf::IntRect(110, 14, 10, 6));
	spr.setOrigin(0, 3);
	spr.setScale(1, 1);
	spr.setRotation(angle_ + 180);
	spr.setPosition(x_ + (width / 2) + (scale_ * ARM_X), ceil(y_) + ARM_Y);
	window.draw(spr);

	// Body
	spr.setTextureRect(ANIM.WALKING[frame_]);
	spr.setOrigin(origin);
	spr.setScale(sf::Vector2f(scale_, 1));
	spr.setRotation(0);
	spr.setPosition(x_ + (width / 2), ceil(y_) + height);
	window.draw(spr);
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