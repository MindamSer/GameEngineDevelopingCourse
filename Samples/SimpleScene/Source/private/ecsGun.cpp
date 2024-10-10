#include <ecsGun.h>
#include <ecsPhys.h>

#include <flecs.h>

void RegisterEcsGunSystems(flecs::world& world)
{
	world.system<ShootColldown, ReloadColldown>()
		.each([&](ShootColldown& shootCD, ReloadColldown& reloadCD)
	{
		shootCD.value -= world.delta_time();
		reloadCD.value -= world.delta_time();
	});

	world.system<TimeToLiveStart, TimeToLive>()
		.each([&](TimeToLiveStart& ttls, TimeToLive& ttl)
	{
		if (ttls.value)
		{
			ttl.value -= world.delta_time();
		}
	});

	world.system<Position*, BouncePlane*, TimeToLiveStart>()
		.each([&](flecs::entity e, Position* pos, BouncePlane* plane, TimeToLiveStart& ttls)
	{
		if (!ttls.value && plane && pos)
		{
			constexpr float planeEpsilon = 0.1f;
			if (plane->value.x * pos->value.x + plane->value.y * pos->value.y + plane->value.z * pos->value.z < plane->value.w + planeEpsilon)
			{
				ttls.value = true;
			}
		}
	});
}
