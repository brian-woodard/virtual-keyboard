#include "CText.h"
#include <filesystem>

CShader *CText::mShader = nullptr;
std::unordered_map<std::string, CText::Character*> CText::mFonts;

CText::CText(mat4 projection, unsigned int size, const char *Font) : mFont(nullptr),
                                                                     mFontFileName(Font),
                                                                     mFontHeight(size),
                                                                     mProjection(projection),
                                                                     mInvertY(false),
                                                                     mTextColor(1.0f)
{
   mFont = GetOrCreateFont();

   // configure VAO/VBO for texture quads
   // -----------------------------------
   GLCALL(glGenVertexArrays(1, &mVao));
   GLCALL(glGenBuffers(1, &mVbo));
   GLCALL(glBindVertexArray(mVao));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
   GLCALL(glEnableVertexAttribArray(0));
   GLCALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
   GLCALL(glBindVertexArray(0));
}

CText::CText(mat4 projection, unsigned int size) : mFont(nullptr),
                                                   mFontFileName("fonts//arial.ttf"),
                                                   mFontHeight(size),
                                                   mProjection(projection),
                                                   mInvertY(false),
                                                   mTextColor(1.0f)
{
   mFont = GetOrCreateFont();

   // configure VAO/VBO for texture quads
   // -----------------------------------
   GLCALL(glGenVertexArrays(1, &mVao));
   GLCALL(glGenBuffers(1, &mVbo));
   GLCALL(glBindVertexArray(mVao));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW));
   GLCALL(glEnableVertexAttribArray(0));
   GLCALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
   GLCALL(glBindVertexArray(0));
}

CText::~CText()
{
   GLCALL(glDeleteVertexArrays(1, &mVao));
   GLCALL(glDeleteBuffers(1, &mVbo));
}

CText::Character* CText::GetOrCreateFont()
{
   // key is font name + font size
   std::string key = mFontFileName + "_" + std::to_string(mFontHeight);

   if (mFonts.count(key) == 0)
   {
      mFonts[key] = InitializeFont();
   }
   
   return mFonts[key];
}

CText::Character* CText::InitializeFont()
{
   if (!mShader)
   {
      mShader = new CShader("text.vs", "text.fs");
      std::cout << "Created shader " << mShader->mID << " for fonts" << std::endl;
   }

   std::cout << "Loading font: " << mFontFileName << " size " << mFontHeight << std::endl;

   if (mFont)
      delete [] mFont;
   mFont = new Character[MAX_GLYPHS];

   mUniformTex = 0;
   mUniformColor = 0;
   mVbo = 0;
   mVao = 0;
   mWindowWidth = 1920.0f;
   mWindowHeight = 1080.0f;
   sx = 2.0f / mWindowWidth;
   sy = 2.0f / mWindowHeight;
   mFace = nullptr;

   /* Initialize the FreeType2 library */
   if (FT_Init_FreeType(&mFt))
   {
      fprintf(stderr, "Could not init freetype library\n");
      return nullptr;
   }

   /* find path to font
   std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
   if (font_name.empty())
   {
      std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
      return -1;
   }
   */
   /* Load a font */
   if (FT_New_Face(mFt, mFontFileName.c_str(), 0, &mFace))
   {
      fprintf(stderr, "Could not open font %s\n", mFontFileName.c_str());
      return nullptr;
   }

   // set size to load glyphs as
   mFontWidth = mFontHeight;
   FT_Set_Pixel_Sizes(mFace, mFontWidth, mFontHeight);

   // disable byte-alignment restriction
   GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

   // load first 128 characters of ASCII set
   for (unsigned char c = 0; c < MAX_GLYPHS; c++)
   {
      // Load character glyph
      if (FT_Load_Char(mFace, c, FT_LOAD_RENDER))
      {
         std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
         continue;
      }
      // generate texture
      unsigned int texture;
      GLCALL(glGenTextures(1, &texture));
      GLCALL(glBindTexture(GL_TEXTURE_2D, texture));
      GLCALL(glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          mFace->glyph->bitmap.width,
          mFace->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          mFace->glyph->bitmap.buffer));
      // set texture options
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
      // now store character for later use
      Character character = {
          texture,
          glm::ivec2(mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows),
          glm::ivec2(mFace->glyph->bitmap_left, mFace->glyph->bitmap_top),
          static_cast<unsigned int>(mFace->glyph->advance.x)};

      mFont[c] = character;
   }
   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
   // destroy FreeType once we're finished
   FT_Done_Face(mFace);
   FT_Done_FreeType(mFt);

   return mFont;
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void CText::Print(string &text, float x, float y, float scale, glm::vec3 color)
{
   // activate corresponding render state
   SetColor(color);
   Print(text.c_str(), x, y, scale);
}

