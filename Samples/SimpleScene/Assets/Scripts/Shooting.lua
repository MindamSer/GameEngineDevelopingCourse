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
			pos.y = 1e3
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

local function CollisionSystem(it)
	local pos_arr, coll_arr = ecs.columns(it)
	local dist2 = 0
	for i = 1, it.count do
		for j = i + 1, it.count do
			dist2 =	
				(pos_arr[i].x - pos_arr[j].x) * (pos_arr[i].x - pos_arr[j].x) +
				(pos_arr[i].y - pos_arr[j].y) * (pos_arr[i].y - pos_arr[j].y) +
				(pos_arr[i].z - pos_arr[j].z) * (pos_arr[i].z - pos_arr[j].z)
			if dist2 < 1. then
				coll_arr[i].value = true
				coll_arr[j].value = true
			else
				coll_arr[i].value = false
				coll_arr[j].value = false
			end
		end
	end
end

local function JumpOnCollisionSystem(it)
    for vel, coll, joc, ent in ecs.each(it) do
		if coll.value then
			vel.y = 0.
		end
    end
end

ecs.system(LivingSystem, "LivingSystem", ecs.OnUpdate, "TimeToLive")
ecs.system(ShootCooldownSystem, "ShootCooldownSystem", ecs.OnUpdate, "ShootCooldown")
ecs.system(ReloadCooldownSystem, "ReloadCooldownSystem", ecs.OnUpdate, "ReloadCooldown")
ecs.system(DeletingSystem, "DeletingSystem", ecs.OnUpdate, "Position, TimeToLive")
ecs.system(DieOnGroundSystem, "DieOnGroundSystem", ecs.OnUpdate, "Position, BouncePlane, TimeToLive, DieOnGround")
ecs.system(CollisionSystem, "CollisionSystem", ecs.OnUpdate, "Position, Collider")
ecs.system(JumpOnCollisionSystem, "JumpOnCollisionSystem", ecs.OnUpdate, "Velocity, Collider, JumpOnCollision")
