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
//  � Veraxx Engineering Corporation, 2023.  All rights reserved.
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
//! Title:      Stats
//! Class:      CPP Source
//! Filename:   Stats.cpp
//! Author:     Brian Woodard
//! Purpose:    Maintains and draws timing stats and FPS.
//
//-----------------------------------------------------------------------------

#include <string.h>
#include "Stats.h"
#include "IresTypes.h"
#include "PrintData.h"

CStats::CStats(int Width, int Height)
   : mFpsHistory{0.0},
     mMouseOverTimerMs(0.0),
     mFpsCount(0),
     mWidth(Width),
     mHeight(Height),
     mProjection(glm::ortho(0.0f, (float)Width, 0.0f, (float)Height)),
     mInvProjection(glm::ortho(0.0f, (float)Width, (float)Height, 0.0f)),
     mBackground(),
     mFpsBackground(),
     mLine60Hz(),
     mTimerLines{},
     mFpsText(mProjection, 20),
     mStatsSize(Width / 4),
     mStatsCount(0),
     mStats(nullptr),
     mFrameStats{},
     mTimersEnabled{false},
     mFpsEnabled(false),
     mStatsEnabled(false),
     mStatsPaused(false),
     mLineGrabbed(false)
{
   mBackground.SetLineMode(TRIANGLE);
   mBackground.SetColor(glm::vec4(0.25f, 0.25f, 0.25f, 0.75f));
   mBackground.setMVP(mInvProjection);
   mBackground.SetPosition(glm::vec3(0.0f,          (float)mHeight,        0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth, (float)mHeight,        0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth, (float)mHeight / 2.0f, 0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth, (float)mHeight / 2.0f, 0.0f));
   mBackground.SetPosition(glm::vec3(0.0f,          (float)mHeight / 2.0f, 0.0f));
   mBackground.SetPosition(glm::vec3(0.0f,          (float)mHeight,        0.0f));
   mBackground.CreateVAO();

   mFpsBackground.SetLineMode(TRIANGLE);
   mFpsBackground.SetColor(glm::vec4(0.25f, 0.25f, 0.25f, 0.75f));
   mFpsBackground.setMVP(mInvProjection);
   mFpsBackground.SetPosition(glm::vec3(  8.0f,  7.0f, 0.0f));
   mFpsBackground.SetPosition(glm::vec3(115.0f,  7.0f, 0.0f));
   mFpsBackground.SetPosition(glm::vec3(115.0f, 27.0f, 0.0f));
   mFpsBackground.SetPosition(glm::vec3(115.0f, 27.0f, 0.0f));
   mFpsBackground.SetPosition(glm::vec3(  8.0f,  7.0f, 0.0f));
   mFpsBackground.SetPosition(glm::vec3(  8.0f, 27.0f, 0.0f));
   mFpsBackground.CreateVAO();

   mLine60Hz.SetLineMode(DASH);
   mLine60Hz.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
   mLine60Hz.setMVP(mInvProjection);
   mLine60Hz.SetLineWidth(1.0);
   mLine60Hz.SetPosition(glm::vec3(         0.0f, (float)mHeight / 2.0f, 0.0f));
   mLine60Hz.SetPosition(glm::vec3((float)mWidth, (float)mHeight / 2.0f, 0.0f));
   mLine60Hz.CreateVAO();

   // some objects for drawing stats
   for (int i = 0; i < TIMER_COUNT; i++)
   {
      mTimerLines[i].SetLineMode(DASH);
      mTimerLines[i].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
      mTimerLines[i].setMVP(mProjection);
      mTimerLines[i].SetLineWidth(1.0f);

      mTimersEnabled[i] = true;
   }

   mTimerLines[TIMER_TOTAL].SetColor(glm::vec3(1.0f, 0.5f, 0.0f)); // orange
   mTimerLines[TIMER_PROCESS_PDU].SetColor(glm::vec3(1.0f, 1.0f, 0.0f)); // yellow
   mTimerLines[TIMER_SYMB_DRAW].SetColor(glm::vec3(0.0f, 1.0f, 1.0f)); // cyan
   mTimerLines[TIMER_MENU_DRAW].SetColor(glm::vec3(1.0f, 0.0f, 1.0f)); //magenta

   mStats = new TStats[mStatsSize+1];

   memset(mStats, 0, sizeof(TStats) * (mStatsSize+1));
   memset(mFpsHistory, 0, sizeof(mFpsHistory));
}

CStats::~CStats()
{
   if (mStats)
      delete [] mStats;
}

