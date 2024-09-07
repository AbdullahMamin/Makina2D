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

function Vec:magnitude()
	return (self.x*self.x + self.y*self.y)^0.5
end

function Vec:scale(s)
	return Vec.new(s*self.x, s*self.y)
end

function Vec:dot(v)
	return self.x*v.x + self.y*v.y
end

function Vec:normalize()
	local inv_magnitude = self:magnitude()
	return Vec.new(inv_magnitude*self.x, inv_magnitude*self.y)
end
