#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <ecsGun.h>
#include <ecsMesh.h>
#include <RenderObject.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>

using namespace GameEngine;

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			Geometry::Ptr Cube();
			Geometry::Ptr Ocrahedron();
		}
	}
}

void RegisterEcsControlSystems(flecs::world& world)
{
	static flecs::query<const Position, TimeToLive, Target> q = world.query<const Position, TimeToLive, Target>();
	static flecs::query<Magazine> mags = world.query<Magazine>();

	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
	{
		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
		}
		position.value = position.value + currentMoveDir.Normalized() * speed * world.delta_time();
		camera.ptr->SetPosition(position.value);
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z < plane.value.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.value.y = jump.value;
			}
		}
	});

	world.system<const CameraPtr, const ControllerPtr, const MagazineCapacity, Magazine, ShootColldown, ReloadColldown>()
		.each([&](const CameraPtr& camera, const ControllerPtr& controller, const MagazineCapacity& magazineCap, Magazine& magazine, ShootColldown& shootCD, ReloadColldown& reloadCD)
	{
		if (reloadCD.value < 0.f)
		{
			if (controller.ptr->IsPressed("Shoot") && shootCD.value < 0.f)
			{
				shootCD.value = 0.5f;
				--magazine.value;
				flecs::entity bullet = world.entity()
					.set(Position{ camera.ptr->GetPosition() + camera.ptr->GetViewDir() * 1.f })
					.set(Velocity{ camera.ptr->GetViewDir() * 20.f })
					.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
					.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
					.set(Bounciness{ 0.5f })
					.set(GeometryPtr{ RenderCore::DefaultGeometry::Ocrahedron() })
					.set(RenderObjectPtr{ new Render::RenderObject() })
					.set(TimeToLive{ 3.0f })
					.set(TimeToLiveStart{ false })
					.set(Bullet());
			}

			if (magazine.value <= 0)
			{
				reloadCD.value = 3.0f;
				magazine.value = magazineCap.value;
			}
		}
	});

	world.system<const Position, TimeToLive, Bullet>()
		.each([&](const Position& bul_pos, TimeToLive& bul_ttl, Bullet)
			{
				q.each([&](const Position& tar_pos, TimeToLive& tar_ttl, Target) 
					{
						if (bul_ttl.value > 0.f &&
							(bul_pos.value.x - tar_pos.value.x) * (bul_pos.value.x - tar_pos.value.x) +
							(bul_pos.value.y - tar_pos.value.y) * (bul_pos.value.y - tar_pos.value.y) +
							(bul_pos.value.z - tar_pos.value.z) * (bul_pos.value.z - tar_pos.value.z) < 1.f)
						{
							bul_ttl.value = -1.f;
							tar_ttl.value = -1.f;

							mags.each([&](Magazine& mag)
								{
									mag.value += 3;
								});
						}
					});
			});
}

