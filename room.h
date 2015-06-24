#ifndef ROOM_H
#define ROOM_H

class Object;
class Server;
class Client;
#include "state.h"
#include "object_id.h"

class Room : public State
{
	private:
		bool shouldDraw(Object const* object) const;

	protected:
		std::vector<Object*> objects;               // Objects present in this room
		Object *view_follow;                        // The object to follow around with the camera
		sf::Time deltaTime;                         // The time delta between the most recent update() and the previous
		int width, height;                    		// The width and height of the room
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

		int getWidth() const;
		int getHeight() const;

		void spawn(Object *obj);
		void deleteObj(object_id id_d);
		void setMultiplier(double m);

		virtual void start();
		virtual void end();
		void reset();

		virtual void updateView(sf::RenderWindow &window);
		virtual void drawBackground(sf::RenderWindow &window) = 0;
		virtual void drawSprites(sf::RenderWindow &window);
		virtual void drawForeground(sf::RenderWindow &window) = 0;
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif