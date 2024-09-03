#include "engine.h"

// TODO use vec2 types for positions and such
// TODO rename camera_zoom to camera_z
// TODO add collisions

// Global engine
static Engine gEngine;

// Lua function
// Description: initializes engine resources and opens window
// Args: (string title, int width, int height, int updates_per_second)
// Returns: true on success, false on failure
static i32 init(lua_State *L);

// Lua function
// Description: quits the application
// Args: Nothing
// Returns: Nothing
static i32 quit(lua_State *L);

// Lua function
// Description: loads a texture from file
// Args: (string texture_path)
// Returns: texture ID (integer) on success, nil on failure
static i32 loadTexture(lua_State *L);

// Lua function
// Description: draws a sprite
// Args: (int texture_id, rect source, rect table)
// Returns: Nothing
// Extra notes: rect is a table containing the fields {x, y, w, h}
static i32 drawSprite(lua_State *L);

// Lua function
// Description: checks if a key is down
// Args: (string key_name)
// Returns: true if the key is down, false otherwise
static i32 isKeyDown(lua_State *L);

// Lua function
// Description: gets the size of a texture
// Args: (int texture_id)
// Returns: texture_width, texture_height
static i32 getTextureSize(lua_State *L);

// Lua function
// Description: loads a sound (wav) from file
// Args: (string sound_path)
// Returns: sound_id (integer) on success, nil on failure
static i32 loadSound(lua_State *L);

// Lua function
// Description: plays a loaded sound
// Args: (int sound_id)
// Returns: Nothing
static i32 playSound(lua_State *L);

// Lua function
// Description: plays a directional sound
// Args: (int sound_id, float angle, float distance)
// Returns: Nothing
// Extra notes: angle is in degrees and 0.0 = North and increase clockwise. distance is in the range [0.0, 1.0] with 1.0 being the furthest distance
static i32 playDirectionalSound(lua_State *L);

// Lua function
// Description: loads a track from file
// Args: (string track_path)
// Returns: track_path (integer) on success, nil on failure
static i32 loadTrack(lua_State *L);

// Lua function
// Description: plays a loaded track
// Args: (int track_id)
// Returns: Nothing
static i32 playTrack(lua_State *L);

// Lua function
// Description: sets normal sound volumes
// Args: (float volume [0.0, 1.0])
// Returns: Nothing
static i32 setSoundsVolume(lua_State *L);

// Lua function
// Description: sets track volumes
// Args: (float volume [0.0, 1.0])
// Returns: Nothing
static i32 setTracksVolume(lua_State *L);

// Lua function
// Description: gets the mouse position
// Args: Nothing
// Returns: x, y coordinates
static i32 getMousePosition(lua_State *L);

// Lua function
// Description: checks if a mouse button is pressed
// Args: (int mouse_button)
// Returns: true if pressed, false if not
static i32 isMouseButtonDown(lua_State *L);

// Lua function
// Description: draws a rectangle
// Args: (color color, rect rect)
// Returns: Nothing
// Extra notes: color is a table containing the fields {r, g, b, a} each in the range [0.0, 1.0], rect is a table containing the fields {x, y, w, h}
static i32 drawRectangle(lua_State *L);

// Lua function
// Description: draws a line
// Args: (color color, point p1, point p2)
// Returns: Nothing
// Extra notes: color is a table containing the fields {r, g, b, a} each in the range [0.0, 1.0], point is a table containing the fields {x, y}
static i32 drawLine(lua_State *L);

// Lua function
// Description: sets a camera to render from
// Args: (point position, float zoom)
// Returns: Nothing
// Extra notes: point is a table containing the fields {x, y}, zoom indicates how much to multiply the size of objects (1.0 = 1x, 0.5 = 0.5x, 2.5 = 2.5x)
static i32 setCamera(lua_State *L);

// Helper functions for transforming points based on camera position and zoom
static f32 cameraTransformX(f32 x);
static f32 cameraTransformY(f32 y);

// Lua function
// Description: starts a real-time timer
// Args: (float seconds)
// Returns: the timer
static i32 startRealTimer(lua_State *L);

// Lua function
// Description: starts a frame-time timer
// Args: (int frames)
// Returns: the timer
static i32 startFrameTimer(lua_State *L);

// Lua function
// Description: checks if a real timer is done
// Args: (timer timer)
// Returns: true if finished, false if not
static i32 isRealTimerFinished(lua_State *L);

// Lua function
// Description: checks if a frame timer is done
// Args: (timer timer)
// Returns: true if finished, false if not
static i32 isFrameTimerFinished(lua_State *L);

// Cleans up engine resources
static void close(void);

