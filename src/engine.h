#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
#include "vector.h"

#define MAX_TEXTURES (64)
#define MAX_SOUNDS (64)
#define MAX_TRACKS (64)

typedef struct Engine
{
	// Lua stuff
	lua_State *L;
		
	// Window stuff
	i32 width, height;
	bool is_on;
	SDL_Window *window;
	SDL_Renderer *renderer;

	// Input stuff
	const u8 *keyboard_state;
	i32 n_keys;

	// Timing stuff
	f32 seconds_per_update;
	f32 milliseconds_per_update;
	u64 current_frame;

	// Texture manager
	SDL_Texture *textures[MAX_TEXTURES];
	i32 n_textures;

	// Audio manager
	Mix_Chunk *sounds[MAX_SOUNDS];
	i32 n_sounds;
	Mix_Music *tracks[MAX_TRACKS];
	i32 n_tracks;

	// Camera
	f32 camera_x, camera_y;
	f32 camera_zoom;
} Engine;

// Runs the lua script which is responsible for calling init(title, width, height, ups) and defining the following functions:
// onInit() -> called once after the engine was initialized, used to initialize game objects and/or load resources as needed
// draw() -> called at the end of each game loop to draw the game state
// update(dt) -> called as needed (based on a set constant update rate) to update game entities, dt is time passed since the last update which is constant
// Returns: true on success, false on failure
bool runScript(const char *script_path);

#endif // ENGINE_H
