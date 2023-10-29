#include "SimTimer.h"

SimTimer::SimTimer()
{
   mNow = 0.f;
   mPreviousTime = 0.f;
   mDelta = 0.f;
   mTimerStop = true;
   mTime = 0.0f;
};

void SimTimer::StartTimer(float _time)
{
   mTime = _time;
   StartTime();
}

void SimTimer::StartTime(void)
{
   mTimerStop = false;
   mNow = (float)glfwGetTime();
   mPreviousTime = mNow;
   mDelta = mNow - mPreviousTime;
}

bool SimTimer::TimeExpired(void)
{
   bool time_expired = true;

   if (!mTimerStop)
   {
      mNow = (float)glfwGetTime();
      mDelta = mNow - mPreviousTime;
      mPreviousTime = mNow;
      mTime -= mDelta;

      if (mTime > 0.f)
         time_expired = false;
      else
      {
         time_expired = true;
         StopTime();
      }
   }

   return time_expired;
}

void SimTimer::StopTime(void)
{
   mNow = 0.f;
   mPreviousTime = 0.f;
   mDelta = 0.f;
   mTimerStop = true;
}
