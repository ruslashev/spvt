#ifndef GLUTILS_HPP
#define GLUTILS_HPP

#include <GL/glew.h>

#include "../errors.hpp"

GLuint CreateShader(GLenum type, const char *src);
GLuint CreateShaderProgram(GLuint vs, GLuint fs);
GLint BindUniform(GLuint shaderProgramP, const char *name);
GLint BindAttribute(GLuint shaderProgramP, const char *name);
void PrintLog(GLuint shaderOrProg);

#endif

