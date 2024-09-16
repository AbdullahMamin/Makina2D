Sprite = require "sprite"
Rect = require "rect"

local Tilemap = {}
Tilemap.__index = Tilemap

function Tilemap.loadFromFile(file_path, image_path)
	local instance = setmetatable({}, Tilemap)
	local tm = require(file_path)
	instance.texture_id = loadTexture(image_path)
	instance.tileset_width = tm.tileset_width
	instance.tileset_height = tm.tileset_height
	instance.width = tm.width
	instance.height = tm.height
	instance.tile_width = tm.tile_width
	instance.tile_height = tm.tile_height
	instance.n_layers = tm.n_layers
	instance.layers = tm.layers
	return instance
end

function Tilemap:draw(destination)
	local function tileToOffset(tile)
		return Rect.new((tile - 1)%self.tileset_width*self.tile_width, (tile - 1)//self.tileset_width*self.tile_height, self.tile_width, self.tile_height)
	end
	local tile_width = destination.w/self.width
	local tile_height = destination.h/self.height
	for l = 1, self.n_layers do
		for i = 1, self.height do
			if self.layers[l][i][j] ~= 0 then
				for j = 1, self.width do
					local spt = Sprite.new(self.texture_id, tileToOffset(self.layers[l][i][j]))
					spt:draw(Rect.new(destination.x + (j - 1)*tile_width, destination.y + (i - 1)*tile_height, tile_width, tile_height))
				end
			end
		end
	end
end

return Tilemap
