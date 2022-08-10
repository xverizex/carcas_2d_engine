#include <shaders/shader-simple-2d-opengl_int.h>
#include <GLES3/gl3.h>
#include <objects/sprite_int.h>

ShaderSimple2DOpenGLInt::ShaderSimple2DOpenGLInt ()
{
}

void ShaderSimple2DOpenGLInt::init (uint32_t program)
{
	this->program = program;
	glUseProgram (program);

	this->uniform_ortho = glGetUniformLocation (program, "ortho");
	this->uniform_position = glGetUniformLocation (program, "pos");
	this->uniform_cur_texture = glGetUniformLocation (program, "s_texture");
}

#include <stdio.h>

void ShaderSimple2DOpenGLInt::render (void *data)
{
	SpriteInt *sp = static_cast<SpriteInt *>(data);
	
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