void CStats::Draw(double FrameTime)
{
   double fps = 0.0;
   double fps_avg = 0.0;

   // do some FPS calculations
   if (FrameTime > 0.0)
      fps = 1.0 / FrameTime;

   mFpsHistory[mFpsCount++] = fps;
   if (mFpsCount >= FPS_HISTORY) mFpsCount = 0;
   for (int i = 0; i < FPS_HISTORY; i++)
      fps_avg += mFpsHistory[i];
   fps_avg *= INV_FPS_HISTORY;

   if (!mStatsPaused)
   {
      // save stats
      mStats[mStatsCount++] = mFrameStats;
      if (mStatsCount > mStatsSize) mStatsCount = 0;
   }

   if (mFpsEnabled || mStatsEnabled)
   {
      char fps_str[100];
      sprintf(fps_str, "FPS: %.1f", fps_avg);
      mFpsBackground.Draw(false);
      mFpsText.SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
      mFpsText.Print(fps_str, 10.0f, (float)mHeight - 25.0f);
   }

   if (mStatsEnabled)
   {
      // draw some stats
      mBackground.Draw(false);
      mLine60Hz.Draw(false);

      float line_y = (float)mHeight;
      
      if (mLine60Hz.GetVertex(0))
         line_y -= mLine60Hz.GetVertex(0)->y;

      mFpsText.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
      mFpsText.Print("60 Hz", 10.0f, line_y - 20.0f);

      line_y *= 2.0;

      // update timing lines
      // TODO: Only update one vertex per frame, need some utility functions in CLine for this to happen
      for (int i = 0; i <= mStatsSize; i++)
      {
         glm::vec3 point = glm::vec3(0.0f);
         point.x = i * 4.0f;
         point.y = (float)mStats[i].Timers[TIMER_TOTAL] * 30.0f * line_y;
         mTimerLines[TIMER_TOTAL].SetPosition(point);
         point.y = (float)mStats[i].Timers[TIMER_PROCESS_PDU] * 30.0f * line_y;
         mTimerLines[TIMER_PROCESS_PDU].SetPosition(point);
         point.y = (float)mStats[i].Timers[TIMER_MENU_DRAW] * 30.0f * line_y;
         mTimerLines[TIMER_MENU_DRAW].SetPosition(point);
         point.y = (float)mStats[i].Timers[TIMER_SYMB_DRAW] * 30.0f * line_y;
         mTimerLines[TIMER_SYMB_DRAW].SetPosition(point);
      }

      if (mTimersEnabled[TIMER_TOTAL])
         mTimerLines[TIMER_TOTAL].Draw();

      if (mTimersEnabled[TIMER_PROCESS_PDU])
         mTimerLines[TIMER_PROCESS_PDU].Draw();

      if (mTimersEnabled[TIMER_SYMB_DRAW])
         mTimerLines[TIMER_SYMB_DRAW].Draw();

      if (mTimersEnabled[TIMER_MENU_DRAW])
         mTimerLines[TIMER_MENU_DRAW].Draw();
   }
}

int CStats::MouseOverTimer(float X, float Y)
{
   // check for mouse over on timing lines
   int result = -1;
   int x_idx = ((int)X / 4) - 2;

   float line_y = (float)mHeight;

   if (mLine60Hz.GetVertex(0))
      line_y -= mLine60Hz.GetVertex(0)->y;

   line_y *= 2.0;

   for (int i = 0; i < 3; i++, x_idx++)
   {
      float y = 0.0;

      if (mTimersEnabled[TIMER_TOTAL])
      {
         y = (float)mStats[x_idx].Timers[TIMER_TOTAL] * 30.0f * line_y;
         if (fabs((float)mHeight - Y - y) < 20.0)
         {
            result = TIMER_TOTAL;
            mMouseOverTimerMs = mStats[x_idx].Timers[TIMER_TOTAL] * 1000.0;
            break;
         }
      }

      if (mTimersEnabled[TIMER_PROCESS_PDU])
      {
         y = (float)mStats[x_idx].Timers[TIMER_PROCESS_PDU] * 30.0f * line_y;
         if (fabs((float)mHeight - Y - y) < 20.0)
         {
            result = TIMER_PROCESS_PDU;
            mMouseOverTimerMs = mStats[x_idx].Timers[TIMER_PROCESS_PDU] * 1000.0;
            break;
         }
      }

      if (mTimersEnabled[TIMER_SYMB_DRAW])
      {
         y = (float)mStats[x_idx].Timers[TIMER_SYMB_DRAW] * 30.0f * line_y;
         if (fabs((float)mHeight - Y - y) < 20.0)
         {
            result = TIMER_SYMB_DRAW;
            mMouseOverTimerMs = mStats[x_idx].Timers[TIMER_SYMB_DRAW] * 1000.0;
            break;
         }
      }

      if (mTimersEnabled[TIMER_MENU_DRAW])
      {
         y = (float)mStats[x_idx].Timers[TIMER_MENU_DRAW] * 30.0f * line_y;
         if (fabs((float)mHeight - Y - y) < 20.0)
         {
            result = TIMER_MENU_DRAW;
            mMouseOverTimerMs = mStats[x_idx].Timers[TIMER_MENU_DRAW] * 1000.0;
            break;
         }
      }
   }

   return result;
}

void CStats::UpdateLine(float X, float Y)
{
   // only using the mouse y value to move the red line around
   mLine60Hz.ClearVertices();
   mLine60Hz.SetPosition(glm::vec3(         0.0f, Y, 0.0f));
   mLine60Hz.SetPosition(glm::vec3((float)mWidth, Y, 0.0f));
   mLine60Hz.CreateVAO();

   mBackground.ClearVertices();
   mBackground.SetPosition(glm::vec3(         0.0f, (float)mHeight, 0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth, (float)mHeight, 0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth,              Y, 0.0f));
   mBackground.SetPosition(glm::vec3((float)mWidth,              Y, 0.0f));
   mBackground.SetPosition(glm::vec3(         0.0f,              Y, 0.0f));
   mBackground.SetPosition(glm::vec3(         0.0f, (float)mHeight, 0.0f));
   mBackground.CreateVAO();
}
