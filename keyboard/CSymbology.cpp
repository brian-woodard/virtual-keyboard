//-----------------------------------------------------------------------------
//                            UNCLASSIFIED
//-----------------------------------------------------------------------------
//                    DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
//  This software and the accompanying documentation are provided to the U.S.
//  Government with unlimited rights as provided in DFARS � 252.227-7014.  The
//  contractor, Veraxx Engineering Corporation, retains ownership, the
//  copyrights, and all other rights.
//
//  � Veraxx Engineering Corporation, 2022.  All rights reserved.
//
// DEVELOPED BY:
//  Veraxx Engineering Corporation
//  14130 Sullyfield Circle, Suite B
//  Chantilly, VA 20151
//  www.Veraxx.com
//  (703)880-9000 (Voice)
//  (703)880-9005 (Fax)
//-----------------------------------------------------------------------------
//
//                         Distribution Warning:
//  WARNING - This file contains technical data whose export is restricted by
//  the Arms Export Control Act (Title 22, U.S.C., Sec. 2751 et seq.) or
//  Executive Order 12470. Violations of these export laws are subject to severe
//  criminal penalties. Disseminate in accordance with provisions of DoD
//  Directive 5230.25
//
//-----------------------------------------------------------------------------
//
//! Title:      Tss Symbology DLL
//! Class:      CPP Source
//! Filename:   CSymbology.cpp
//! Author:     JP Scheff
//! Purpose:    Symbology for the TSS Sensor
//
//-----------------------------------------------------------------------------

#include <math.h>
#include <string>
#include <list>
#include <numeric>
#include <algorithm>
#include <sstream>
#include "CSymbology.h"

// Unlike Aechelon DLL, we are setting the drawing coordinate system
// to provide an match the display resolution.  This should allow exact
// pixel placement.
const float CSymbology::HOST_COORD_X_MAX = 1920.0f;
const float CSymbology::HOST_COORD_Y_MAX = 1080.0f;

// Only allow symbol and word IDs from 0 to 255 so we can use 1-byte in
// the host interface to identify these objects.
#define MAX_SYMBOL_ID 255
#define MAX_WORD_ID 255

// CSymbology construction/destruction

CSymbology::CSymbology(glm::mat4 &projection, glm::vec3 &windowSize,
                       unsigned int size, const std::string &imgFile) : mFontSize(size),
                                                                  mText(projection, size),
                                                                  mLine(vec3(windowSize.x, windowSize.y, 0.0f))
{
   mLeftCoordLimit = -1.0;
   mRightCoordLimit = 1.0;
   mTopCoordLimit = 1.0;
   mBottomCoordLimit = -1.0;

   // mInitialized = false;

   mWidth = windowSize.x;
   mHeight = windowSize.y;
   mProjection = projection;
   mYseparation = 10.0f;
   mXseparation = 10.0f;
   mInfoBoxMaxWidth = mWidth / 3;
   mInfoBoxMaxHeight = mHeight / 3;
   mMaxPos_x = mWidth;
   mMaxPos_y = mHeight;
   mMaxNeg_x = 0.0f;
   mMaxNeg_y = 0.0f;

   mPositionInit = false;
   FromHostUH = new T_FromHostToSymbologyUH;
   FromHostAH = new T_FromHostToSymbologyAH;

   int arr[2][5] = {{1, 8, 12, 20, 25}, {5, 9, 13, 24, 26}};
}

CSymbology::~CSymbology()
{
   if (FromHostUH)
      delete FromHostUH;
   if (FromHostAH)
      delete FromHostAH;
}

void CSymbology::DrawLine(vec3 &start, vec3 &end, bool compat)
{
   vec3 color(0, 1, 0);

   ///////////////////////////////////////////////////////////////////////
   // only 3d drawing
   // vec3 position = vec3(cos(radians(mAngle)), 1, sin(radians(mAngle)));
   // vec3 position = vec3(4.0f, 3.0f, 3.0f);
   // mat4 view = glm::lookAt(position, vec3(0, 0, 0), vec3(0, 1, 0));
   // glm::mat4 model = glm::mat4(1.0f);
   // line.setMVP(mProjection * view * model);
   ///////////////////////////////////////////////////////////////////////

   // before we calculate where here is
   mLine.SetCompatibility(compat);
   mLine.Here(start, end);
   mLine.Draw();
   // reset line mode
}

void CSymbology::DrawScaleBarReticle(void)
{
   float len;
   float offy;
   // default
   GLCALL(glLineWidth(2.5));
   vec3 start;
   vec3 end;

   CText stat(mProjection, 38);
   vec3 green(0.0f, 1.0f, 0.0f);

   float fixTab = 40;
   float offLeft = mWidth / 2 - stat.GetWidth();
   float offRight = mWidth / 2 + stat.GetWidth();
   offy = mHeight / 2 + fixTab;

   mLine.SetLineMode(SOLID);
   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);

   int idis = (int)FromHostAH->yardstick_dist;

   std::string dis = std::to_string(idis);
   len = idis;

   // Upper Right
   start = vec3(offRight, offy + fixTab, 0.0f);
   end = vec3(offRight + len, offy + fixTab, 0.0f);
   mLine.Here(start, end);
   mLine.Draw();

   start = end;
   end = vec3(offRight + len, offy, 0.0f);
   mLine.Here(start, end);
   mLine.Draw();

   stat.Print(dis, mWidth / 2 - stat.GetWidth() / 2, offy + fixTab - stat.GetHeight() / 2, 1.0f, green);

   // Upper Left
   start = vec3(offLeft, offy + fixTab, 0.0f);
   end = vec3(offLeft - len, offy + fixTab, 0.0f);
   mLine.Here(start, end);
   mLine.Draw();

   start = end;
   end = vec3(offLeft - len, offy, 0.0f);
   mLine.Here(start, end);
   mLine.Draw();
}

