#pragma once
#include "SDL.h"

class TickTimer
{
public:
	TickTimer()
	{
		m_prevTick = SDL_GetTicks64();
	}

	Uint64 GetTicks()
	{
		Uint64 tick = SDL_GetTicks64();

		Uint64 elapsed = tick - m_prevTick;
		m_prevTick = tick;
		
		return elapsed;
	}

private:
	Uint64 m_prevTick;
};