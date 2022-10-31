#pragma once
#include "../System.h"
#include "KeyCodes.h"
#include <set>

class InputSystem : public System
{
public:
	InputSystem();
	~InputSystem();

	virtual bool ProcessEvent(SDL_Event& event) override;
	virtual void BeginFrame() override;

	bool GetKey(SDL_Keycode code) const { return m_key.contains(code); }
	bool GetKeyDown(SDL_Keycode code) const { return m_keyDown.contains(code); }
	bool GetKeyUp(SDL_Keycode code) const { return m_keyUp.contains(code); }

private:
	std::set<SDL_Keycode> m_keyDown;
	std::set<SDL_Keycode> m_key;
	std::set<SDL_Keycode> m_keyUp;
};

