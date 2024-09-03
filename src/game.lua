Player = {x = 0, y = 0}
Player.__index = Player

function Player.new(x, y)
	instance = setmetatable({}, Player)
	instance.x = x or Player.x
	instance.y = y or Player.y
	return instance
end

function Player:update(dt)
	local speed = 500
	local vel = {x = 0, y = 0}
	if isKeyDown("W") then
		vel.y = vel.y - 1
	end
	if isKeyDown("S") then
		vel.y = vel.y + 1
	end
	if isKeyDown("A") then
		vel.x = vel.x - 1
	end
	if isKeyDown("D") then
		vel.x = vel.x + 1
	end
	if vel.x ~= 0 or vel.y ~= 0 then
		length = (vel.x*vel.x + vel.y*vel.y)^0.5
		vel.x = vel.x/length
		vel.y = vel.y/length
		
		vel.x = speed*vel.x
		vel.y = speed*vel.y

		self.x = self.x + vel.x*dt
		self.y = self.y + vel.y*dt
	end
end

function Player:draw()
	drawSprite(Textures[1], {x = 0, y = 0, w = 100, h = 100}, {x = self.x, y = self.y, w = 400, h = 400})
end

function onInit()
	setSoundsVolume(0.2)
	setTracksVolume(0.2)
	Textures = {
		loadTexture("assets/test.jpeg")
	}
	Tracks = {
		loadTrack("assets/test.mp3")
	}
	Sounds = {
		loadSound("assets/test.wav")
	}
	Players = {}
	for i = 1, 10000 do
		Players[#Players + 1] = Player.new(i, i)
	end
	playTrack(Tracks[1])
	playSound(Sounds[1])
end

function update(dt)
	if isKeyDown("escape") then
		quit()
	end
	for i = 1, #Players do
		Players[i]:update(dt)
	end
end

function draw()
	for i = 1, #Players do
		Players[i]:draw()
	end
end

init("Hello", 1600, 900, 60)
