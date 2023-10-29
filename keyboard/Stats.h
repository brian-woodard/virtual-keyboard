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
//! Class:      CPP Header
//! Filename:   Stats.h
//! Author:     Brian Woodard
//! Purpose:    Maintains and draws timing stats and FPS.
//
//-----------------------------------------------------------------------------

#pragma once

#include "Line.h"
#include "CText.h"

const int FPS_HISTORY = 100;
const double INV_FPS_HISTORY = 1.0 / (double)FPS_HISTORY;

class CStats
{
public:

   enum Timers
   {
      TIMER_TOTAL,
      TIMER_PROCESS_PDU,
      TIMER_MENU_DRAW,
      TIMER_SYMB_DRAW,
      TIMER_WAIT_FOR_NEXT_FRAME,

      TIMER_COUNT
   };

   struct TStats
   {
      double Timers[TIMER_COUNT];
   };

   CStats(int Width, int Height);
   virtual ~CStats();

   void Draw(double FrameTime);

   float GetLineY()
   {
      if (mLine60Hz.GetVertex(0))
         return mLine60Hz.GetVertex(0)->y;
      else
         return 0.0f;
   }

   void GrabLine(bool Grab) { mLineGrabbed = Grab; }
   bool IsGrabbed() const { return mLineGrabbed; }

   int MouseOverTimer(float X, float Y);
   double GetMouseOverTime() const { return mMouseOverTimerMs; }

   bool StatsEnabled() const { return mStatsEnabled; }

   double& Timer(int Timer)
   {
      if (Timer < 0 || Timer >= TIMER_COUNT)
         return mFrameStats.Timers[TIMER_TOTAL];
      else
         return mFrameStats.Timers[Timer];
   }

   void ToggleFps() { mFpsEnabled = !mFpsEnabled;  }
   void ToggleStats() { mStatsEnabled = !mStatsEnabled; }
   void TogglePause() { mStatsPaused = !mStatsPaused;  }

   void ToggleTimer(int Timer)
   {
      if (Timer < 0 || Timer >= TIMER_COUNT)
         mTimersEnabled[TIMER_TOTAL] = !mTimersEnabled[TIMER_TOTAL];
      else
         mTimersEnabled[Timer] = !mTimersEnabled[Timer];
   }

   void UpdateLine(float X, float Y);

private:

   double    mFpsHistory[FPS_HISTORY];
   double    mMouseOverTimerMs;
   int       mFpsCount;
   int       mWidth;
   int       mHeight;
   glm::mat4 mProjection;
   glm::mat4 mInvProjection;
   CLine     mBackground;
   CLine     mFpsBackground;
   CLine     mLine60Hz;
   CLine     mTimerLines[TIMER_COUNT];
   CText     mFpsText;
   int       mStatsSize;
   int       mStatsCount;
   TStats*   mStats;
   TStats    mFrameStats;
   bool      mTimersEnabled[TIMER_COUNT];
   bool      mFpsEnabled;
   bool      mStatsEnabled;
   bool      mStatsPaused;
   bool      mLineGrabbed;

};


