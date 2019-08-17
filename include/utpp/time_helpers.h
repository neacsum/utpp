#pragma once
/*!
  \file time_helpers.h
  \brief Definition of UnitTest::Timer class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

namespace UnitTest {


/// An object that can be interrogated to get elapsed time
class Timer
{
public:
  Timer ();
  void Start ();
  int GetTimeInMs () const;
  long long GetTimeInUs () const;

private:
  long long GetTime () const;
  long long startTime;
  static long long frequency;
};


void SleepMs (int ms);

}
