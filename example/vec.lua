local Vec = {}
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

function Vec:scale(s)
	return Vec.new(self.x*s, self.y*s)
end

function Vec:magnitude(v)
	return (self.x*self.x + self.y*self.y)^0.5
end

function Vec:normalize(v)
	return self:scale(1/self:magnitude())
end

return Vec
