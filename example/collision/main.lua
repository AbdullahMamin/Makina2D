require "rect"
require "vec"

function onInit()
	static_box = Rect(0, 0, 100, 100)
	dynamic_box = Rect(-200, -200, 50, 50)
end

function update(dt)
	local speed = 100
	local displacement = Vec.new(0, 0)
	if isKeyDown("W") then
		displacement.y = displacement.y - 1.0
	end
	if isKeyDown("A") then
		displacement.x = displacement.x - 1.0
	end
	if isKeyDown("S") then
		displacement.y = displacement.y + 1.0
	end
	if isKeyDown("D") then
		displacement.x = displacement.x + 1.0
	end
	if displacement.x ~= 0 or displacement.y ~= 0 then
		displacement = displacement:normalize():scale(speed*dt)
		
		local collided, t, nx, ny = isRectVsRect(dynamic_box, displacement, static_box)
		local normal = Vec.new(nx, ny)

		if collided and t < 1.0 then
			print(t)
			local reverse_normal = Vec.new(normal.y, normal.x)
			local dot = displacement:dot(reverse_normal)*(1.0 - t) 
			displacement = reverse_normal:scale(dot)
		end

		dynamic_box.x = dynamic_box.x + displacement.x
		dynamic_box.y = dynamic_box.y + displacement.y
	end
end

function draw()
	drawRectangle({r=1.0,g=1.0,b=1.0,a=1.0}, static_box)
	drawRectangle({r=0.0,g=1.0,b=1.0,a=1.0}, dynamic_box)
end

init("Game", 800, 800, 60)
