cmake_minimum_required (VERSION 3.22)

include (FindPkgConfig)

project (fishing_boy VERSION 1.0)

set (EXECUTABLE_OUTPUT_PATH ..)

add_executable (fishing_boy 
	main.cpp
	core/downloader.cpp
	core/game.cpp
	core/globals.cpp
	levels/level-logo.cpp
	levels/level-swamp.cpp
	core/opengles.cpp
	core/shaders.cpp
	shaders/shader-simple-2d-opengl.cpp
	shaders/shader-simple-2d-opengl_int.cpp
	objects/sprite.cpp
	objects/sprite_int.cpp
	core/link-opengl.cpp
	core/languages.cpp
	core/texter.cpp
        core/sound.cpp core/sound.h core/sound_linux.cpp core/sound_linux.h)

pkg_check_modules (SDL2 "sdl2")
pkg_check_modules (GL "glesv2")
pkg_check_modules (FREETYPE "freetype2")
pkg_check_modules (OPENAL "openal")

include_directories (fishing_boy PUBLIC
	"third_party/glm"
	"."
	${SDL2_INCLUDE_DIRS}
	${GL_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	${OPENAL_INCLUDE_DIRS}
	)

target_link_libraries (fishing_boy PUBLIC
	${SDL2_LIBRARIES}
	${GL_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${OPENAL_LIBRARIES}
	)
