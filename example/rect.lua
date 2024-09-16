local Rect = {}
Rect.__index = Rect

function Rect.new(x, y, w, h)
	local instance = setmetatable({}, Rect)
	instance.x = x
	instance.y = y
	instance.w = w
	instance.h = h
	return instance
end

return Rect