bool runScript(const char *script_path)
{
	gEngine.L = luaL_newstate();
	if (!gEngine.L)
	{
		return false;
	}

	luaL_openlibs(gEngine.L);
	lua_register(gEngine.L, "init", init);
	lua_register(gEngine.L, "quit", quit);
	lua_register(gEngine.L, "loadTexture", loadTexture);
	lua_register(gEngine.L, "drawSprite", drawSprite);
	lua_register(gEngine.L, "isKeyDown", isKeyDown);
	lua_register(gEngine.L, "getTextureSize", getTextureSize);
	lua_register(gEngine.L, "loadSound", loadSound);
	lua_register(gEngine.L, "playSound", playSound);
	lua_register(gEngine.L, "playDirectionalSound", playDirectionalSound);
	lua_register(gEngine.L, "loadTrack", loadTrack);
	lua_register(gEngine.L, "playTrack", playTrack);
	lua_register(gEngine.L, "setSoundsVolume", setSoundsVolume);
	lua_register(gEngine.L, "setTracksVolume", setTracksVolume);
	lua_register(gEngine.L, "getMousePosition", getMousePosition);
	lua_register(gEngine.L, "isMouseButtonDown", isMouseButtonDown);
	lua_register(gEngine.L, "drawRectangle", drawRectangle);
	lua_register(gEngine.L, "drawLine", drawLine);
	lua_register(gEngine.L, "setCamera", setCamera);
	lua_register(gEngine.L, "startRealTimer", startRealTimer);
	lua_register(gEngine.L, "startFrameTimer", startFrameTimer);
	lua_register(gEngine.L, "isRealTimerFinished", isRealTimerFinished);
	lua_register(gEngine.L, "isFrameTimerFinished", isFrameTimerFinished);
	
	if (luaL_dofile(gEngine.L, script_path) != LUA_OK)
	{
		lua_close(gEngine.L);
		return false;
	}

	// calling onInit() function
	lua_getglobal(gEngine.L, "onInit");
	lua_pcall(gEngine.L, 0, 0, 0);

	u64 current_ticks = SDL_GetTicks64();
	u64 accumulated_ticks = 0;
	while (gEngine.is_on)
	{
		// Processing events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				gEngine.is_on = false;
				break;
			default:
				break;
			}
		}

		// Updating
		u64 new_ticks = SDL_GetTicks64();
		accumulated_ticks += new_ticks - current_ticks;
		// printf("FPS: %f\n", 1000.f/(new_ticks - current_ticks));
		current_ticks = new_ticks;
		while (accumulated_ticks >= gEngine.milliseconds_per_update)
		{
			// calling update(dt) function
			lua_getglobal(gEngine.L, "update");
			lua_pushnumber(gEngine.L, gEngine.seconds_per_update);
			lua_pcall(gEngine.L, 1, 0, 0);
			
			accumulated_ticks -= gEngine.milliseconds_per_update;
			gEngine.current_frame++;
		}

		// Drawing
		SDL_SetRenderDrawColor(gEngine.renderer, 0, 0, 0, 255);
		SDL_RenderClear(gEngine.renderer);
		// calling draw() function
		lua_getglobal(gEngine.L, "draw");
		lua_pcall(gEngine.L, 0, 0, 0);
		SDL_RenderPresent(gEngine.renderer);
	}

	// Cleanup
	lua_close(gEngine.L);
	close();
	return true;
}

static i32 init(lua_State *L)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		lua_pushboolean(gEngine.L, false);
		return 1;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Quit();
		lua_pushboolean(gEngine.L, false);
		return 1;
	}
	if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3)
	{
		IMG_Quit();
		SDL_Quit();
		lua_pushboolean(gEngine.L, false);
		return 1;
	}
	if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048) < 0)
	{
		IMG_Quit();
		SDL_Quit();
		lua_pushboolean(gEngine.L, false);
		return 1;
	}

	gEngine.width = lua_tointeger(L, 2);
	gEngine.height = lua_tointeger(L, 3);
	gEngine.window = SDL_CreateWindow(lua_tostring(L, 1), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gEngine.width, gEngine.height, 0);
	if (!gEngine.window)
	{
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		lua_pushboolean(gEngine.L, false);
		return 1;
	}
	gEngine.renderer = SDL_CreateRenderer(gEngine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!gEngine.renderer)
	{
		SDL_DestroyWindow(gEngine.window);
		Mix_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		lua_pushboolean(gEngine.L, false);
		return 1;
	}

	gEngine.keyboard_state = SDL_GetKeyboardState(&gEngine.n_keys);

	i32 ups = lua_tointeger(L, 4);
	gEngine.seconds_per_update = 1.f/ups;
	gEngine.milliseconds_per_update = 1000.f/ups;
	gEngine.current_frame = 0;

	gEngine.n_textures = 0;
	gEngine.n_sounds = 0;
	gEngine.n_tracks = 0;

	SDL_SetRenderDrawBlendMode(gEngine.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(gEngine.renderer, 0, 0, 0, 255);

	gEngine.camera_x = 0.f;
	gEngine.camera_y = 0.f;
	gEngine.camera_zoom = 1.f;

	gEngine.is_on = true;
	lua_pushboolean(gEngine.L, true);
	return 1;
}

