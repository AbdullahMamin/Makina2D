require "vec"
require "tilemap"

function onInit()
	tilemap = loadTilemap("res/tilemap.png", 12, 11, 16, 16)
	tiles = {
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		width = 8,
		height = 8
	}
	for i = 1, 8 do
		for j = 1, 8 do
			tiles[i][j] = math.random(12*11)
		end
	end

	timer = startFrameTimer(120)

	zoom = 1.0

end

function update(dt)
	zoom = zoom + dt
end

function draw()
	setCamera(Vec.new(0.0, 0.0), zoom)
	drawTiles(tilemap, tiles, Rect(-400, -400, 800, 800))
	-- if isFrameTimerFinished(timer) then drawLine({r=1.0,g=1.0,b=1.0,a=0.3},Vec.new(0.0, 0.0),Vec.new(800.0, 700)) end
	-- drawRectangle({r=1.0,g=1.0,b=1.0,a=0.3},{x=0.0,y=0.0,w=200,h=200})
end

init("Game", 800, 800, 60)
