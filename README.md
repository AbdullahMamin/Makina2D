# Makina2D
Game engine, which I use for my own projects, written in C with scripting in lua.

## Build
This project has the following dependencies: SDL2, SDL2_image, SDL2_mixer, and the lua5.4 library.
Simply compile all the sources and link against these libraries. The makefile provided works on linux and may need slight changes to work on other platforms.

## Documentation
The different functions, and how to use them are all located in engine.c and are all marked as "lua function"

## Usage
To use the engine, go into the directory of your game project, and run the engine executable with the argument being the main.lua file where the onInit, update, and draw functions are defined.
An example game is provided to see how to work with the engine.
To try it out, simple go to the example directory and run the command "engine main.lua", where engine is the executable.

assets from: https://kenney.nl/assets/pixel-shmup
