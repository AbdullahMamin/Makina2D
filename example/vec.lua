Vec = {}
Vec.__index = Vec

function Vec.new(x, y)
	local instance = setmetatable({}, Vec)
	instance.x = x
	instance.y = y
	return instance
end

function Vec:add(v)
	return Vec.new(self.x + v.x, self.y + v.y)
end

function Vec:subtract(v)
	return Vec.new(self.x - v.x, self.y - v.y)
end

function Vec:multiply(v)
	return Vec.new(self.x*v.x, self.y*v.y)
end

function Vec:divide(v)
	return Vec.new(self.x/v.x, self.y/v.y)
end

function Vec:magnitude(v)
	return (v.x*v.x + v.y*v.y)^0.5
end

function Vec:scale(s)
	return Vec.new(s*v.x, s*v.y)
end

function Vec:dot(v)
	return self.x*v.x + self.y*v.y
end