static i32 quit(lua_State *L)
{
	(void)L;
	gEngine.is_on = false;
	return 0;
}

static i32 loadTexture(lua_State *L)
{
	if (gEngine.n_textures == MAX_TEXTURES)
	{
		lua_pushnil(L);
		return 1;
	}

	SDL_Surface *surface = IMG_Load(lua_tostring(L, 1));
	if (!surface)
	{
		lua_pushnil(L);
		return 1;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(gEngine.renderer, surface);
	if (!texture)
	{
		SDL_FreeSurface(surface);
		lua_pushnil(L);
		return 1;
	}

	gEngine.textures[gEngine.n_textures++] = texture;
	lua_pushinteger(L, gEngine.n_textures - 1);
	return 1;
}

static i32 drawSprite(lua_State *L)
{
	i32 texture_id;
	SDL_Rect source;
	SDL_FRect destination;

	// reading texture_id
	texture_id = lua_tointeger(L, 1);

	// reading source
	lua_getfield(L, 2, "x");
	source.x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "y");
	source.y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "w");
	source.w = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "h");
	source.h = lua_tonumber(L, -1);
	lua_pop(L, 1);

	// reading destination
	lua_getfield(L, 3, "x");
	destination.x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 3, "y");
	destination.y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 3, "w");
	destination.w = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 3, "h");
	destination.h = lua_tonumber(L, -1);
	lua_pop(L, 1);

	destination.x = cameraTransformX(destination.x);
	destination.y = cameraTransformY(destination.y);
	destination.w /= gEngine.camera_zoom;
	destination.h /= gEngine.camera_zoom;

	SDL_RenderCopyF(gEngine.renderer, gEngine.textures[texture_id], &source, &destination);
	return 0;
}

static i32 isKeyDown(lua_State *L)
{
	SDL_Scancode scancode = SDL_GetScancodeFromName(lua_tostring(L, 1));
	if (scancode >= (u32)gEngine.n_keys)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, gEngine.keyboard_state[scancode]);
	return 1;
}

static i32 getTextureSize(lua_State *L)
{
	i32 width, height;
	i32 texture_id = lua_tointeger(L, 1);
	SDL_QueryTexture(gEngine.textures[texture_id], NULL, NULL, &width, &height);
	lua_pushinteger(L, width);
	lua_pushinteger(L, height);
	return 2;
}

static i32 loadSound(lua_State *L)
{
	if (gEngine.n_sounds == MAX_SOUNDS)
	{
		lua_pushnil(L);
		return 1;
	}

	Mix_Chunk *sound = Mix_LoadWAV(lua_tostring(L, 1));
	if (!sound)
	{
		lua_pushnil(L);
		return 1;
	}

	gEngine.sounds[gEngine.n_sounds++] = sound;
	lua_pushinteger(L, gEngine.n_sounds - 1);
	return 1;
}

static i32 playSound(lua_State *L)
{
	Mix_PlayChannel(-1, gEngine.sounds[lua_tointeger(L, 1)], 0);
	return 0;
}

static i32 playDirectionalSound(lua_State *L)
{
	i32 channel = Mix_PlayChannel(-1, gEngine.sounds[lua_tointeger(L, 1)], 0);
	Mix_SetPosition(channel, lua_tonumber(L, 2), 255*lua_tonumber(L, 3));
	return 0;
}

static i32 loadTrack(lua_State *L)
{
	if (gEngine.n_tracks == MAX_TRACKS)
	{
		lua_pushnil(L);
		return 1;
	}

	Mix_Music *track = Mix_LoadMUS(lua_tostring(L, 1));
	if (!track)
	{
		lua_pushnil(L);
		return 1;
	}

	gEngine.tracks[gEngine.n_tracks++] = track;
	lua_pushinteger(L, gEngine.n_tracks - 1);
	return 1;
}

static i32 playTrack(lua_State *L)
{
	Mix_PlayMusic(gEngine.tracks[lua_tointeger(L, 1)], 0);
	return 0;
}

