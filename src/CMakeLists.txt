cmake_minimum_required (VERSION 3.22)

include (FindPkgConfig)

project (one_in_taiga VERSION 1.0)

set (EXECUTABLE_OUTPUT_PATH ..)

add_executable (one_in_taiga 
	main.cpp
	core/downloader.cpp
	core/game.cpp
	core/globals.cpp
	levels/level-logo.cpp
	core/opengles.cpp
	core/shaders.cpp
	shaders/shader-simple-2d-opengl.cpp
	objects/sprite.cpp
	core/link-opengl.cpp
	core/languages.cpp
	core/texter.cpp
        core/sound.cpp core/sound.h core/sound_linux.cpp core/sound_linux.h)

pkg_check_modules (SDL2 "sdl2")
pkg_check_modules (GL "glesv2")
pkg_check_modules (FREETYPE "freetype2")

include_directories (one_in_taiga PUBLIC
	"third_party/glm"
	"."
	${SDL2_INCLUDE_DIRS}
	${GL_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	)

target_link_libraries (one_in_taiga PUBLIC
	${SDL2_LIBRARIES}
	${GL_LIBRARIES}
	${FREETYPE_LIBRARIES}
	)
