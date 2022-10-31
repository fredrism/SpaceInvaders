#pragma once
#include "SDL.h"
#include <string>

inline unsigned int GetKeyCode(std::string str)
{
	if (str.size() == 1)
	{
		if ('a' <= str[0] && str[0] <= 'z')
		{
			return SDLK_a + (str[0] - 'a');
		}

		if ('0' <= str[0] && str[0] <= '9')
		{
			return SDLK_0 + (str[0] - '0');
		}
	}

	if (str == "lctrl") return SDLK_LCTRL;
	if (str == "rctrl") return SDLK_RCTRL;
	if (str == "lshift") return SDLK_LSHIFT;
	if (str == "rshift") return SDLK_RSHIFT;
	if (str == "space") return SDLK_SPACE;
	if (str == "left") return SDLK_LEFT;
	if (str == "right") return SDLK_RIGHT;
	if (str == "up") return SDLK_UP;
	if (str == "down") return SDLK_DOWN;

	return 0;
}