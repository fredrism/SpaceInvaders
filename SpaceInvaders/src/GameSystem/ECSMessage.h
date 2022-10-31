#pragma once
#include "ECSCommon.h"
#include <string>

enum ECSMessageType
{
	Created, Destroyed, Collision, String, Callback
};

struct ECSMessage
{
	ECSMessageType type;
	ComponentFlags mask;
	EntityID entity;
	EntityID other;
	std::string message;
	std::function<void()> callback;							
	void* data;									//Because message handling is delayed until after updating each system, the message data must be valid until the end of the frame
};