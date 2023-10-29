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
//  Class:      C++ Header
//  Filename:   Stopwatch.h
//  Author:     Brian Woodard
//  Purpose:    This module performs the following tasks:
//
//  Provides a basic timing class for for scoped or manual benchmarking purposes.
//
//-----------------------------------------------------------------------------

#pragma once

#include <chrono>

class CStopwatch
{
public:

   typedef std::chrono::duration<double> double_secs;

   //! \fn CStopwatch()
   //! \details A manual timer constructor. User must call Start() and then
   //!          GetTime() manually to get the duration in seconds.
   CStopwatch() : mDuration(nullptr) {}

   //! \fn CStopwatch(double* Duration)
   //! \details A scoped timer constructor, accepts a pointer for where to
   //!          store the duration in seconds upon destruction.
   CStopwatch(double* Duration);

   //! \fn ~CStopwatch()
   //! \details A scoped timer destructor, calculates the duration in seconds
   //!          and sets that to the mDuration passed in on construction.
   ~CStopwatch();

   //! \fn double GetTime()
   //! \details Returns the time, in seconds, since the stopwatch was started
   double GetTime();

   //! \fn void Start()
   //! \details Starts the stopwatch timer
   void Start();

private:

   std::chrono::steady_clock::time_point mStart;
   std::chrono::steady_clock::time_point mEnd;
   double*                               mDuration;

};
