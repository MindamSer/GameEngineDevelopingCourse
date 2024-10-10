#pragma once

#include <flecs.h>

struct MagazineCapacity
{
	int value;
};

struct Magazine
{
	int value;
};

struct ShootColldown
{
	float value;
};

struct ReloadColldown
{
	float value;
};

struct TimeToLiveStart
{
	bool value;
};

struct TimeToLive
{
	float value;
};

struct Bullet 
{
	bool value;
};

struct Target
{
	bool value;
};

void RegisterEcsGunSystems(flecs::world& world);
