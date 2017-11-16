/*!
  \file time_helpers.cpp
  \brief Implementation of UnitTest::Timer class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/time_helpers.h>
#include <windows.h>

namespace UnitTest {

__int64 Timer::frequency = 0;

Timer::Timer ()
  : startTime (0)
{
  if (!frequency)
  {
    ::QueryPerformanceFrequency (reinterpret_cast<LARGE_INTEGER*>(&frequency));
  }
}

/// Record starting time
void Timer::Start ()
{
  startTime = GetTime ();
}

/// Return elapsed time in milliseconds since the starting time
int Timer::GetTimeInMs () const
{
  __int64 elapsedTime = GetTime () - startTime;
  double seconds = double (elapsedTime) / double (frequency);
  return int (seconds * 1000.0);
}

__int64 Timer::GetTime () const
{
  LARGE_INTEGER curTime;
  ::QueryPerformanceCounter (&curTime);
  return curTime.QuadPart;
}

/// Pause current thread for the specified time
void SleepMs (int const ms)
{
  ::Sleep (ms);
}

}
