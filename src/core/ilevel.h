#pragma once

#include "graphics.h"
#include "events.h"
#include <GLES3/gl3.h>
#include "downloader.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "shader.h"
#include "shaders.h"

extern Shader **global_shader;

class ILevel {
        public:
                virtual void clear_screen (Graphics *graphics) = 0;
                virtual void load () = 0;
				virtual void load_links () = 0;
		virtual void unload () = 0;
                virtual void handle_button (enum EVENT_TYPE type, int x, int y) = 0;
                virtual void render () = 0;

		int from, to;
};
