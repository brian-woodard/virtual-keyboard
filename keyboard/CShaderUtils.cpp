/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */

#include "CShaderUtils.h"

CShader::CShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
   // 1. retrieve the vertex/fragment source code from filePath
   std::string vertexCode;
   std::string fragmentCode;
   std::string geometryCode;
   std::ifstream vShaderFile;
   std::ifstream fShaderFile;
   std::ifstream gShaderFile;
   // ensure ifstream objects can throw exceptions:
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try
   {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      // if geometry shader path is present, also load a geometry shader
      if (geometryPath != nullptr)
      {
         gShaderFile.open(geometryPath);
         std::stringstream gShaderStream;
         gShaderStream << gShaderFile.rdbuf();
         gShaderFile.close();
         geometryCode = gShaderStream.str();
      }
   }
   catch (std::ifstream::failure &e)
   {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
   }
   const char *vShaderCode = vertexCode.c_str();
   const char *fShaderCode = fragmentCode.c_str();
   // 2. compile shaders
   unsigned int vertex, fragment;
   // vertex shader
   GLCALL(vertex = glCreateShader(GL_VERTEX_SHADER));
   GLCALL(glShaderSource(vertex, 1, &vShaderCode, NULL));
   GLCALL(glCompileShader(vertex));
   checkCompileErrors(vertex, "VERTEX");
   // fragment Shader
   GLCALL(fragment = glCreateShader(GL_FRAGMENT_SHADER));
   GLCALL(glShaderSource(fragment, 1, &fShaderCode, NULL));
   GLCALL(glCompileShader(fragment));
   checkCompileErrors(fragment, "FRAGMENT");
   // if geometry shader is given, compile geometry shader
   unsigned int geometry;
   if (geometryPath != nullptr)
   {
      const char *gShaderCode = geometryCode.c_str();
      GLCALL(geometry = glCreateShader(GL_GEOMETRY_SHADER));
      GLCALL(glShaderSource(geometry, 1, &gShaderCode, NULL));
      GLCALL(glCompileShader(geometry));
      checkCompileErrors(geometry, "GEOMETRY");
   }
   // shader Program
   GLCALL(mID = glCreateProgram());
   GLCALL(glAttachShader(mID, vertex));
   GLCALL(glAttachShader(mID, fragment));
   if (geometryPath != nullptr)
      GLCALL(glAttachShader(mID, geometry));
   GLCALL(glLinkProgram(mID));
   checkCompileErrors(mID, "PROGRAM");
   // delete the shaders as they're linked into our program now and no longer necessary

   GLCALL(glDeleteShader(vertex));
   GLCALL(glDeleteShader(fragment));
   if (geometryPath != nullptr)
      GLCALL(glDeleteShader(geometry));

   GLCALL(glDetachShader(mID, vertex))
   GLCALL(glDetachShader(mID, fragment));
   if (geometryPath != nullptr)
      GLCALL(glDetachShader(mID, geometry));
}

CShader::~CShader()
{
   GLCALL(glDeleteProgram(mID));
}

GLint CShader::GetAttrib(GLuint program, const char *name)
{
   GLint attribute;
   GLCALL(attribute = glGetAttribLocation(program, name));
   if (attribute == -1)
      fprintf(stderr, "Could not bind attribute %s\n", name);
   return attribute;
}

GLint CShader::GetUniform(GLuint program, const char *name)
{
   GLint uniform;
   GLCALL(uniform = glGetUniformLocation(program, name));
   if (uniform == -1)
      fprintf(stderr, "Could not bind uniform %s\n", name);
   return uniform;
}
// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void CShader::checkCompileErrors(GLuint shader, std::string type)
{
   GLint success;
   GLchar infoLog[1024];
   if (type != "PROGRAM")
   {
      GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
      if (!success)
      {
         GLCALL(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
         std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                   << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
   }
   else
   {
      GLCALL(glGetProgramiv(shader, GL_LINK_STATUS, &success));
      if (!success)
      {
         GLCALL(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
         std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                   << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
   }
}
