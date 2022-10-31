#include "pch.h"
#include <SDL.h>
#include "Core/Core.h"
#include "Core/Engine.h"
#include "VMath/Matrix4f.h"
#include "Core/Random/Random.h"
#include "Core/Threads/ThreadPool.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	Engine* e = new Engine("Space Invaders from Outer Space");
	e->Run();
	delete e;

	SDL_Quit();

	return 0;
}