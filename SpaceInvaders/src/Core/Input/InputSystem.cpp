#include "pch.h"
#include "InputSystem.h"
#include <bitset>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::BeginFrame()
{
	for (auto keycode : m_keyDown)
	{
		m_key.insert(keycode);
	}

	m_keyDown.clear();
	m_keyUp.clear();
}

bool InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if(!m_key.contains(event.key.keysym.sym)) m_keyDown.insert(event.key.keysym.sym);
		return true;
	case SDL_KEYUP:
		m_key.erase(event.key.keysym.sym);
		m_keyUp.insert(event.key.keysym.sym);
		return true;
	}

	return false;
}
