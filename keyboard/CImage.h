#pragma once

#include "CShaderUtils.h"
#include <vector>
#include <SOIL2.h>
#include "stb_image.h"

#define VERT_SIZE 32
#define IND_SIZE 6

class CImage
{
   static CShader *mShader;

public:
   CImage(std::string imFname, unsigned int mUnit);
   ~CImage();

   void SetTextNum(int num) { mGlTexUnit = num; }
   void DrawImage(void);
   void SetAlpha(float Alpha) { mAlpha = Alpha; }
   void SetVertices(float *vert = nullptr);
   void CreateVAO(void);

   void SetScale(float Scale)
   {
      mModel = glm::scale(mModel, glm::vec3(Scale, Scale, 1.0f));
   }

   void SetTranslate(const glm::vec3 &Translate)
   {
      mModel = glm::translate(mModel, Translate);
   }

private:
   std::string mFileName;
   unsigned int VBO, VAO, EBO;
   unsigned int mTexture;
   bool mImageFail;
   unsigned int mGlTexUnit;
   float mVertices[VERT_SIZE];
   unsigned int mIndices[IND_SIZE];
   glm::mat4 mModel;
   float mAlpha;

   void LoadImage(void);
};
