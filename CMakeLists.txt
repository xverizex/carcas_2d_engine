cmake_minimum_required (VERSION 3.22)

include (FindPkgConfig)

project (one_in_taiga VERSION 1.0)

add_executable (one_in_taiga 
	main.cpp
	downloader.cpp
	game.cpp
	globals.cpp
	level-logo.cpp
	opengles.cpp
	shaders.cpp
	shader-simple-2d-opengl.cpp
	sprite.cpp
	link-opengl.cpp
	languages.cpp
	texter.cpp
	)

pkg_check_modules (SDL2 "sdl2")
pkg_check_modules (GL "glesv2")
pkg_check_modules (FREETYPE "freetype2")

include_directories (one_in_taiga PUBLIC
	"glm/glm"
	${SDL2_INCLUDE_DIRS}
	${GL_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	)

target_link_libraries (one_in_taiga PUBLIC
	${SDL2_LIBRARIES}
	${GL_LIBRARIES}
	${FREETYPE_LIBRARIES}
	)

file (COPY assets DESTINATION ${CMAKE_BINARY_DIR})

