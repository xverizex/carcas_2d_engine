#pragma once

enum TYPE_SHADER {
	SHADER_MAIN,
	TYPE_SHADERS_N
};

void init_shaders ();

#define SHADER_CONVERT_TO_VERT(name) name##_VERT
#define SHADER_CONVERT_TO_FRAG(name) name##_FRAG

#ifdef __ANDROID__
#define SHADER_MAIN_VERT              "main.vert"
#define SHADER_MAIN_FRAG              "main.frag"
#else
#define SHADER_MAIN_VERT              "assets/main.vert"
#define SHADER_MAIN_FRAG              "assets/main.frag"
#endif