void CSymbology::DrawPrimaryLockedTargetReticle(void)
{
   float off, len;
   float offy;
   // default
   GLCALL(glLineWidth(2.5));
   vec3 start;
   vec3 end;

   // F O V    B R A C K E T S
   /*
   if (!FromHostAH->fov_zoom && ((FromHostAH->fov < 2 && FromHostAH->video_type == TVC) ||
      (FromHostAH->fov < 3 && FromHostAH->video_type == FLIR)))
   {
   */
   FromHostAH->fov = 0;

   len = 35.0;
   if (FromHostAH->fov == 0)
   {
      off = 103;
      offy = 83;
   }
   else if (FromHostAH->fov == 1)
   {
      off = 103;
      offy = 83;
   }
   else
   {
      off = 320;
      offy = off - 40;
   }

   // Upper Right
   start = vec3(off, offy, 0.0f);
   end = vec3(off - len, offy, 0.0f);
   DrawLine(start, end);

   start = vec3(off, offy, 0.0f);
   end = vec3(off, offy - len, 0.0f);
   DrawLine(start, end);

   // Upper Left
   start = vec3(-off, offy, 0.0f);
   end = vec3(-off + len, offy, 0.0f);
   DrawLine(start, end);

   start = vec3(-off, offy, 0.0f);
   end = vec3(-off, offy - len, 0.0f);
   DrawLine(start, end);

   // Lower  Left
   start = vec3(-off, -offy, 0.0f);
   end = vec3(-off + len, -offy, 0.0);
   DrawLine(start, end);

   start = vec3(-off, -offy, 0.0f);
   end = vec3(-off, -offy + len, 0.0f);
   DrawLine(start, end);

   // Lower  Right
   start = vec3(off, -offy, 0.0f);
   end = vec3(off - len, -offy, 0.0f);
   DrawLine(start, end);

   start = vec3(off, -offy, 0.0f);
   end = vec3(off, -offy + len, 0.0f);
   DrawLine(start, end);
   //}
}

void CSymbology::DrawPotentialTargetRecticule(void)
{
   float off = 0;
   float len = 50;
   float x_off, y_off;
   float x1, x2, y1, y2, angle, radius;
   vec3 start;
   vec3 end;
   GLCALL(glLineWidth(15));
   vec3 green(0.0f, 1.0f, 0.0f);

   x_off = (0.0f);
   y_off = (0.0f);

   // 12 O'Clock Vert Line
   start = vec3(x_off, y_off + off, 0.0);
   end = vec3(x_off, y_off + off + len, 0.0);
   DrawLine(start, end);

   // 6 O'Clock Vert Line
   start = vec3(x_off, y_off + -off, 0.0);
   end = vec3(x_off, y_off + -off - len, 0.0);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off, y_off, 0.0);
   end = vec3(x_off + off + len, y_off, 0.0);
   DrawLine(start, end);

   // 9 O'Clock Vert Line
   start = vec3(x_off - off, y_off, 0.0);
   end = vec3(x_off - off - len, y_off, 0.0);
   DrawLine(start, end);

   GLCALL(glLineWidth(2.5));
}

float CSymbology::GetTextWidth(std::string &msg)
{
   float textWidth = mText.GetWidth() * msg.length();

   return textWidth;
}

float CSymbology::GetTextHeight(std::string &msg)
{
   float textHeight = mText.GetHeight();

   return textHeight;
}

void CSymbology::PrintText(std::string &_string)
{
}

void CSymbology::DrawStatusMsg(void)
{
   // default
   GLCALL(glLineWidth(2.5));
   CText stat(mProjection, 38);
   vec3 green(0.0f, 1.0f, 0.0f);

   std::string msg1("TSS FLIR CALIBRATION IN PROGRESS");
   std::string msg2("TSS BORESIGHT COMPLETE");
   std::string msg3("TSS BORESIGHT ABORTED");

   float wx, wy;

   GetBCSection(wx, wy);
   wx = (mWidth / 2) - (stat.GetWidth() * (msg1.size() / 4));
   wy = mHeight / 10;

   if (FromHostAH->status_msg == 1)
   {
      stat.Print(msg1, wx, wy, 1.0f, green);
   }
   else if (FromHostAH->status_msg == 2)
   {
      stat.Print(msg2, wx, wy, 1.0f, green);
   }
   else if (FromHostAH->status_msg == 3)
   {
      stat.Print(msg3, wx, wy, 1.0f, green);
   }
}

