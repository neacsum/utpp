/*!
  \file time_constraint.cpp
  \brief Implementation of UnitTest::TimeConstraint class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/time_constraint.h>
#include <utpp/test.h>

#include <sstream>
#include <cassert>

namespace UnitTest {

/*!
  Initializes a TimeConstraint object.
  \param ms       Maximum allowed duration in milliseconds
  \param file     Filename associated with this constraint
  \param line     Line number associated with this constraint

  The object contains a timer that is started now. It also keeps track of the
  filename and line number where it has been created.
*/
TimeConstraint::TimeConstraint (int ms, const char* file, int line)
  : filename (file)
  , line_number (line)
  , max_ms (ms)
{
  timer.Start ();
}

/*!
  If the timer is greater than allowed value it records a time constraint failure
  for the current test.
*/
TimeConstraint::~TimeConstraint ()
{
  int t = timer.GetTimeInMs ();
  if (t > max_ms)
  {
    std::stringstream stream;
    stream << "Time constraint failed. Expected to run test under " << max_ms <<
      "ms but took " << t << "ms.";

    ReportFailure (filename, line_number, stream.str ());
  }
}

}
