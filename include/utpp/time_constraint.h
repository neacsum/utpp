#pragma once
/*!
  \file time_constraint.h
  \brief Definition of UnitTest::TimeConstraint class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <string>
#include "time_helpers.h"

namespace UnitTest {

///Defines maximum run time of a test
class TimeConstraint
{
public:
  TimeConstraint (int ms, const char* file, int line);
  ~TimeConstraint ();

private:
#ifdef UTPP_CPP11
  void operator=(TimeConstraint const&) = delete;
  TimeConstraint (TimeConstraint const&) = delete;
#endif

  Timer timer;
  std::string filename;
  int line_number;
  int const max_ms;
};

/// Defines a local (per scope) time constraint
#define UTPP_TIME_CONSTRAINT(ms) \
  UnitTest::TimeConstraint unitTest__timeConstraint__(ms, __FILE__, __LINE__)

/// Flags a test as not subject to the global time constraint
#define UTPP_TIME_CONSTRAINT_EXEMPT() no_time_constraint ()

}
