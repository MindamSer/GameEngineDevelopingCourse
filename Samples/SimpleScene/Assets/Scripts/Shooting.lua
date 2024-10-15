local ecs = require "ecs"

local function LivingSystem(it)
    for ttl, ent in ecs.each(it) do
		if ttl.start then
			ttl.value = ttl.value - it.delta_time
		end
    end
end

local function ShootCooldownSystem(it)
    for shootCD, ent in ecs.each(it) do
        shootCD.value = shootCD.value - it.delta_time
    end
end

local function ReloadCooldownSystem(it)
    for reloadCD, ent in ecs.each(it) do
        reloadCD.value = reloadCD.value - it.delta_time
    end
end

local function DeletingSystem(it)
    for pos, ttl, ent in ecs.each(it) do
        if ttl.value < 0.0 then
			pos.x = 1e6
		end
    end
end

local function DieOnGroundSystem(it)
    for pos, plane, ttl, dog, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			ttl.start = true 
		end
    end
end

ecs.system(LivingSystem, "LivingSystem", ecs.OnUpdate, "TimeToLive")
ecs.system(ShootCooldownSystem, "ShootCooldownSystem", ecs.OnUpdate, "ShootCooldown")
ecs.system(ReloadCooldownSystem, "ReloadCooldownSystem", ecs.OnUpdate, "ReloadCooldown")
ecs.system(DeletingSystem, "DeletingSystem", ecs.OnUpdate, "Position, TimeToLive")
ecs.system(DieOnGroundSystem, "DieOnGroundSystem", ecs.OnUpdate, "Position, BouncePlane, TimeToLive, DieOnGround")
