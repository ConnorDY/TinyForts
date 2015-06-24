#include <cassert>
#include <algorithm>
#include <cmath>
#include "room.h"
#include "globals.h"
#include "object.h"

Room::Room(StateManager &stm, SoundManager &som, TextureManager const &tm, settings_t &settings)
	: State(stm),
	  width(VIEW_WIDTH), height(VIEW_HEIGHT), multiplier(1),
	  settings(settings), soundManager(som), textureManager(tm),
	  view_follow(nullptr)
{
	setView(sf::View(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT)));
}

Room::~Room()
{
	end();
}

static bool depthSort(Object const* i, Object const* j)
{
	return i->getDepth() > j->getDepth();
}

bool Room::exceedsHorizontalBounds(sf::FloatRect const &rect) const
{
	return rect.left < 0 || rect.left + rect.width >= width;
}

settings_t const& Room::getSettings() const { return settings; }

std::vector<Object*> const Room::getObjects() const
{
	return objects;
}

double Room::getMultiplier() const
{
	return multiplier;
}

void Room::spawn(Object *obj)
{
	spawnQueue.push_back(obj);
}

void Room::deleteObj(object_id id_d)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		Object* obj = objects.at(i);
		object_id id_o = obj->getId();

		if (id_d.owner == id_o.owner && id_d.id == id_o.id)
		{
			printf("Found object to delete (%d:%d). Deleting...\n", id_d.owner, id_d.id);
			obj->destroyedByServer = true;
			obj->kill();
			return;
		}
	}
}

void Room::setMultiplier(double m)
{
	multiplier = m;
}

void Room::start()
{
}

void Room::end()
{
	view_follow = nullptr;
	for (Object* i : objects) delete i;
	objects.clear();
	for (Object* i : spawnQueue) delete i;
	spawnQueue.clear();
}

void Room::reset()
{
	end();
	start();
}

void Room::updateView(sf::RenderWindow &window)
{
	if (view_follow)
	{
		// Update view
		auto rect = view_follow->Object::getRect();
		double vx = rect.left + rect.width/2;
		double vy = rect.top + rect.height/2;

		auto view_size = getView().getSize();
		double vw = view_size.x;
		double vh = view_size.y;

		auto viewLeftMax = vw / 2;
		auto viewTopMax = vh / 2;
		auto viewRightMax = width - viewLeftMax;
		auto viewBottomMax = height - viewTopMax;

		// Don't let the view go past the bottom or right of the room
		if (vx > viewRightMax) vx = viewRightMax;
		if (vy > viewBottomMax) vy = viewBottomMax;

		// Don't let the view go past the top or left of the room
		if (vx < vw / 2.0f) vx = vw / 2.0f;
		if (vy < vh / 2.0f) vy = vh / 2.0f;

		getView().setCenter(vx, vh / 2.0f);
		window.setView(getView());
	}
}

bool Room::shouldDraw(Object const *obj) const
{
	if (obj != nullptr) return true;
	else return false;
}

void Room::drawSprites(sf::RenderWindow &window)
{
	for (auto object : objects)
		if (shouldDraw(object))
			object->draw(window);
}

void Room::draw(sf::RenderWindow &window)
{
	updateView(window);
	std::sort(objects.begin(), objects.end(), depthSort);
	drawBackground(window);
	drawSprites(window);
	drawForeground(window);
}

void Room::update(sf::RenderWindow&, SoundManager&, InputHandler&)
{
	deltaTime = sf::milliseconds(restartClock().asMilliseconds() * multiplier);

	#ifdef DEBUG_MODE
	printf("FPS estimate: %4.3f\n", 1 / deltaTime.asSeconds());
	#endif

	// If the framerate is too low, slow down the game anyway (instead of having shit jumping everywhere)
	auto maxTime = sf::seconds(0.1f);
	if (deltaTime > maxTime) deltaTime = maxTime;

	std::copy(spawnQueue.begin(), spawnQueue.end(), std::back_inserter(objects));
	spawnQueue.clear();

	// Update objects and player
	auto iter = objects.begin();
	auto end = objects.end();

	while (iter != end)
	{
		Object *obj = *iter;

		if (!obj->shouldDelete()) obj->update(deltaTime);

		// Redo deletion check, because update() can cause it to be deleted
		if (obj->shouldDelete())
		{
			// Send deletion to server
			if (!obj->destroyedByServer)
			{
				object_id id_d = obj->getId();

				StateManager &sM = getStateManager();
				Server *server = sM.getServer();
				Client *client = sM.getClient();

				if (server != nullptr) server->sendDelete(id_d);
				if (client != nullptr) client->sendDelete(id_d);
			}

			delete obj;
			iter = objects.erase(iter);
			end = objects.end();
			continue;
		}

		iter++;
	}
}
