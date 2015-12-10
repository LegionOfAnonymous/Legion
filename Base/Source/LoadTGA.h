#ifndef LOAD_TGA_H
#define LOAD_TGA_H
#include <GL\glew.h>

GLuint LoadTGA(const char *file_path, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR_MIPMAP_LINEAR, GLint wrap = GL_REPEAT, bool generateMipmap = true, bool anisotropy = true);

#endif