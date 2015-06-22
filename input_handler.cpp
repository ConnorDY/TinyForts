#include "input_handler.h"

InputHandler::InputHandler()
{
	// Left
	InputKeys *key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::A;
	Inputs[Input::Left] = key;

	// Right
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::D;
	Inputs[Input::Right] = key;

	// Down
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::S;
	Inputs[Input::Down] = key;

	// Up
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::W;
	Inputs[Input::Up] = key;

	// Start
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Return;
	Inputs[Input::Start] = key;

	// Exit
	key = new InputKeys();
	key->inputType = Key;
	key->eventType = sf::Event::KeyPressed;
	key->keyCode = sf::Keyboard::Escape;
	Inputs[Input::Exit] = key;

	// Left Pressed
	key = new InputKeys();
	key->inputType = MouseInput;
	key->eventType = sf::Event::MouseButtonPressed;
	key->mouseButton = sf::Mouse::Left;
	Inputs[Input::PressL] = key;

	// Left Released
	key = new InputKeys();
	key->inputType = MouseInput;
	key->eventType = sf::Event::MouseButtonReleased;
	key->mouseButton = sf::Mouse::Left;
	Inputs[Input::ReleaseL] = key;

	// Right Pressed
	key = new InputKeys();
	key->inputType = MouseInput;
	key->eventType = sf::Event::MouseButtonPressed;
	key->mouseButton = sf::Mouse::Right;
	Inputs[Input::PressR] = key;

	// Right Pressed
	key = new InputKeys();
	key->inputType = MouseInput;
	key->eventType = sf::Event::MouseButtonReleased;
	key->mouseButton = sf::Mouse::Right;
	Inputs[Input::ReleaseR] = key;
}


/* Actions */
bool InputHandler::checkInput(Input i, sf::Event e)
{
	// Mouse event
	if (Inputs[i]->inputType == MouseInput &&
		Inputs[i]->eventType == e.type &&
		Inputs[i]->mouseButton == e.mouseButton.button) return true;
	// KeyPress event
	else if (Inputs[i]->inputType == Key &&
		Inputs[i]->eventType == e.type &&
		Inputs[i]->keyCode == e.key.code) return true;
	else return false;
}

bool InputHandler::checkInput(Input i)
{
	// KeyHeld event
	if (Inputs[i]->inputType == Key &&
		sf::Keyboard::isKeyPressed(Inputs[i]->keyCode)) return true;
	else return false;
}