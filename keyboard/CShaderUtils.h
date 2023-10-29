/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#ifndef _SHADER_UTILS_H
#define _SHADER_UTILS_H

#include <GL/glew.h>
// #include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// NOTE: Uncomment the following line for GL error handling
//#define GL_DEBUG

#ifdef GL_DEBUG
#define GLCALL(function) \
   { \
      GLenum error = GL_INVALID_ENUM; \
      while (error != GL_NO_ERROR) \
      { \
         error = glGetError(); \
      } \
      function; \
      error = glGetError(); \
      if (error != GL_NO_ERROR) \
      { \
         fprintf(stderr, "OpenGL Error: GL_ENUM(%d) at %s:%d\n", error, __FILE__, __LINE__); \
      } \
   }
#else
#define GLCALL(function) function;
#endif

class CShader
{
   void checkCompileErrors(GLuint shader, std::string type);

public:
   unsigned int mID;

   CShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
   virtual ~CShader();

   char *FileRead(const char *filename);
   void PrintLog(GLuint object);
   GLuint CreateShader(const char *filename, GLenum type);
   GLuint CreateProgram(const char *vertexfile, const char *fragmentfile);
   GLuint CreateGsProgram(const char *vertexfile, const char *geometryfile,
                          const char *fragmentfile, GLint input, GLint output,
                          GLint vertices);
   GLint GetAttrib(GLuint program, const char *name);
   GLint GetUniform(GLuint program, const char *name);
   // activate the shader
   // ------------------------------------------------------------------------
   void use()
   {
      GLCALL(glUseProgram(mID));
   }
   // utility uniform functions
   // ------------------------------------------------------------------------
   void setBool(const std::string &name, bool value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform1i(location, (int)value));
   }
   // ------------------------------------------------------------------------
   void setInt(const std::string &name, int value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform1i(location, value));
   }
   // ------------------------------------------------------------------------
   void setFloat(const std::string &name, float value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform1f(location, value));
   }
   // ------------------------------------------------------------------------
   void setVec2(const std::string &name, const glm::vec2 &value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform2fv(location, 1, &value[0]));
   }
   void setVec2(const std::string &name, float x, float y) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform2f(location, x, y));
   }
   // ------------------------------------------------------------------------
   void setVec3(const std::string &name, const glm::vec3 &value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform3fv(location, 1, &value[0]));
   }
   void setVec3(const std::string &name, float x, float y, float z) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform3f(location, x, y, z));
   }
   // ------------------------------------------------------------------------
   void setVec4(const std::string &name, const glm::vec4 &value) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform4fv(location, 1, &value[0]));
   }
   void setVec4(const std::string &name, float x, float y, float z, float w)
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniform4f(location, x, y, z, w));
   }
   // ------------------------------------------------------------------------
   void setMat2(const std::string &name, const glm::mat2 &mat) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]));
   }
   // ------------------------------------------------------------------------
   void setMat3(const std::string &name, const glm::mat3 &mat) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]));
   }
   // ------------------------------------------------------------------------
   void setMat4(const std::string &name, const glm::mat4 &mat) const
   {
      GLint location;
      GLCALL(location = glGetUniformLocation(mID, name.c_str()));
      GLCALL(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
   }
};
#endif
