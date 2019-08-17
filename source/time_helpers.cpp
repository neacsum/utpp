/*!
  \file time_helpers.cpp
  \brief Implementation of UnitTest::Timer class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/time_helpers.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

namespace UnitTest {

long long Timer::frequency = 0;

Timer::Timer ()
  : startTime (0)
{
#ifdef _WIN32
  if (!frequency)
    ::QueryPerformanceFrequency (reinterpret_cast<LARGE_INTEGER*>(&frequency));
#else
  frequency = 1000000;
#endif
}

/// Record starting time
void Timer::Start ()
{
  startTime = GetTime ();
}

/// Return elapsed time in milliseconds since the starting time
int Timer::GetTimeInMs () const
{
  long long elapsedTime = GetTime () - startTime;
  double seconds = double (elapsedTime) / double (frequency);
  return int (seconds * 1000.0);
}

/// Return elapsed time in microseconds since the starting time
long long Timer::GetTimeInUs () const
{
  long long elapsedTime = GetTime () - startTime;
  double seconds = double (elapsedTime) / double (frequency);
  return long long (seconds * 1000000.0);
}

long long Timer::GetTime () const
{
#ifdef _WIN32
  LARGE_INTEGER curTime;
  ::QueryPerformanceCounter (&curTime);
  return curTime.QuadPart;
#else
  struct timeval currentTime;
  gettimeofday (&currentTime, 0);
  return currentTime.tv_sec * frequency + currentTime.tv_usec;
#endif
}

/// Pause current thread for the specified time
void SleepMs (int const ms)
{
#ifdef _WIN32
  ::Sleep (ms);
#else
  usleep (static_cast<useconds_t>(ms * 1000));
#endif
}

}
