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



local function collisions(it)
	local dist2 = 0
    for pos, coll, ent in ecs.each(it) do
		coll.value = true
		
		--local q = ecs.query("Position, Collider, AppContext($)")
        --local other_it = ecs.query_iter(q)
		--while ecs.query_next(other_it) do
		--	for other_pos, other_coll, other_ent in ecs.each(other_it) do
		--		if not coll.value and ent.name ~= other_ent.name then
		--			dist2 =	
		--				(pos.x - other_pos.x) * (pos.x - other_pos.x) +
		--				(pos.y - other_pos.y) * (pos.y - other_pos.y) +
		--				(pos.z - other_pos.z) * (pos.z - other_pos.z)
		--			if dist2 < 1.0 then
		--				coll.value = true
		--				
		--				pos.y = 15.
		--			end
		--		end
		--	end
		--end
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

ecs.system(collisions, "CollisionsSystem", ecs.OnUpdate, "Position, Collider")
ecs.system(jumpOnCollision, "JumpOnCollisionSystem", ecs.OnUpdate, "Velocity, Collider, JumpOnCollision")
ecs.system(wooshOnCollision, "WooshOnCollisionSystem", ecs.OnUpdate, "Velocity, Collider, WooshOnCollision")
