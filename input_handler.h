#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class InputHandler
{
	public:
		enum Input
		{
			Left, Right, Up, Down,
			Start, Exit,
			PressL, ReleaseL, PressR, ReleaseR
		};

		InputHandler();

		// Actions
		bool checkInput(Input i, sf::Event e);
		bool checkInput(Input i);
	private:
		enum InputType
		{
			Key,
			MouseInput,
			JoystickInput
		};
		struct InputKeys
		{
			InputType inputType;
			sf::Event::EventType eventType;
			sf::Keyboard::Key keyCode;
			sf::Mouse::Button mouseButton;
		};

		InputKeys *Inputs[10];
};

#endif