float CText::Print(const char *Text, float X, float Y, float Scale)
{
   float x = X;

   if (!Text || !mFont)
      return 0;

   // activate corresponding render state
   mShader->use();
   mShader->setMat4("projection", mProjection);
   mShader->setVec3("textColor", mTextColor);

   GLCALL(glActiveTexture(GL_TEXTURE0));
   GLCALL(glBindVertexArray(mVao));

   // iterate through all characters
   for (size_t i = 0; i < strlen(Text); i++)
   {
      int ch_idx = Text[i];
      if (ch_idx < 0 || ch_idx > MAX_GLYPHS)
         continue;

      Character ch = mFont[ch_idx];

      float xpos = x + ch.Bearing.x * Scale;
      float ypos = Y - (ch.Size.y - ch.Bearing.y) * Scale;

      if (mInvertY)
         ypos = Y - ch.Bearing.y * Scale;

      float w = ch.Size.x * Scale;
      float h = ch.Size.y * Scale;
      // update VBO for each character
      if (mInvertY)
      {
         float vertices[6][4] = {
             {xpos, ypos + h, 0.0f, 1.0f},
             {xpos, ypos, 0.0f, 0.0f},
             {xpos + w, ypos, 1.0f, 0.0f},

             {xpos, ypos + h, 0.0f, 1.0f},
             {xpos + w, ypos, 1.0f, 0.0f},
             {xpos + w, ypos + h, 1.0f, 1.0f}};

         // render glyph texture over quad
         GLCALL(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
         // update content of VBO memory
         GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
         GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData
      }
      else
      {
         float vertices[6][4] = {
             {xpos, ypos + h, 0.0f, 0.0f},
             {xpos, ypos, 0.0f, 1.0f},
             {xpos + w, ypos, 1.0f, 1.0f},

             {xpos, ypos + h, 0.0f, 0.0f},
             {xpos + w, ypos, 1.0f, 1.0f},
             {xpos + w, ypos + h, 1.0f, 0.0f}};

         // render glyph texture over quad
         GLCALL(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
         // update content of VBO memory
         GLCALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
         GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData
      }

      GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
      // render quad
      GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6));
      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch.Advance >> 6) * Scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
   }
   GLCALL(glBindVertexArray(0));
   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

   // return width of displayed text
   return (x - X);
}

float CText::GetWidth(const char *Text, float Scale)
{
   float width = 0;

   if (!Text || !mFont)
      return 0;

   // iterate through all characters
   for (size_t i = 0; i < strlen(Text); i++)
   {
      int ch_idx = Text[i];
      if (ch_idx < 0 || ch_idx > MAX_GLYPHS)
         continue;

      Character ch = mFont[ch_idx];

      width += (ch.Advance >> 6) * Scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
   }

   return width;
}

void CText::Test()
{
   vec3 green(0.0f, 1.0f, 0.0f);
   vec3 red(1.0f, 0.0f, 0.0f);

   string testString("The Quick Brown Fox Jumps Over The Lazy Dog");

   // Print(testString, 25.0f, 25.0f, 1.0f, green);
   Print(testString, 0.5f, 0.5f, 1.0f, green);

   /*
   Print("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

   // Scaling the texture versus changing the font size
   Print("The Small Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 175 * sy, sx * 0.5, sy * 0.5);
   FT_Set_Pixel_Sizes(mFace, 0, 24);
   Print("The Small Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 200 * sy, sx, sy);
   FT_Set_Pixel_Sizes(mFace, 0, 48);
   Print("The Tiny Texture Scaled Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 235 * sy, sx * 0.25, sy * 0.25);
   FT_Set_Pixel_Sizes(mFace, 0, 12);
   Print("The Tiny Font Sized Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 250 * sy, sx, sy);
   FT_Set_Pixel_Sizes(mFace, 0, 48);

   // Colors and transparency
   Print("The Solid Black Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 430 * sy, sx, sy);

   glUniform4fv(mUniformColor, 1, red);
   Print("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 330 * sy, sx, sy);
   Print("The Solid Red Fox Jumps Over The Lazy Dog", -1 + 28 * sx, 1 - 450 * sy, sx, sy);

   glUniform4fv(mUniformColor, 1, transparent_green);
   Print("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 380 * sy, sx, sy);
   Print("The Transparent Green Fox Jumps Over The Lazy Dog", -1 + 18 * sx, 1 - 440 * sy, sx, sy);
   */
}
