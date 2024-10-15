#pragma once

#include <flecs.h>

// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN // Ensure meta symbols are only defined once
#endif

ECS_STRUCT(TimeToLive,
{
	bool start;
	float value;
});

ECS_STRUCT(DieOnGround, { bool value; });

ECS_STRUCT(Magazine,
{
	int max_value;
	int value;
});

ECS_STRUCT(ShootCooldown,
{
	float max_value;
	float value;
});

ECS_STRUCT(ReloadCooldown,
{
	float max_value;
	float value;
});

ECS_STRUCT(Collider, { bool value; });
ECS_STRUCT(JumpOnCollision, { bool value; });
