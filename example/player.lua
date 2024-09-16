Vec = require "vec"

local Player = {}
Player.__index = Player

function Player.new(position, speed, sprite, size, bullet_cooldown, min_bullet_speed, max_bullet_speed, bullet_sprite, bullet_size)
	local instance = setmetatable({}, Player)
	instance.position = position
	instance.speed = speed
	instance.sprite = sprite
	instance.size = size
	instance.bullet_cooldown = bullet_cooldown
	instance.bullet_timer = 0.0
	instance.min_bullet_speed = min_bullet_speed
	instance.max_bullet_speed = max_bullet_speed
	instance.n_bullets = 0
	instance.bullets = {}
	instance.bullet_sprite = bullet_sprite
	instance.bullet_size = bullet_size
	return instance
end

function Player:update(dt)
	local vel = Vec.new(0, 0)
	if isKeyDown("W") then
		vel.y = vel.y - 1
	end
	if isKeyDown("A") then
		vel.x = vel.x - 1
	end
	if isKeyDown("S") then
		vel.y = vel.y + 1
	end
	if isKeyDown("D") then
		vel.x = vel.x + 1
	end
	if vel.x ~= 0 or vel.y ~= 0 then
		vel = vel:normalize():scale(self.speed*dt)
		self.position = self.position:add(vel)
		if self.position.x < -Game.width/2 then
			self.position.x = -Game.width/2
		elseif self.position.x > Game.width/2 - self.size.x then
			self.position.x = Game.width/2 - self.size.x
		end
		if self.position.y < -Game.height/2 then
			self.position.y = -Game.height/2
		elseif self.position.y > Game.height/2 - self.size.y then
			self.position.y = Game.height/2 - self.size.y
		end
	end
	
	if isKeyDown("space") then
		if isRealTimerFinished(self.bullet_timer) then
			self.bullets[self.n_bullets + 1] = {
				position = self.position:add(Vec.new(self.size.x/2, 0)),
				velocity = Vec.new(0, -(math.random()*(self.max_bullet_speed - self.min_bullet_speed) + self.min_bullet_speed)),
				alive = true
			}
			self.n_bullets = self.n_bullets + 1
			self.bullet_timer = startRealTimer(self.bullet_cooldown)
		end
	end

	for i = 1, self.n_bullets do
		self.bullets[i].position = self.bullets[i].position:add(self.bullets[i].velocity:scale(dt))
	end
end

function Player:draw()
	self.sprite:draw(Rect.new(self.position.x, self.position.y, self.size.x, self.size.y))
	for i = 1, self.n_bullets do
		self.bullet_sprite:draw(Rect.new(self.bullets[i].position.x, self.bullets[i].position.y, self.bullet_size.x, self.bullet_size.y))
	end
end

return Player
