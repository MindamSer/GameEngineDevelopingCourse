local ecs = require "ecs"

local function timeToLive(it)
    for ttl, ent in ecs.each(it) do
		if ttl.start then
			ttl.value = ttl.value - it.delta_time
		end
    end
end

local function deleteOnDeath(it)
    for pos, ttl, ent in ecs.each(it) do
        if ttl.value < 0.0 then
			pos.x = 1e6
			pos.y = 1e6
		end
    end
end

local function dieOnGround(it)
    for pos, plane, ttl, dog, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			ttl.start = true
		end
    end
end



local function oneCollision(it)
	x, y, z, ent = it.param[1], it.param[2], it.param[3], it.param[4]
	
	for other_pos, other_coll, other_ent in ecs.each(it) do
		if ent ~= other_ent then
			local dist2 =	
				(x - other_pos.x) * (x - other_pos.x) +
				(y - other_pos.y) * (y - other_pos.y) +
				(z - other_pos.z) * (z - other_pos.z)
			if dist2 < 1.0 then
				it.interrupted_by = 1
			end
		end
    end
end
oneCollisionId = ecs.system(oneCollision, "OneCollisionSystem", ecs.Empty, "Position, Collider")

local function collisions(it)
	for pos, coll, ent in ecs.each(it) do
		coll.value = ecs.run(oneCollisionId, 0.0, {pos.x, pos.y, pos.z, ent})
    end
end

local function jumpOnCollision(it)
    for vel, coll, ws, ent in ecs.each(it) do
        if coll.value then
			vel.y = 10.
		end
    end
end

local function wooshOnCollision(it)
    for vel, coll, ws, ent in ecs.each(it) do
        if coll.value then
			vel.x = 10.
		end
    end
end



ecs.system(timeToLive, "TimeToLiveSystem", ecs.OnUpdate, "TimeToLive")
ecs.system(deleteOnDeath, "DeletingSystem", ecs.OnUpdate, "Position, TimeToLive")
ecs.system(dieOnGround, "DieOnGroundSystem", ecs.OnUpdate, "Position, BouncePlane, TimeToLive, DieOnGround")

ecs.system(collisions, "CollisionSystem", ecs.OnUpdate, "Position, Collider")
ecs.system(jumpOnCollision, "JumpOnCollisionSystem", ecs.OnUpdate, "Velocity, Collider, JumpOnCollision")
ecs.system(wooshOnCollision, "WooshOnCollisionSystem", ecs.OnUpdate, "Velocity, Collider, WooshOnCollision")