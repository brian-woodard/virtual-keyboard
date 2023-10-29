
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#define __TESTING_SYMB__

using namespace std;

class SimTimer
{
   // testing timer
   //
   float mTime;
   float mPreviousTime;
   float mNow;
   float mDelta;

public:
   bool mTimerStop;
   SimTimer();
   void StartTimer(float _time);

private:
   void StartTime(void);

public:
   bool TimeExpired(void);
   void StopTime(void);
};
