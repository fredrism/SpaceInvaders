#pragma once
#include "SDL.h"

class System
{
public:
	virtual ~System() {};
	virtual void BeginFrame() {};
	virtual bool ProcessEvent(SDL_Event& event) { return false; };
	virtual void EndFrame() {};
};