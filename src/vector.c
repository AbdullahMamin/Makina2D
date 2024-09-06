#include "vector.h"

vec2 vec2Add(vec2 v1, vec2 v2)
{
	return (vec2){v1.x + v2.x, v1.y + v2.y};
}

vec2 vec2Subtract(vec2 v1, vec2 v2)
{
	return (vec2){v1.x - v2.x, v1.y - v2.y};
}

vec2 vec2Multiply(vec2 v1, vec2 v2)
{
	return (vec2){v1.x*v2.x, v1.y*v2.y};
}

vec2 vec2Divide(vec2 v1, vec2 v2)
{
	return (vec2){v1.x/v2.x, v1.y/v2.y};
}

f32 vec2Dot(vec2 v1, vec2 v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

f32 vec2Cross(vec2 v1, vec2 v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

vec2 vec2Scale(f32 s, vec2 v)
{
	return (vec2){s*v.x, s*v.y};
}

f32 vec2Magnitude(vec2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

vec2 vec2Normalize(vec2 v)
{
	f32 inv_magnitude = 1.f/sqrt(v.x*v.x + v.y*v.y);
	return (vec2){inv_magnitude*v.x, inv_magnitude*v.y};
}
