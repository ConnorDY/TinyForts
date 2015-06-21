#ifndef ROOM_H
#define ROOM_H

class Object;
#include "state.h"

class Room : public State
{
	private:
		bool shouldDraw(Object const* object) const;

	protected:
		std::vector<Object*> objects;               // Objects present in this room
		Object *view_follow;                        // The object to follow around with the camera
		sf::Time deltaTime;                         // The time delta between the most recent update() and the previous
		int const width, height;                    // The width and height of the room
		std::vector<Object*> spawnQueue;            // Queue of objects to be spawned next tick
		settings_t &settings;
		double multiplier;

	public:
		SoundManager &soundManager;                 // Sound manager for use by objects in this room
		TextureManager const &textureManager;

		explicit Room(StateManager &stm, SoundManager &som, TextureManager const &tm, settings_t &settings);
		virtual ~Room();

		bool exceedsHorizontalBounds(sf::FloatRect const &rect) const;

		settings_t const& getSettings() const;
		std::vector<Object*> const getObjects() const;
		double getMultiplier() const;

		void spawn(Object *obj);
		void setMultiplier(double m);

		virtual void start();
		virtual void end();
		void reset();

		virtual void updateView(sf::RenderWindow &window);
		virtual void drawBackground(sf::RenderWindow &window);
		virtual void drawSprites(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif