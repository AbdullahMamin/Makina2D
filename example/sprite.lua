Rect = require "rect"

local Sprite = {}
Sprite.__index = Sprite

function Sprite.new(texture_id, source)
	local instance = setmetatable({}, Sprite)
	instance.texture_id = texture_id
	instance.source = source
	return instance
end

function Sprite:draw(destination)
	drawSprite(self.texture_id, self.source, destination)
end

return Sprite