static i32 setSoundsVolume(lua_State *L)
{
	Mix_Volume(-1, lua_tonumber(L, 1)*MIX_MAX_VOLUME);
	return 0;
}

static i32 setTracksVolume(lua_State *L)
{
	Mix_VolumeMusic(lua_tonumber(L, 1)*MIX_MAX_VOLUME);
	return 0;
}

static i32 getMousePosition(lua_State *L)
{
	i32 x, y;
	SDL_GetMouseState(&x, &y);
	lua_pushinteger(L, x);
	lua_pushinteger(L, y);
	return 2;
}

static i32 isMouseButtonDown(lua_State *L)
{
	lua_pushboolean(L, SDL_BUTTON(lua_tointeger(L, 1)) & SDL_GetMouseState(NULL, NULL));
	return 1;
}

static i32 drawRectangle(lua_State *L)
{
	struct {u8 r, g, b, a;} color;
	SDL_FRect rect;

	// reading color
	lua_getfield(L, 1, "r");
	color.r = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "g");
	color.g = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "b");
	color.b = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "a");
	color.a = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);

	// reading rect
	lua_getfield(L, 2, "x");
	rect.x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "y");
	rect.y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "w");
	rect.w = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "h");
	rect.h = lua_tonumber(L, -1);
	lua_pop(L, 1);

	rect.x = cameraTransformX(rect.x);
	rect.y = cameraTransformX(rect.y);
	rect.w /= gEngine.camera_zoom;
	rect.h /= gEngine.camera_zoom;

	SDL_SetRenderDrawColor(gEngine.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRectF(gEngine.renderer, &rect);
	return 0;
}

static i32 drawLine(lua_State *L)
{
	struct {u8 r, g, b, a;} color;
	struct {f32 x, y;} p1, p2;

	// reading color
	lua_getfield(L, 1, "r");
	color.r = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "g");
	color.g = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "b");
	color.b = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "a");
	color.a = 255*lua_tonumber(L, -1);
	lua_pop(L, 1);

	// reading p1
	lua_getfield(L, 2, "x");
	p1.x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 2, "y");
	p1.y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	// reading p2
	lua_getfield(L, 3, "x");
	p2.x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 3, "y");
	p2.y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	SDL_SetRenderDrawColor(gEngine.renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLineF(gEngine.renderer, cameraTransformX(p1.x), cameraTransformY(p1.y), cameraTransformX(p2.x), cameraTransformY(p2.y));
	return 0;
}

static i32 setCamera(lua_State *L)
{
	lua_getfield(L, 1, "x");
	gEngine.camera_x = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, 1, "y");
	gEngine.camera_y = lua_tonumber(L, -1);
	lua_pop(L, 1);
	gEngine.camera_zoom = 1.f/lua_tonumber(L, 2);
	
	return 0;
}

static f32 cameraTransformX(f32 x)
{
	return (x - gEngine.camera_x)/gEngine.camera_zoom + gEngine.width/2.f;
}

static f32 cameraTransformY(f32 y)
{
	return (y - gEngine.camera_y)/gEngine.camera_zoom + gEngine.height/2.f;
}

static i32 startRealTimer(lua_State *L)
{
	f32 seconds = lua_tonumber(L, 1);
	f64 timer = SDL_GetTicks64() + 1000.f*seconds;
	lua_pushnumber(L, timer);
	return 1;
}

static i32 startFrameTimer(lua_State *L)
{
	i32 frames = lua_tointeger(L, 1);
	f64 timer = gEngine.current_frame + frames;
	lua_pushnumber(L, timer);
	return 1;
}

static i32 isRealTimerFinished(lua_State *L)
{
	f64 timer = lua_tonumber(L, 1);
	lua_pushboolean(L, SDL_GetTicks64() >= timer);
	return 1;
}

static i32 isFrameTimerFinished(lua_State *L)
{
	f64 timer = lua_tonumber(L, 1);
	lua_pushboolean(L, gEngine.current_frame >= timer);
	return 1;
}

static void close(void)
{
	for (i32 i = 0; i < gEngine.n_sounds; i++)
	{
		Mix_FreeChunk(gEngine.sounds[i]);
	}
	for (i32 i = 0; i < gEngine.n_tracks; i++)
	{
		Mix_FreeMusic(gEngine.tracks[i]);
	}
	for (i32 i = 0; i < gEngine.n_textures; i++)
	{
		SDL_DestroyTexture(gEngine.textures[i]);
	}
	SDL_DestroyRenderer(gEngine.renderer);
	SDL_DestroyWindow(gEngine.window);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
