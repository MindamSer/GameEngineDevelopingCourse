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

ecs.system(timeToLive, "TimeToLiveSystem", ecs.OnUpdate, "TimeToLive")
ecs.system(deleteOnDeath, "DeletingSystem", ecs.OnUpdate, "Position, TimeToLive")
ecs.system(dieOnGround, "DieOnGroundSystem", ecs.OnUpdate, "Position, BouncePlane, TimeToLive, DieOnGround")
