#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <ecsShooting.h>
#include <ecsMesh.h>
#include <RenderObject.h>

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

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
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
		position.x = position.x + currentMoveDir.Normalized().x * speed.value * world.delta_time();
		position.y = position.y + currentMoveDir.Normalized().y * speed.value * world.delta_time();
		position.z = position.z + currentMoveDir.Normalized().z * speed.value * world.delta_time();
		camera.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z));
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.y = jump.value;
			}
		}
	});

	world.system<const CameraPtr, const ControllerPtr, Magazine, ShootCooldown, ReloadCooldown>()
		.each([&](const CameraPtr& camera, const ControllerPtr& controller, Magazine& mag, ShootCooldown& shootCD, ReloadCooldown& relCD)
			{
				if (relCD.value <= 0.f)
				{
					if (controller.ptr->IsPressed("Shoot") && shootCD.value <= 0.f)
					{
						shootCD.value = shootCD.max_value;
						--mag.value;

						Math::Vector3f spawn_pos = camera.ptr->GetPosition() + camera.ptr->GetViewDir() * 1.f;
						Math::Vector3f spawn_vel = camera.ptr->GetViewDir() * 20.f;

						flecs::entity bullet = world.entity()
							.set(Position{ spawn_pos.x, spawn_pos.y, spawn_pos.z })
							.set(Velocity{ spawn_vel.x, spawn_vel.y, spawn_vel.z })
							.set(Gravity{ 0.f, -9.8065f, 0.f })
							.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
							.set(Bounciness{ 0.5f })
							.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Ocrahedron() })
							.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
							.set(TimeToLive{ false, 2.f })
							.set(DieOnGround())
							.set(Collider());
					}

					if (mag.value <= 0)
					{
						relCD.value = relCD.max_value;
						mag.value = mag.max_value;
					}
				}
			});
}

