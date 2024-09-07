require "rect"

function loadTilemap(file_path, width, height, tile_width, tile_height)
	return {
		texture = loadTexture(file_path),
		width = width,
		height = height,
		tile_width = tile_width,
		tile_height = tile_height
	}
end

function drawTiles(tilemap, tiles, destination)
	local function tileToOffset(tile)
		local row = (tile - 1)//tilemap.width
		local column = (tile - 1)%tilemap.height
		return Rect(column*tilemap.tile_width, row*tilemap.tile_height, tilemap.tile_width, tilemap.tile_height)
	end
	local w = destination.w/tiles.width
	local h = destination.h/tiles.height
	for i = 1, tiles.height do
		for j = 1, tiles.width do
			drawSprite(
				tilemap.texture,
				tileToOffset(tiles[i][j]),
				Rect(destination.x + (j - 1)*w, destination.y + (i - 1)*h, w, h)
			)
		end
	end
end
