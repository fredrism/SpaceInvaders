#pragma once
#include "SDL.h"
#include <iostream>

#pragma warning (disable:4305)
#define iCHECK_SDL_ERROR(x) if(x < 0) { std::cout << SDL_GetError() << std::endl; assert(false); }
#define pCHECK_SDL_ERROR(x) if(x == NULL) { std::cout << SDL_GetError() << std::endl; assert(false); }