#pragma once

#include <flecs.h>

struct TimeToLive
{
	bool start;
	float value;
};

struct DieOnGround 
{
	bool value;
};

struct Collider
{
	bool value;
};

struct JumpOnCollision
{
	bool value;
};

struct WooshOnCollision
{
	bool value;
};