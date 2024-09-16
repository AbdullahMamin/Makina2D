Player = require "player"
Tilemap = require "tilemap"
Sprite = require "sprite"
Vec = require "vec"

Game = {
	width = 256,
	height = 256,
	scale = 3
}

function onInit()
	TM = Tilemap.loadFromFile("res/tilemap", "res/tiles_packed.png")
	PLANES = loadTexture("res/ships_packed.png")

	Entities = {
		player = Player.new(Vec.new(0, 0), 64, Sprite.new(PLANES, Rect.new(0, 0, 32, 32)), Vec.new(32, 32), 1/5, 128, 176, Sprite.new(PLANES, Rect.new(32, 32, 32, 32)), Vec.new(16, 16)),
	}

	setCamera(Vec.new(0, 0), Game.scale)
end

function update(dt)
	Entities.player:update(dt)
end

function draw()
	TM:draw(Rect.new(-Game.width/2, -Game.height/2, Game.width, Game.height))
	Entities.player:draw()
end

init("Space shooter example", Game.width*Game.scale, Game.height*Game.scale, 60)
