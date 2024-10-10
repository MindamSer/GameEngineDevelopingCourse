#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsGun.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsMeshSystems(flecs::world& world)
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<const GeometryPtr, RenderObjectPtr>()
		.each([&](flecs::entity e, const GeometryPtr& geometry, RenderObjectPtr& renderObject)
	{
		renderThread->ptr->EnqueueCommand(Render::ERC::CreateRenderObject, geometry.ptr, renderObject.ptr);
		e.remove<GeometryPtr>();
	});

	world.system<RenderObjectPtr, const Position>()
		.each([&](RenderObjectPtr& renderObject, const Position& position)
	{
		renderObject.ptr->SetPosition(position.value, renderThread->ptr->GetMainFrame());
	});

	world.system<RenderObjectPtr, TimeToLive>()
		.each([&](flecs::entity e, RenderObjectPtr& renderObject, TimeToLive& ttl)
	{
		if (ttl.value <= 0.f)
		{
			renderThread->ptr->EnqueueCommand(Render::ERC::DeleteRenderObject, GeometryPtr().ptr, renderObject.ptr);
			e.destruct();
		}
	});
}


