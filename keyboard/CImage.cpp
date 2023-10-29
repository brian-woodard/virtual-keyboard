
#include "CImage.h"

CShader *CImage::mShader = nullptr;

// Texture num are the GL_TEXTURE0-15 values
CImage::CImage(std::string imFname, unsigned int mUnit) : mFileName(imFname),
                                                          VBO(0),
                                                          VAO(0),
                                                          EBO(0),
                                                          mTexture(0),
                                                          mImageFail(false),
                                                          mGlTexUnit(mUnit),
                                                          mModel(1.0f),
                                                          mAlpha(1.0f)
{
   if (mShader == nullptr)
   {
      mShader = new CShader("texture.vs", "texture.fs");
      std::cout << "Created shader " << mShader->mID << " for images" << std::endl;
   }

   memset(mVertices, 0, sizeof(mVertices));

   mIndices[0] = 2;
   mIndices[1] = 3;
   mIndices[2] = 0;
   mIndices[3] = 0;
   mIndices[4] = 1;
   mIndices[5] = 2;

   LoadImage();
}

CImage::~CImage()
{
   GLCALL(glDeleteVertexArrays(1, &VAO));
   GLCALL(glDeleteBuffers(1, &VBO));
   GLCALL(glDeleteBuffers(1, &EBO));

   std::cout << "CIMAGE DEleting VAO, VBO, and EBO " << std::endl;
}

void CImage::SetVertices(float *vert)
{
   if (vert)
   {
      for (int i = 0; i < VERT_SIZE; i++)
         mVertices[i] = vert[i];
   }
   CreateVAO();
}

void CImage::LoadImage(void)
{
   int width = 0;
   int height = 0;
   int channels = 0;

   unsigned int imageFormat = 0;
   unsigned int dataFormat = 0;

   GLCALL(glGenTextures(1, &mTexture));
   GLCALL(glBindTexture(GL_TEXTURE_2D, mTexture));
   // set the texture wrapping parameters
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); // set texture wrapping to GL_REPEAT (default wrapping method)
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
   // set texture filtering parameters
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
   GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

   stbi_set_flip_vertically_on_load(1);
   unsigned char *imagemap = SOIL_load_image(
       mFileName.c_str(),
       &width, &height, &channels,
       SOIL_LOAD_AUTO);

   if (imagemap)
   {
      if (channels == 4)
      {
         imageFormat = GL_RGBA8;
         dataFormat = GL_RGBA;
         // std::cout << "Texture Format, Channels: " << channels << std::endl;
      }
      else if (channels == 3)
      {
         imageFormat = GL_RGB8;
         dataFormat = GL_RGB;
         // std::cout << "Texture Format, Channels: " << channels << std::endl;
      }
      else
      {
         std::cout << "Texture Format Not Supported, Channels: " << channels << std::endl;
         mImageFail = true;
         SOIL_free_image_data(imagemap);
         return;
      }

      GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, imagemap));
      GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

      SOIL_free_image_data(imagemap);

      mImageFail = false;
   }
   else
   {
      mImageFail = true;
      std::cout << "Error: Image " << mFileName << " failed to load!" << std::endl;
   }
}

void CImage::CreateVAO(void)
{
   if (VAO != 0)
   {
      GLCALL(glDeleteVertexArrays(1, &VAO));
      GLCALL(glDeleteBuffers(1, &VBO));
      GLCALL(glDeleteBuffers(1, &EBO));
   }

   GLCALL(glGenVertexArrays(1, &VAO));
   GLCALL(glGenBuffers(1, &VBO));
   GLCALL(glGenBuffers(1, &EBO));

   GLCALL(glBindVertexArray(VAO));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_DYNAMIC_DRAW));
   GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
   GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices), mIndices, GL_DYNAMIC_DRAW));

   // position attribute
   GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0));
   GLCALL(glEnableVertexAttribArray(0));
   // color attribute
   GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))));
   GLCALL(glEnableVertexAttribArray(1));
   // texture coord attribute
   GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))));
   GLCALL(glEnableVertexAttribArray(2));
}

void CImage::DrawImage(void)
{
   if (!mImageFail)
   {
      glm::mat4 projection(1.0f);
      glm::mat4 view(1.0f);
      glm::mat4 mvp = projection * view * mModel;

      mShader->use();
      mShader->setMat4("MVP", mvp);
      mShader->setInt("textureUnit", mGlTexUnit-GL_TEXTURE0);
      mShader->setFloat("Alpha", mAlpha);

      GLCALL(glActiveTexture(mGlTexUnit));
      GLCALL(glBindTexture(GL_TEXTURE_2D, mTexture));

      GLCALL(glBindVertexArray(VAO));
      GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

      // reset model matrix
      mModel = glm::mat4(1.0f);
   }
}