void CSymbology::DrawLaserIlluminatorReticle(void)
{
   vec3 start;
   vec3 end;
   float off, len;

   float x_off = (0.5f - FromHostAH->los_x_pos) * 960.0f;
   float y_off = (0.5f - FromHostAH->los_y_pos) * 540.0f;
   off = 26;
   len = 82;

   // 12 O'Clock Horizontal Line
   start = vec3(x_off - off - len, y_off + off + len, 0.0f);
   end = vec3(x_off + off + len, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Horiz Line
   start = vec3(x_off - off - len, y_off - off - len, 0.0f);
   end = vec3(x_off + off + len, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off + len, y_off + off + len, 0.0f);
   end = vec3(x_off + off + len, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock vert Line
   start = vec3(x_off - off - len, y_off + off + len, 0.0f);
   end = vec3(x_off - off - len, y_off - off - len, 0.0f);
   DrawLine(start, end);
}

void CSymbology::DrawPointStyleLOSReticle(void)
{
   vec3 start;
   vec3 end;
   float len;

   float x_off = 960.0f;
   float y_off = 540.0f;
   len = 10;

   GLCALL(glLineWidth(2.5));
   vec3 green(0.0f, 1.0f, 0.0f);

   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);

   mLine.SetLineMode(TRIANGLE);

   len = 10;
   start = vec3(x_off - len / 2, y_off - len / 2, 0.0f);
   mLine.Here(start);

   end = vec3(x_off + len / 2, y_off - len / 2, 0.0f);
   mLine.Here(end);

   start = vec3(x_off + len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);
   mLine.Draw();

   start = vec3(x_off + len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);

   start = vec3(x_off - len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);

   start = vec3(x_off - len / 2, y_off - len / 2, 0.0f);
   mLine.Here(start);
   mLine.Draw();

   mLine.SetLineMode(SOLID);
}

void CSymbology::DrawLaserDesignatorReticle(void)
{

   vec3 start;
   vec3 end;
   float off, len;

   float x_off = 40;
   float y_off = 0;
   float y_off2 = 40;
   off = 26;
   len = 82;
   float gap = 40;

   // 12 O'Clock Horizontal Line
   start = vec3(x_off - off - len + gap, y_off + off + len, 0.0f);
   end = vec3(x_off + off + len - gap, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Horiz Line
   start = vec3(x_off - off - len + gap, y_off - off - len, 0.0f);
   end = vec3(x_off + off + len - gap, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off + len, y_off + off + len - gap, 0.0f);
   end = vec3(x_off + off + len, y_off - off - len + gap, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock vert Line
   start = vec3(x_off - off - len, y_off + off + len - gap, 0.0f);
   end = vec3(x_off - off - len, y_off - off - len + gap, 0.0f);
   DrawLine(start, end);
}

void CSymbology::DrawAquisitionGateReticle(void)
{

   vec3 start;
   vec3 end;
   float off, len;

   float x_off = (0.5f - FromHostAH->los_x_pos) * 960.0f;
   float y_off = (0.5f - FromHostAH->los_y_pos) * 540.0f;
   off = 26;
   len = 82;

   mLine.SetLineMode(DASH);
   mLine.SetDashSize(10.0f);
   mLine.SetGapSize(10.0f);

   // 12 O'Clock Horizontal Line
   start = vec3(x_off - off - len, y_off + off + len, 0.0f);
   end = vec3(x_off + off + len, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Horiz Line
   start = vec3(x_off - off - len, y_off - off - len, 0.0f);
   end = vec3(x_off + off + len, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off + len, y_off + off + len, 0.0f);
   end = vec3(x_off + off + len, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock vert Line
   start = vec3(x_off - off - len, y_off + off + len, 0.0f);
   end = vec3(x_off - off - len, y_off - off - len, 0.0f);
   DrawLine(start, end);

   mLine.SetLineMode(SOLID);
}

void CSymbology::DrawStandardLOSReticle(void)
{
   float off, len;

   vec3 start;
   vec3 end;

   float x_off = (0.5f - FromHostAH->los_x_pos) * 960.0f;
   float y_off = (0.5f - FromHostAH->los_y_pos) * 540.0f;
   off = 26;
   len = 52;

   // 12 O'Clock Vert Line
   start = vec3(x_off, y_off + off, 0.0f);
   end = vec3(x_off, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Vert Line
   start = vec3(x_off, y_off + -off, 0.0f);
   end = vec3(x_off, y_off + -off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off, y_off, 0.0f);
   end = vec3(x_off + off + len, y_off, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock Vert Line
   start = vec3(x_off - off, y_off, 0.0f);
   end = vec3(x_off - off - len, y_off, 0.0f);
   DrawLine(start, end);
}

void CSymbology::DrawLaserangeFinderReticle(void)
{
   float off, len;

   vec3 start;
   vec3 end;

   float x_off = (0.5f - FromHostAH->los_x_pos) * 960.0f;
   float y_off = (0.5f - FromHostAH->los_y_pos) * 540.0f;
   off = 26;
   len = 52;

   // 12 O'Clock Vert Line
   start = vec3(x_off, y_off + off, 0.0f);
   end = vec3(x_off, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 12 O'Clock Horizontal Line
   start = vec3(x_off - off, y_off + off + len, 0.0f);
   end = vec3(x_off + off, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Horiz Line
   start = vec3(x_off, y_off - off, 0.0f);
   end = vec3(x_off, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock vert Line
   start = vec3(x_off - off, y_off - off - len, 0.0f);
   end = vec3(x_off + off, y_off - off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Horiz Line
   start = vec3(x_off + off, y_off, 0.0f);
   end = vec3(x_off + off + len, y_off, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off + len, y_off + off, 0.0f);
   end = vec3(x_off + off + len, y_off - off, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock horiz Line
   start = vec3(x_off - off, y_off, 0.0f);
   end = vec3(x_off - off - len, y_off, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock vert Line
   start = vec3(x_off - off - len, y_off + off, 0.0f);
   end = vec3(x_off - off - len, y_off - off, 0.0f);
   DrawLine(start, end);
}

void CSymbology::DrawCtrLineNortIndicator(void)
{
   vec3 start;
   vec3 end;
   double rot = 45.0 * 3.1415 / 180.0;
   vec3 origin;
   float off = 50;
   float len = 150;

   float x_off = 960.0f;
   float y_off = 540.0f;

   mLine.SetLineMode(SOLID);
   GLCALL(glLineWidth(2.5));
   vec3 green(0.0f, 1.0f, 0.0f);

   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);

   // origin for the arrow start @ bottom upper left
   origin = vec3(x_off - len / 3 * cos(rot), y_off + len / 3 * sin(rot), 0.0f);

   start = vec3(origin.x, origin.y, 0.0);
   end = vec3(start.x - len * cos(rot), start.y + (len * sin(rot)), 0.0f);
   DrawLine(start, end, mCompatibility);

   mLine.SetLineMode(TRIANGLE);

   len = 10;
   start = vec3(x_off - len / 2, y_off - len / 2, 0.0f);
   mLine.Here(start);

   end = vec3(x_off + len / 2, y_off - len / 2, 0.0f);
   mLine.Here(end);

   start = vec3(x_off + len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);
   mLine.Draw();

   start = vec3(x_off + len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);

   start = vec3(x_off - len / 2, y_off + len / 2, 0.0f);
   mLine.Here(start);

   start = vec3(x_off - len / 2, y_off - len / 2, 0.0f);
   mLine.Here(start);
   mLine.Draw();
   mLine.SetLineMode(SOLID);
}

void CSymbology::DrawCtrArrowNortIndicator(void)
{
   vec3 start;
   vec3 end;

   double rot = 45.0 * 3.1415 / 180.0;
   vec3 origin;
   mat4 trans(1.0f); // init to identiy mat
   mat4 iden(1.0f);
   float space = 50;

   GLCALL(glLineWidth(2.5));
   vec3 green(0.0f, 1.0f, 0.0f);

   float x_off = (0.5f - FromHostAH->los_x_pos) * 960.0f;
   float y_off = (0.5f - FromHostAH->los_y_pos) * 540.0f;
   float off = 26;
   float len = 52;

   // 12 O'Clock Vert Line
   start = vec3(x_off, y_off + off, 0.0f);
   end = vec3(x_off, y_off + off + len, 0.0f);
   DrawLine(start, end);

   // 6 O'Clock Vert Line
   start = vec3(x_off, y_off + -off, 0.0f);
   end = vec3(x_off, y_off + -off - len, 0.0f);
   DrawLine(start, end);

   // 3 O'Clock Vert Line
   start = vec3(x_off + off, y_off, 0.0f);
   end = vec3(x_off + off + len, y_off, 0.0f);
   DrawLine(start, end);

   // 9 O'Clock Vert Line
   start = vec3(x_off - off, y_off, 0.0f);
   end = vec3(x_off - off - len, y_off, 0.0f);
   DrawLine(start, end);

   mLine.SetLineMode(SOLID);

   // origin for the arrow start @ bottom upper left
   origin = vec3(end.x, y_off + off + len, 0.0f);
   len = 60.0f;

   start = vec3(origin.x, origin.y, 0.0);
   end = vec3(start.x - len * cos(rot), start.y + (len * sin(rot)), 0.0f);
   DrawLine(start, end);

   mLine.SetLineMode(TRIANGLE);
   origin = vec3(end.x, end.y, 0.0f);
   rot = 30.0 * 3.1415 / 180.0;
   len = 20.0f;
   end = vec3(origin.x + len * sin(rot), origin.y - len * cos(rot), 0.0f);
   // mLine.setMVP(trans);
   mLine.Here(end);

   // segment 2
   rot = 45.0 * 3.1415 / 180.0;
   start = end;
   // need to use position from previous point
   end = vec3(start.x + len / 2 * cos(rot), start.y + len / 2 * sin(rot), 0.0f);
   mLine.Here(end);

   // segment 3
   rot = 30.0 * 3.1415 / 180.0;
   start = end;
   end = origin;
   mLine.Here(end);
   mLine.Draw();

   // reset to identy mat
   mLine.setMVP(iden);
   mLine.SetLineMode(SOLID);
}

void CSymbology::DrawStandardNorthIndicator(void)
{
   vec3 start;
   vec3 end;

   float x_off = 0;
   float y_off = 0;
   float len = 60.0f;
   double rot = 45.0 * 3.1415 / 180.0;
   float wx, wy;
   string msg1("N");
   CText north(mProjection, 38);
   GetTLSection(wx, wy);
   x_off = wx;
   y_off = wy;
   float textWidth = 0.0f;
   float textHeight = 0.0f;
   vec3 origin;
   mat4 trans(1.0f); // init to identiy mat
   mat4 iden(1.0f);
   float space = 50;

   GLCALL(glLineWidth(2.5));
   vec3 green(0.0f, 1.0f, 0.0f);

   //
   // no need to rotate, but the below code works
   //
   /*
   trans = rotate(trans, radians(-45.0f), vec3(0.0f, 0.0f, 1.0f)); // rotate about the z axis
   mLine.setMVP(trans);
   DrawLine(origin, end, mCompatibility);
   //
   */

   mCompatibility = true;
   mLine.SetLineMode(SOLID);
   mLine.SetCompatibility(mCompatibility);

   // Upper Left
   start = vec3(x_off, y_off, 0.0f);
   end = vec3(start.x - len * cos(rot), start.y + len * sin(rot), 0.0f);
   DrawLine(start, end, mCompatibility);

   textWidth = north.GetWidth() * msg1.length();
   textHeight = north.GetHeight();
   float factor = (textHeight / textWidth);
   north.Print(msg1, end.x - space / 2 * cos(rot) - (textWidth / 3), end.y + space / 2 * sin(rot) - (textHeight / 3), 1.0f, green);

   start = vec3(end.x - space * cos(rot), end.y + space * sin(rot), 0.0);
   end = vec3(start.x - len * cos(rot), start.y + (len * sin(rot)), 0.0f);
   DrawLine(start, end, mCompatibility);

   mLine.SetLineMode(TRIANGLE);
   origin = vec3(end.x, end.y, 0.0f);
   rot = 30.0 * 3.1415 / 180.0;
   len = 20.0f;
   end = vec3(origin.x + len * sin(rot), origin.y - len * cos(rot), 0.0f);
   // mLine.setMVP(trans);
   mLine.Here(end);

   // segment 2
   rot = 45.0 * 3.1415 / 180.0;
   start = end;
   // need to use position from previous point
   end = vec3(start.x + len / 2 * cos(rot), start.y + len / 2 * sin(rot), 0.0f);
   mLine.Here(end);

   // segment 3
   rot = 30.0 * 3.1415 / 180.0;
   start = end;
   end = origin;
   mLine.Here(end);
   mLine.Draw();

   // reset to identy mat
   mLine.setMVP(iden);
   mLine.SetLineMode(SOLID);
}

void CSymbology::DrawNextZoomMarkerGraphic(void)
{
   list<float> x_avg[3];
   list<float> y_avg[3];
   float len = 0;
   float x_off, y_off, x_size, y_size;
   int AVGSIZE = 20;
   bool flash = 0;
   static int flash_counter = 0;
   vec3 start;
   vec3 end;

   x_off = 0;
   y_off = 0;
   x_size = 200 * mHeight / mWidth;
   y_size = 200 * mHeight / mWidth;

   mLine.SetLineMode(DASH);
   mLine.SetDashSize(2.0f);
   mLine.SetGapSize(2.0f);

   len = 19;

   // Upper Left
   start = vec3(x_off - x_size, y_off + y_size, 0.0);
   end = vec3(x_off - x_size + len, y_off + y_size, 0.0);
   DrawLine(start, end);

   start = vec3(x_off - x_size, y_off + y_size, 0.0);
   end = vec3(x_off - x_size, y_off + y_size - len, 0.0);
   DrawLine(start, end);

   // Upper Right
   start = vec3(x_off + x_size, y_off + y_size, 0.0);
   end = vec3(x_off + x_size - len, y_off + y_size, 0.0);
   DrawLine(start, end);

   start = vec3(x_off + x_size, y_off + y_size, 0.0);
   end = vec3(x_off + x_size, y_off + y_size - len, 0.0);
   DrawLine(start, end);

   // Lower Left
   start = vec3(x_off - x_size, y_off - y_size, 0.0);
   end = vec3(x_off - x_size + len, y_off - y_size, 0.0);
   DrawLine(start, end);

   start = vec3(x_off - x_size, y_off - y_size, 0.0);
   end = vec3(x_off - x_size, y_off - y_size + len, 0.0);
   DrawLine(start, end);

   // Lower Right
   start = vec3(x_off + x_size, y_off - y_size, 0.0);
   end = vec3(x_off + x_size - len, y_off - y_size, 0.0);
   DrawLine(start, end);

   start = vec3(x_off + x_size, y_off - y_size, 0.0);
   end = vec3(x_off + x_size, y_off - y_size + len, 0.0);
   DrawLine(start, end);

   mLine.SetLineMode(SOLID);
}

void CSymbology::UpdateHost(T_FromHostToSymbologyUH *_FromHostUH)
{
   memcpy(FromHostUH, _FromHostUH, sizeof(T_FromHostToSymbologyUH));
}

void CSymbology::UpdateHost(T_FromHostToSymbologyAH *_FromHostAH)
{
   memcpy(FromHostAH, _FromHostAH, sizeof(T_FromHostToSymbologyAH));
}

// Note (0,0) is the top left and (1,1) is the bottom right
void CSymbology::GetTLSection(float &_X, float &_Y)
{
   // top left
   //
   _X = mInfoBoxMaxWidth / 4;
   _Y = mMaxPos_y - (mInfoBoxMaxHeight / 2);
}

void CSymbology::GetMLSection(float &_X, float &_Y)
{
   // middle left
   //
   _X = mMaxNeg_x + mXseparation + 50;
   _Y = mHeight / 2;
}

void CSymbology::GetBLSection(float &_X, float &_Y)
{
   // bottom left
   //
   _X = mMaxNeg_x + mXseparation;
   _Y = mMaxPos_y - (mInfoBoxMaxHeight * 3) - mYseparation;
}

void CSymbology::GetTCSection(float &_X, float &_Y)
{
   // middle top ctr
   //
   _X = mMaxNeg_x + mInfoBoxMaxWidth + mXseparation;
   _Y = mMaxPos_y - mInfoBoxMaxHeight - mYseparation;
}

void CSymbology::GetMCSection(float &_X, float &_Y)
{
   // middle cetr
   //
   _X = mMaxNeg_x + mInfoBoxMaxWidth + mXseparation;
   _Y = mMaxPos_y - (mInfoBoxMaxHeight * 2) - mYseparation;
}

void CSymbology::GetBCSection(float &_X, float &_Y)
{
   // bottom ctr
   //
   _X = mInfoBoxMaxWidth + mInfoBoxMaxWidth / 4;
   _Y = (mInfoBoxMaxHeight / 4);
}

void CSymbology::GetRTSection(float &_X, float &_Y)
{
   // right top
   //
   _X = mMaxPos_x - mInfoBoxMaxWidth - mXseparation;
   _Y = mMaxPos_y - mInfoBoxMaxHeight - mYseparation;
}

void CSymbology::GetRMSection(float &_X, float &_Y)
{
   // right middle
   //
   _X = mMaxPos_x - mInfoBoxMaxWidth - mXseparation;
   _Y = mMaxPos_y - (mInfoBoxMaxHeight * 2) - mYseparation;
}

void CSymbology::GetRBSection(float &_X, float &_Y)
{
   // bottom right
   //
   _X = mMaxPos_x - mInfoBoxMaxWidth - mXseparation;
   _Y = mMaxPos_y - (mInfoBoxMaxHeight * 3) - mYseparation;
}

void CSymbology::DrawElevationPointer(void)
{
   vec3 start;
   vec3 end;
   float x1, x2, y1, y2, xoff, yoff;
   float tick_size_y1 = 45;
   const float tick_size_y2 = 83;
   // Figure out Center to next tick

   const int textHeight = 18;
   GetMLSection(xoff, yoff);

   mCompatibility = true;
   y1 = yoff;
   y2 = y1;

   GLCALL(glLineWidth(3.0));
   mLine.SetLineMode(SOLID);

   x1 = 0.395;
   x2 = 0.41;

   // Top linesegment
   start = vec3(xoff * (0.5f + x1) + tick_size_y1 + textHeight, yoff, 0.0f);
   end = vec3(xoff * (0.5f + x2) + tick_size_y2 + textHeight, yoff, 0.0f);
   DrawLine(start, end, mCompatibility);

   // bottom linesegment
   start = vec3(xoff * (0.5f + x1), yoff, 0.0f);
   end = vec3(xoff * (0.5f + x2) - (tick_size_y1), yoff, 0.0f);
   DrawLine(start, end, mCompatibility);
   GLCALL(glLineWidth(1.0));
}

void CSymbology::DrawElevationValues(void)
{
   vec3 start;
   vec3 end;
   float x1, x2, y1, y2, xoff, yoff;
   float tick_size_y1 = 45;
   const float tick_size_y2 = 63;
   // Figure out Center to next tick
   CText text(mProjection, 20);
   vec3 green(0.0f, 1.0f, 0.0f);

   // text width is same as height
   int textWidth = 20;

   int first_tick_l = ((int)FromHostUH->heading % 10);
   int first_hdg_l = (int)FromHostUH->heading - 90 - first_tick_l;
   if (first_hdg_l < 0)
      first_hdg_l += 360;

   y1 = mBsHeadingDegPerTick * -(90 + first_tick_l);
   y2 = y1;

   GetMLSection(xoff, yoff);
   mCompatibility = true;

   yoff += yoff * (0.5f + y1) + tick_size_y1 + textWidth + 3;

   GLCALL(glLineWidth(3.0));
   mLine.SetLineMode(SOLID);
   for (int i = 0; i < 18; ++i)
   {
      int cur_hdg = first_hdg_l + i * 10;
      if (cur_hdg > 360)
         cur_hdg -= 360;
      y1 += (10 * mBsHeadingDegPerTick);
      x1 = (cur_hdg % 20 == 0) ? 0.28f : 0.395;
      y2 = y1;
      x2 = 0.41;

      start = vec3(xoff * (0.5f + x1) + tick_size_y1, mHeight / 2 - (yoff * y1), 0.0f);
      end = vec3(xoff * (0.5f + x2) + tick_size_y2, mHeight / 2 - (yoff * y2), 0.0f);

      DrawLine(start, end, mCompatibility);

      std::string dir;
      if (cur_hdg % 20 == 0)
      {
         dir = std::to_string(cur_hdg);
         text.Print(dir, xoff * (0.5f + x2 + 0.02f), mHeight / 2 - (yoff * y1) + textWidth / 2, 1.0f, green);
      }
   }
   GLCALL(glLineWidth(1.0));
}

void CSymbology::DrawAzimuthPointer(void)
{
   vec3 start;
   vec3 end;
   float x1, x2, y1, y2, xoff, yoff;
   float tick_size_y1 = 35;
   const float tick_size_y2 = 73;
   // Figure out Center to next tick

   const int textHeight = 18;
   GetBCSection(xoff, yoff);

   xoff = mWidth / 2; // overide it, we want it at ctr

   mCompatibility = true;
   x1 = xoff;
   x2 = x1;

   GLCALL(glLineWidth(3.0));
   mLine.SetLineMode(SOLID);

   y1 = 0.395;
   y2 = 0.41;

   // Top linesegment
   start = vec3(xoff, yoff * (0.5f + y2) + tick_size_y1 + textHeight, 0.0f);
   end = vec3(xoff, yoff * (0.5f + y2) + tick_size_y2 + textHeight, 0.0f);
   DrawLine(start, end, mCompatibility);

   // bottom linesegment
   start = vec3(xoff, yoff * (0.5f + y2), 0.0f);
   end = vec3(xoff, yoff * (0.5f + y2) - (tick_size_y1), 0.0f);
   DrawLine(start, end, mCompatibility);
   GLCALL(glLineWidth(1.0));
}

void CSymbology::DrawAzimuthValues(void)
{
   vec3 start;
   vec3 end;
   float x1, x2, y1, y2, xoff, yoff;
   float tick_size_y1 = 35;
   const float tick_size_y2 = 53;
   // Figure out Center to next tick
   CText text(mProjection, 20);
   vec3 green(0.0f, 1.0f, 0.0f);

   int textWidth = 20;

   int first_tick_l = ((int)FromHostUH->heading % 10);
   int first_hdg_l = (int)FromHostUH->heading - 90 - first_tick_l;
   if (first_hdg_l < 0)
      first_hdg_l += 360;

   x1 = mBsHeadingDegPerTick * -(90 + first_tick_l);
   x2 = x1;

   GetBCSection(xoff, yoff);
   mCompatibility = true;

   xoff += mInfoBoxMaxWidth / 2;

   GLCALL(glLineWidth(3.0));
   mLine.SetLineMode(SOLID);
   for (int i = 0; i < 18; ++i)
   {
      int cur_hdg = first_hdg_l + i * 10;
      if (cur_hdg > 360)
         cur_hdg -= 360;
      x1 += (10 * mBsHeadingDegPerTick);
      y1 = (cur_hdg % 20 == 0) ? 0.28f : 0.395;
      x2 = x1;
      y2 = 0.41;

      start = vec3(mWidth / 2 - (xoff * x1), yoff * (0.5f + y1) + tick_size_y1, 0.0f);
      end = vec3(mWidth / 2 - (xoff * x2), yoff * (0.5f + y2) + tick_size_y2, 0.0f);

      DrawLine(start, end, mCompatibility);

      std::string dir;
      if (cur_hdg % 20 == 0)
      {
         dir = std::to_string(cur_hdg);
         text.Print(dir, mWidth / 2 - (xoff * x1) - textWidth, yoff * (0.5f + y2 + 0.02f), 1.0f, green);
      }
   }
   GLCALL(glLineWidth(1.0));
}

void CSymbology::DrawElevationPictograph(void)
{
   vec3 start;
   vec3 end;
   vec3 prev;

   float x1 = 0;
   float y1 = 0;
   bool first_pass = false;
   float size = 30;

   GetMLSection(x1, y1);
   x1 += size * 2;
   y1 += size * 4;
   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);
   double twopi = PI * 2;
   float theta = 0;
   float x = 0;
   float y = 0;
   float textWidth = 0.0f;
   float textHeight = 0.0f;

   CText text(mProjection, 28);

   mLine.SetLineMode(TRIANGLE);
   int elevation = (int)FromHostUH->mt_los_alt;

   start = vec3(x1, y1, 0.0);
   for (int index = 1; index <= 361; index++)
   {

      UpdatePoints(x, y, index, size);

      end = vec3(x + x1, y + y1, 0.0);

      if (index != elevation)
      {
         mLine.Here(end);

         if ((first_pass))
         {
            mLine.Here(start);
            mLine.Here(prev);
            mLine.Draw();
         }
         else
         {
            // every 3 verticies draw ttriangle
            if (!(index % 2))
            {
               // add the start
               mLine.Here(start);
               mLine.Draw();
               first_pass = true;
            }
         }
      }
      else
      {
         index += 30; // visible slice
         UpdatePoints(x, y, index, size);
         end = vec3(x + x1, y + y1, 0.0);
      }
      prev = end;
   }
   mLine.SetLineMode(SOLID);

   // drawing this on top
   DrawTopPictograph(x1, y1);

   vec3 green(0.0f, 1.0f, 0.0f);
   std::string temp = to_string(elevation);
   textWidth = text.GetWidth();
   textHeight = text.GetHeight();

   int spacing = 30;
   // text is at 270
   theta = 270 * PI / 180;
   y = (float)size * sinf(theta);
   x = (float)size * cosf(theta);

   // text.Print(temp, (x * mWidth / 2) - ( textWidth / 2), (y * mHeight / 2) - spacing - textHeight, 1.0f, green);
   text.Print(temp, (x1 - (textWidth / 2)), y1 - spacing - textHeight, 1.0f, green);
}

void CSymbology::DrawTopPictograph(float mx, float my)
{

   vec3 p1, start;
   vec3 p2, end;

   float lenW, lenH;

   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);

   mLine.SetLineMode(TRIANGLE);

   my += 30;
   lenH = 30;
   lenW = 38;

   // first main top box
   p1 = vec3(mx, my, 0.0f);
   mLine.Here(p1);

   p2 = vec3(mx + lenW, my, 0.0f);
   mLine.Here(p2);

   p1 = vec3(mx + lenW, my + lenH, 0.0f);
   mLine.Here(p1);
   mLine.Draw();

   p1 = vec3(mx, my, 0.0f);
   mLine.Here(p1);

   p2 = vec3(mx + lenW, my + lenH, 0.0f);
   mLine.Here(p2);

   p1 = vec3(mx - lenW, my + lenH, 0.0f);
   mLine.Here(p1);
   mLine.Draw();

   p1 = vec3(mx, my, 0.0f);
   mLine.Here(p1);

   p2 = vec3(mx - lenW, my, 0.0f);
   mLine.Here(p2);

   p1 = vec3(mx - lenW, my + lenH, 0.0f);
   mLine.Here(p1);
   mLine.Draw();

   // second upright top box
   p1 = vec3(mx - lenW / 2, my - lenH / 2 - lenH / 6, 0.0);
   mLine.Here(p1);

   p2 = vec3(mx + lenW / 2, my - lenH / 2 - lenH / 6, 0.0f);
   mLine.Here(p2);

   p1 = vec3(mx + lenW / 2, my, 0.0);
   mLine.Here(p1);
   mLine.Draw();

   mLine.Here(p1);

   p2 = vec3(mx - lenW / 2, my, 0.0f);
   mLine.Here(p2);

   p1 = vec3(mx - lenW / 2, my - lenH / 2 - lenH / 6, 0.0);
   mLine.Here(p1);
   mLine.Draw();

   // 1/2 circle black line
   GLCALL(glLineWidth(2.0));
   vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
   float x = 0.0f;
   float y = 0.0f;
   mLine.SetLineMode(SOLID);
   mLine.SetColor(black);
   start = vec3(mx - lenW / 2, my - lenH / 2 - lenH / 6, 0.0);

   for (int index = 180; index < 360; index++)
   {
      UpdatePoints(x, y, index, lenW / 2);
      end = vec3(x + mx, y + my - lenH / 2 - lenH / 6, 0.0);
      mLine.Here(start, end);
      start = end;
      mLine.Draw();
   }
   // the inverted arc
   start = vec3(mx - lenW / 2, my, 0.0);
   end = vec3(start.x, my - lenH / 2 - lenH / 6, 0.0);
   mLine.Here(start, end);
   mLine.Draw();

   start = vec3(mx + lenW / 2, my, 0.0);
   end = vec3(start.x, my - lenH / 2 - lenH / 6, 0.0);
   mLine.Here(start, end);
   mLine.Draw();

   // draw the arrwo
   mLine.SetLineMode(SOLID);
   unsigned short off = 5;
   start = vec3(mx + lenW, my + lenH / 2 - off, 0.0);
   end = vec3(mx - lenW, my + lenH / 2 - off, 0.0);
   mLine.Here(start, end);
   mLine.Draw();

   // divider
   unsigned short off2 = 5;
   start = vec3(mx + lenW / 2, my + lenH / 2 + off2, 0.0);
   end = vec3(mx - lenW / 2, my + lenH / 2 + off2, 0.0);
   mLine.Here(start, end);
   mLine.Draw();

   mLine.SetLineMode(TRIANGLE);

   float x_off = start.x;
   float y_off = start.y;

   x = 0;
   y = 0;

   // arrow head
   float len = 7;
   p1 = vec3(x_off, y_off, 0.0f);
   mLine.Here(p1);
   p1 = vec3(x_off - len, y_off - len, 0.0f);
   mLine.Here(p1);
   p2 = vec3(x_off - len, y_off, 0.0f);
   mLine.Here(p2);
   mLine.Draw();

   p1 = p2;
   mLine.Here(p1);
   p2 = vec3(x_off - len, y_off + len, 0.0f);
   mLine.Here(p2);
   p1 = vec3(x_off, y_off, 0.0f);
   mLine.Here(p1);
   mLine.Draw();

   vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
   mLine.SetLineMode(SOLID);
   mLine.SetColor(green);
}

/*
void CSymbology::DrawTopPictograph(float mx, float my)
{
   float x1, y1;

   // positions
   // colors
   // texture coords
   // convert to device
   x1 = 2 * mx / mWidth - 1;
   y1 = 2 * my / mHeight - 1;
   float iw = 2 * 45 / mWidth - 1;
   float ih = 2 * 48 / mWidth - 1;

   // top right
   // bottom right
   // bottom left
   // top left

   float vertices[] = {0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.f, 1.0f,
                  0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                  0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                  0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

   // create transformations
   glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
   transform = glm::translate(transform, glm::vec3(x1, y1 - ih / 24, 0.0f));
   transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.0f));

   mTopElImg.SetVertices(vertices);
   mTopElImg.SetTransform(transform);
   mTopElImg.DrawImage();
}
*/

void CSymbology::DrawAzimuthPictograph(void)
{
   vec3 start;
   vec3 end;
   vec3 prev;

   float x1 = 0;
   float y1 = 0;
   bool first_pass = false;
   float size = 30;

   GetMLSection(x1, y1);
   x1 += size * 2;
   mCompatibility = true;
   mLine.SetCompatibility(mCompatibility);
   double twopi = PI * 2;
   float theta = 0;
   float x = 0;
   float y = 0;
   float textWidth = 0.0f;
   float textHeight = 0.0f;

   CText text(mProjection, 28);

   mLine.SetLineMode(TRIANGLE);
   int azimuth = (int)FromHostAH->azimuth;

   start = vec3(x1, y1, 0.0);
   for (int index = 1; index <= 361; index++)
   {

      UpdatePoints(x, y, index, size);

      end = vec3(x + x1, y + y1, 0.0);

      if (index != azimuth)
      {
         mLine.Here(end);

         if ((first_pass))
         {
            mLine.Here(start);
            mLine.Here(prev);
            mLine.Draw();
         }
         else
         {
            // every 3 verticies draw ttriangle
            if (!(index % 2))
            {
               // add the start
               mLine.Here(start);
               mLine.Draw();
               first_pass = true;
            }
         }
      }
      else
      {
         index += 30; // visible slice
         UpdatePoints(x, y, index, size);
         end = vec3(x + x1, y + y1, 0.0);
      }
      prev = end;
   }

   mLine.SetLineMode(SOLID);

   vec3 green(0.0f, 1.0f, 0.0f);
   std::string temp = to_string(azimuth);
   textWidth = text.GetWidth();
   textHeight = text.GetHeight();

   int spacing = 30;
   // text is at 270
   theta = 270 * PI / 180;
   y = (float)size * sinf(theta);
   x = (float)size * cosf(theta);

   // text.Print(temp, (x * mWidth / 2) - ( textWidth / 2), (y * mHeight / 2) - spacing - textHeight, 1.0f, green);
   text.Print(temp, (x1 - (textWidth / 2)), y1 - spacing - textHeight, 1.0f, green);
}

void CSymbology::UpdatePoints(float &x, float &y, int idx, int size)
{
   // get the current angle
   //  do not show triangle if it matches the current azimuth
   float theta = idx * PI / 180;
   // calculate the x component
   x = (float)size * cosf(theta);

   // calculate the y component
   y = (float)size * sinf(theta);
}

void CSymbology::DrawBSCompassTape(void)
{
   vec3 start;
   vec3 end;
   float x1, x2, y1, y2, xoff, yoff;
   // Figure out Center to next tick
   CText text(mProjection, 28);
   vec3 green(0.0f, 1.0f, 0.0f);

   int first_tick_l = ((int)FromHostUH->heading % 10);
   int first_hdg_l = (int)FromHostUH->heading - 90 - first_tick_l;
   if (first_hdg_l < 0)
      first_hdg_l += 360;

   printf("%d %d\n", first_tick_l, first_hdg_l);

   x1 = mBsHeadingDegPerTick * -(90 + first_tick_l);
   x2 = x1;

   GetBCSection(xoff, yoff);
   mCompatibility = true;

   mLine.SetLineMode(SOLID);
   for (int i = 0; i < 18; ++i)
   {
      int cur_hdg = first_hdg_l + i * 10;
      if (cur_hdg > 360)
         cur_hdg -= 360;
      x1 += (10 * mBsHeadingDegPerTick);
      x2 = x1;
      y1 = (cur_hdg % 30 == 0) ? 0.28 : 0.395;
      y2 = 0.41;

      // glColor4f(0.0, 1.0, 0.0, 1.0);

      start = vec3(xoff * (0.5f + x1), yoff * (0.5f + y1) + 32, 0.0f);
      end = vec3(xoff * (0.5f + x2), yoff * (0.5f + y2) + 50, 0.0f);

      DrawLine(start, end, mCompatibility);

      GLCALL(glLineWidth(1.0));

      // char dir[4];
      std::string dir;
      if (cur_hdg % 30 == 0)
      {
         switch (cur_hdg)
         {
         case 0:
         case 360:
            dir = "N";
            break;
         case 90:
            dir = "E";
            break;
         case 180:
            dir = "S";
            break;
         case 270:
            dir = "W";
            break;
         default:
            dir = std::to_string(cur_hdg);
            break;
         }
         text.Print(dir, xoff * (0.5f + x1), yoff * (0.5f + y2 + 0.02f), 1.0f, green);
      }
   }

   /*
      glRasterPos2d(0.5,0.5+ 0.35);
      char hdg[5];
      sprintf(hdg,"%f",test_hdg);
      mFontLib.Print(mFontId, hdg);
   */

   // Draw heading lables
   // Draw LOS text
   std::cout << "Heading = " << FromHostUH->heading << std::endl;
}
