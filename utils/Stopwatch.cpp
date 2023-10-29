//-----------------------------------------------------------------------------
//                            UNCLASSIFIED
//-----------------------------------------------------------------------------
//                    DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
//  This software and the accompanying documentation are provided to the U.S.
//  Government with unlimited rights as provided in DFARS § 252.227-7014.  The
//  contractor, Veraxx Engineering Corporation, retains ownership, the
//  copyrights, and all other rights.
//
//  © Veraxx Engineering Corporation 2021.  All rights reserved.
//
// DEVELOPED BY:
//  Veraxx Engineering Corporation
//  14130 Sullyfield Circle, Suite B
//  Chantilly, VA 20151
//  (703)880-9000 (Voice)
//  (703)880-9005 (Fax)
//-----------------------------------------------------------------------------
//  Title:      Stopwatch Class
//  Class:      C++ Source
//  Filename:   Stopwatch.cpp
//  Author:     Brian Woodard
//  Purpose:    This module performs the following tasks:
//
//  Provides a basic scoped timing class for benchmarking purposes.
//
//-----------------------------------------------------------------------------

#include "Stopwatch.h"

CStopwatch::CStopwatch(double* Duration)
   : mStart (std::chrono::steady_clock::now()),
     mDuration (Duration)
{
}

CStopwatch::~CStopwatch()
{
   if (mDuration)
   {
      mEnd = std::chrono::steady_clock::now();

      auto duration = std::chrono::duration_cast<double_secs>(mEnd - mStart);
      *mDuration = duration.count();
   }
}

double CStopwatch::GetTime()
{
   mEnd = std::chrono::steady_clock::now();

   return std::chrono::duration_cast<double_secs>(mEnd - mStart).count();
}

void CStopwatch::Start()
{
   mStart = std::chrono::steady_clock::now();
}
