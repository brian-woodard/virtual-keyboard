
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include "CShaderUtils.h"
#include FT_FREETYPE_H

using namespace std;
using namespace glm;

const int MAX_GLYPHS = 128;

class CText
{
private:
   
   struct Point
   {
      GLfloat x;
      GLfloat y;
      GLfloat s;
      GLfloat t;
   };

   // Holds all state information relevant to a character as loaded using FreeType
   struct Character
   {
      unsigned int TextureID; // ID handle of the glyph texture
      ivec2 Size;             // Size of glyph
      ivec2 Bearing;          // Offset from baseline to left/top of glyph
      unsigned int Advance;   // Horizontal offset to advance to next glyph
   };

   static CShader *mShader;

   Character* GetOrCreateFont();

   //Character Characters[MAX_GLYPHS];
   Character* mFont;
   static std::unordered_map<std::string, Character*> mFonts; // holds array of textures for each unique font file

   GLint mAttributeCoord;
   GLint mUniformTex;
   GLint mUniformColor;
   Point mPoint;
   GLuint mVbo;
   GLuint mVao;
   float mWindowWidth;
   float mWindowHeight;

   FT_Library mFt;
   FT_Face mFace;
   string mFontFileName;
   float sx = 0.0f;
   float sy = 0.0f;
   unsigned int mFontHeight = 48;
   unsigned int mFontWidth = 48;
   mat4 mProjection;
   bool mInvertY;
   glm::vec3 mTextColor;

public:
   CText(mat4 projection, unsigned int size);
   CText(mat4 projection, unsigned int size, const char *Font);
   ~CText();

   Character* InitializeFont();
   bool IsInitialized() const { return (mFont != nullptr); }

   void Test();

   unsigned int GetWidth(void)
   {
      return mFontWidth;
   }

   unsigned int GetHeight(void)
   {
      return mFontHeight;
   }

   void SetWindowSize(float w, float h)
   {
      mWindowWidth = w;
      mWindowHeight = h;
   }

   void SetFontFileName(string &font)
   {
      mFontFileName = font;
   }

   void SetFontSize(unsigned int size)
   {
      mFontHeight = size;
   }

   unsigned int GetFontSize() { return mFontHeight; }

   void SetColor(const vec3 &color)
   {
      mTextColor = color;
   }

   void SetColor(const vec4 &color)
   {
      mTextColor.r = color.r;
      mTextColor.g = color.g;
      mTextColor.b = color.b;
   }

   void SetMVP(const glm::mat4& mvp)
   {
      mProjection = mvp;
   }

   void SetInvertY(bool Invert) { mInvertY = Invert; }

   void Print(string &text, float x, float y, float scale, vec3 color);

   float Print(const char *Text, float X, float Y, float Scale = 1.0f);

   float GetWidth(const char *Text, float Scale = 1.0f);
};
