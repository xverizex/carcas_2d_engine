# carcas engine for linux and android 2d.
There is support features
* display sprite
* display text

# How to add resource file?
Add to directory `assets` your file. Open the file `to_download.h` and fill by pattern.
```
#pragma once

#define TO_STRING_FILENAME(name) name##_STRING


enum TO_DOWNLOADS {
        LINK_LOGO,
        LINKS_N
};

#ifdef __ANDROID__
#define LINK_LOGO_STRING                  "logo.res"
#else
#define LINK_LOGO_STRING                  "assets/logo.res"
#endif
```
then open the file `donwloader.cpp` and find the function `donwloader_load` and fill by pattern.
```
Link *downloader_load (const enum TO_DOWNLOADS file)
{
        switch (file) {
                case LINK_LOGO:
                        if (link[LINK_LOGO] == nullptr)
                                link[LINK_LOGO] = load_link (TO_STRING_FILENAME (LINK_LOGO));
                        break;
        }

        return link[file];
}
```
Now you are can to load this resource once and use it with some objects on scene.
```
        logo = new Sprite ();
        logo->shader = global_shader[SHADER_MAIN];
        logo->link = downloader_load (LINK_LOGO);
        logo->set_pos (0.0f, 0.0f, 0.0f);
        logo->set_width (1.0f);
```
# How to add own shader?
Implement `class Shader`.
```
class Shader {
        public:
                virtual void init (uint32_t type) = 0;
                virtual void render (void *data) = 0;
};
```
This is example how to implement shader for simple 2d sprite.
```
#include "shader-simple-2d-opengl.h"
#include <GLES3/gl3.h>
#include "sprite.h"

ShaderSimple2DOpenGL::ShaderSimple2DOpenGL ()
{
}

void ShaderSimple2DOpenGL::init (uint32_t program)
{
        this->program = program;
        glUseProgram (program);

        this->uniform_ortho = glGetUniformLocation (program, "ortho");
        this->uniform_position = glGetUniformLocation (program, "pos");
        this->uniform_cur_texture = glGetUniformLocation (program, "s_texture");
}

void ShaderSimple2DOpenGL::render (void *data)
{
        Sprite *sp = static_cast<Sprite *>(data);

        glUseProgram (this->program);

        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_2D, sp->link->tex[sp->cur_tex]);

        glUniform1i (this->uniform_cur_texture, 0);
        glUniformMatrix4fv (this->uniform_ortho, 1, GL_FALSE, &sp->ortho[0][0]);
        glUniformMatrix4fv (this->uniform_position, 1, GL_FALSE, &sp->pos[0][0]);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (VERTEX_COORD, 3, GL_FLOAT, GL_FALSE, 0, sp->link->v);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, 0, sp->link->t);

        glDrawArrays (GL_TRIANGLES, 0, 6);
}
```
The Shader should to create once and uses in all program. Here is the implementation **render** of `Sprite`.
```
void Sprite::render ()
{
        play_animate ();

        shader->render (this);
}
```
The shader knows how to use fields of sprite. Then is just add this shader to sprite class.
```
        logo->shader = global_shader[SHADER_MAIN];
```
# How to add animation to sprite?
Just simple to point your preferences in methods of sprite class.
```
void set_animate_range (int index, int start, int end)
void play_once (int offset, int what)
void set_speed (int offset, int speeds)
```
# How to add multilanguage?
Just to create files in `assets/lang`. For example there is two files `end.txt` and `rus.txt`. Each line in file is index in global array text. Open a file `translates.h` and fill by pattern.
```
#pragma once

enum LANGUAGE {
        LANG_ENG,
        LANG_RUS,
        LANG_N
};

#define LANG_TO_STRING_RES(name) name##_RES

#ifdef __ANDROID__
#define LANG_ENG_RES                          "lang/eng.txt"
#define LANG_RUS_RES                          "lang/rus.txt"
#else
#define LANG_ENG_RES                          "assets/lang/eng.txt"
#define LANG_RUS_RES                          "assets/lang/rus.txt"
#endif

inline char **init_list_lang ()
{
        char **file = new char *[LANG_N];

        file[LANG_ENG] = LANG_TO_STRING_RES (LANG_ENG);
        file[LANG_RUS] = LANG_TO_STRING_RES (LANG_RUS);

        return file;
}
```
Open the file `text.h` and fill by pattern.
```
#pragma once 

enum TEXT {
        TEXT_HELLO_WORLD,
        TEXT_N
};
```
Then you can use this.
```
extern Language global_language;
...
void LevelLogo::load ()
{
        global_language.set_language (LANG_RUS);
	...
        text->link = texter->generate_link (global_language[TEXT_HELLO_WORLD], 18);
	...
}
```
# How to add new level?
Implement `ILevel class`.
```
class ILevel {
        public:
                virtual void clear_screen (Graphics *graphics) = 0;
                virtual void load () = 0;
                virtual void handle_button (enum EVENT_TYPE type, int x, int y) = 0;
                virtual void render () = 0;
};
```
In `Game::loop` add a new level and point `global_cur_level` to start.
```
void Game::loop ()
{

        level = new ILevel *[LEVEL_N];
        level[LEVEL_LOGO] = new LevelLogo ();

        global_cur_level = LEVEL_LOGO;

        level[global_cur_level]->load ();
```
