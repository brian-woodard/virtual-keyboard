
#pragma once

#include <iostream>
#include <vector>
#include <math.h>

// #include <GLFW/glfw3.h>
#include "CShaderUtils.h"

using glm::lookAt;
using glm::mat4;
using glm::radians;
using glm::vec3;
using std::string;
using std::vector;
typedef enum
{
   SOLID,
   DASH,
   TRIANGLE,
   TRIANGLE_FAN,
   RECTANGLE
} LineMode;

class CLine
{
   unsigned int VBO, VAO;
   vector<glm::vec3> mVertices;
   vec3 mStartPoint;
   vec3 mEndPoint;
   vec3 mWindowSize;
   glm::vec4 lineColor;
   static CShader *mShader;
   float x1;
   float y1;
   float x2;
   float y2;
   float w;
   float h;
   float mLineWidth;
   GLushort pattern;
   GLfloat dashsize;
   GLfloat gapsize;
   GLint resolution;
   bool mCompat;
   LineMode mLineMode;
   mat4 MVP;
   mat4 mModel;
   mat4 mView;
   mat4 mClipPlane;

protected:
   void SetVertices(void);
   void SetVertice(void);

public:
   void DrawQuad(float X, float Y, float Width, float Height);
   void DrawQuad(const vec3 &P1, const vec3 &P2, const vec3 &P3, const vec3 &P4);
   void DrawTriangle(const vec3 &P1, const vec3 &P2, const vec3 &P3);
   void Here(vec3 &start, vec3 &end);
   void Here(vec3 &start);
   void SetPosition(const vec3 &start, const vec3 &end);
   void SetPosition(const vec3 &point);
   CLine(vec3 windowSize, bool compat = false);
   CLine();
   CLine &operator=(CLine &from);
   virtual ~CLine();
   GLuint CreateVAO();
   void ClearVertices();

   vec3* GetVertex(size_t Index)
   {
      if (Index >= 0 && Index < mVertices.size())
         return &mVertices[Index];
      else
         return nullptr;
   }

   void SetLineMode(LineMode mode)
   {
      mLineMode = mode;
   }

   int setMVP(mat4 mvp)
   {
      MVP = mvp;
      return 1;
   }

   void SetCompatibility(bool shiftOrNot)
   {
      mCompat = shiftOrNot;
   }

   void SetColor(const vec3 &color)
   {
      lineColor = glm::vec4(color.r, color.g, color.b, 1.0f);
   }

   void SetColor(const glm::vec4 &color)
   {
      lineColor = color;
   }

   void SetDashSize(float size)
   {
      dashsize = size;
   }

   void SetGapSize(float gap)
   {
      gapsize = gap;
   }

   void SetLineWidth(float LineWidth) { mLineWidth = LineWidth; }

   int Draw(bool Clear = true);
};
