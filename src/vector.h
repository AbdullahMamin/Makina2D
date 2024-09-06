#ifndef VECTOR_H
#define VECTOR_H

#include <tgmath.h>
#include "types.h"

typedef struct
{
	f32 x, y;
} vec2;

vec2 vec2Add(vec2 v1, vec2 v2);
vec2 vec2Subtract(vec2 v1, vec2 v2);
vec2 vec2Multiply(vec2 v1, vec2 v2);
vec2 vec2Divide(vec2 v1, vec2 v2);
f32 vec2Dot(vec2 v1, vec2 v2);
f32 vec2Cross(vec2 v1, vec2 v2); // returns z-component
vec2 vec2Scale(f32 s, vec2 v);
f32 vec2Magnitude(vec2 v);
vec2 vec2Normalize(vec2 v);

#endif // VECTOR_H
