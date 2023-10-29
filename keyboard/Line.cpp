#include "Line.h"

CShader *CLine::mShader = nullptr;

CLine::CLine() : mStartPoint(vec3(0.0f, 0.0f, 0.0f)),
                 mEndPoint(vec3(0.0f, 0.0f, 0.0f)),
                 mWindowSize(vec3(1920.0f, 1080.0f, 0.0f)),
                 w(1920.0f), h(1080.0f), mLineWidth(-1.0f),
                 mCompat(false),
                 MVP(1.0f), mModel(1.0f), mView(1.0), mClipPlane(1.0)
{
   if (!mShader)
   {
      mShader = new CShader("lines.vs", "lines.fs");
      std::cout << "Created shader " << mShader->mID << " for lines" << std::endl;
   }

   VBO = 0;
   VAO = 0;
   x1 = 0.0f;
   y1 = 0.0f;
   x2 = 0.0f;
   y2 = 0.0f;
   lineColor = glm::vec4(0, 1, 0, 1);
   pattern = 0xFFFF; // solid line
   resolution = 0;
   mLineMode = SOLID;
   dashsize = 1; //?
   gapsize = 0;  //?
}

CLine::CLine(vec3 windowSize, bool compat) : mStartPoint(vec3(0.0f, 0.0f, 0.0f)),
                                             mEndPoint(vec3(0.0f, 0.0f, 0.0f)),
                                             mWindowSize(windowSize),
                                             w(windowSize.x), h(windowSize.y), mLineWidth(-1.0f),
                                             mCompat(compat),
                                             MVP(1.0f), mModel(1.0f), mView(1.0), mClipPlane(1.0)
{
   if (!mShader)
   {
      mShader = new CShader("lines.vs", "lines.fs");
      std::cout << "Created shader " << mShader->mID << " for lines" << std::endl;
   }

   VBO = 0;
   VAO = 0;
   x1 = 0.0f;
   y1 = 0.0f;
   x2 = 0.0f;
   y2 = 0.0f;
   lineColor = glm::vec4(0, 1, 0, 1);
   pattern = 0xFFFF; // solid line
   dashsize = 1;     //?
   gapsize = 0;      //?
   resolution = 0;
   mLineMode = SOLID;
}

void CLine::DrawQuad(float X, float Y, float Width, float Height)
{
   mLineMode = TRIANGLE;
   glm::vec3 p1 = glm::vec3(X, Y, 0.0f);
   glm::vec3 p2 = p1 + glm::vec3(Width, 0.0f, 0.0f);
   glm::vec3 p3 = p1 + glm::vec3(0.0f, Height, 0.0f);
   glm::vec3 p4 = p1 + glm::vec3(Width, Height, 0.0f);

   mVertices.clear();

   mVertices.push_back(p1);
   mVertices.push_back(p2);
   mVertices.push_back(p4);

   mVertices.push_back(p1);
   mVertices.push_back(p4);
   mVertices.push_back(p3);

   Draw();
}

void CLine::DrawQuad(const vec3 &P1, const vec3 &P2, const vec3 &P3, const vec3 &P4)
{
   mLineMode = TRIANGLE;
   mVertices.clear();

   mVertices.push_back(P1);
   mVertices.push_back(P2);
   mVertices.push_back(P4);

   mVertices.push_back(P1);
   mVertices.push_back(P4);
   mVertices.push_back(P3);

   Draw();
}

void CLine::DrawTriangle(const vec3 &P1, const vec3 &P2, const vec3 &P3)
{
   mLineMode = TRIANGLE;
   mVertices.clear();

   mVertices.push_back(P1);
   mVertices.push_back(P2);
   mVertices.push_back(P3);

   Draw();
}

void CLine::Here(vec3 &start, vec3 &end)
{
   x1 = start.x;
   y1 = start.y;
   x2 = end.x;
   y2 = end.y;

   mStartPoint = start;
   mEndPoint = end;

   SetVertices();
}

void CLine::Here(vec3 &start)
{
   x1 = start.x;
   y1 = start.y;

   mStartPoint = start;

   SetVertice();
}

void CLine::SetPosition(const vec3 &start, const vec3 &end)
{
   mVertices.push_back(start);
   mVertices.push_back(end);
}

void CLine::SetPosition(const vec3 &point)
{
   mVertices.push_back(point);
}

void CLine::SetVertice(void)
{
   if (!mCompat)
   {
      // ADD screen size to get drawing offset at ctr of screen
      float ctr_x = w / 2;
      float ctr_y = h / 2;

      x1 += ctr_x;
      y1 += ctr_y;
   }

   // convert 3d world space position 2d screen space position
   x1 = 2 * x1 / w - 1;
   y1 = 2 * y1 / h - 1;

   // add ctr x,y so it fossets at center
   mStartPoint.x = x1;
   mStartPoint.y = y1;

   mVertices.push_back(mStartPoint);
}

void CLine::SetVertices(void)
{

   if (!mCompat)
   {
      // ADD screen size to get drawing offset at ctr of screen
      float ctr_x = w / 2;
      float ctr_y = h / 2;

      x1 += ctr_x;
      y1 += ctr_y;
      x2 += ctr_x;
      y2 += ctr_y;
   }

   // convert 3d world space position 2d screen space position
   x1 = 2 * x1 / w - 1;
   y1 = 2 * y1 / h - 1;

   x2 = 2 * x2 / w - 1;
   y2 = 2 * y2 / h - 1;

   // add ctr x,y so it fossets at center
   mStartPoint.x = x1;
   mStartPoint.y = y1;
   mEndPoint.x = x2;
   mEndPoint.y = y2;

   mVertices.push_back(mStartPoint);
   mVertices.push_back(mEndPoint);
}

CLine::~CLine()
{
   GLCALL(glDeleteVertexArrays(1, &VAO));
   GLCALL(glDeleteBuffers(1, &VBO));
}

CLine &CLine::operator=(CLine &from)
{
   VBO = from.VBO;
   VAO = from.VAO;
   mVertices = from.mVertices;
   mStartPoint = from.mStartPoint;
   mEndPoint = from.mEndPoint;
   mWindowSize = from.mWindowSize;
   MVP = from.MVP;
   lineColor = from.lineColor;
   pattern = from.pattern;
   resolution = from.resolution;
   mLineMode = from.mLineMode;

   return *this;
}

// used for polygons
GLuint CLine::CreateVAO()
{
   GLCALL(glGenBuffers(1, &VBO));
   GLCALL(glGenVertexArrays(1, &VAO));
   GLCALL(glBindVertexArray(VAO));
   GLCALL(glEnableVertexAttribArray(0));
   GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
   GLCALL(glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(*mVertices.data()), mVertices.data(), GL_DYNAMIC_DRAW));
   GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

   return VAO;
}

void CLine::ClearVertices()
{
   mVertices.clear();
   GLCALL(glDeleteVertexArrays(1, &VAO));
   GLCALL(glDeleteBuffers(1, &VBO));
}

int CLine::Draw(bool Clear)
{
   mShader->use();

   if (mLineMode != DASH)
   {
      // need to reset once the above is used
      dashsize = 0;
      gapsize = 0;
   }

   if (mLineWidth != -1.0f)
      GLCALL(glLineWidth(mLineWidth));

   if (Clear)
      CreateVAO();

   GLCALL(glBindVertexArray(VAO));

   mShader->setMat4("MVP", MVP);
   mShader->setVec4("color", lineColor);
   mShader->setVec2("resolution", glm::vec2(w, h));
   mShader->setFloat("dashSize", dashsize);
   mShader->setFloat("gapSize", gapsize);

   switch (mLineMode)
   {
   case DASH:
   {
      GLCALL(glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)mVertices.size()));
      break;
   }
   case SOLID:
   {
      GLCALL(glDrawArrays(GL_LINES, 0, (GLsizei)mVertices.size()));
      break;
   }
   case TRIANGLE:
   {
      GLCALL(glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mVertices.size()));
      break;
   }
   case TRIANGLE_FAN:
   {
      GLCALL(glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)mVertices.size()));
      break;
   }
   default:
      break;
   }

   if (Clear)
   {
      ClearVertices();
   }

   return 1;
}
