/*!
  \file test.cpp
  \brief Implementation of UnitTest::Test class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test.h>
#include <utpp/test_suite.h>
#include <utpp/test_reporter.h>

#include <sstream>

namespace UnitTest {

/// Constructor
Test::Test (const std::string& test_name)
  : name (test_name)
  , failures (0)
  , time (0)
{
}

Test::~Test ()
{
}

/*!
  If test is not exempted of global time constraint it starts a timer and calls
  RunImpl() to execute test code.

  When RunImpl() returns, it records the elapsed time.
*/
void Test::run ()
{
  Timer test_timer;
  test_timer.Start ();

  RunImpl ();

  time = test_timer.GetTimeInMs ();
}

/*!
  Increment failures count for this test
*/
void Test::failure ()
{
  failures++;
}

/*!
  The function called by the various CHECK_... macros to record a failure.
  \param filename Name of file where the failure has occurred
  \param line     Line number where the failure has occurred
  \param message  Failure description

  It calls the TestReporter::ReportFailure function of the current reporter
  object.
*/
void ReportFailure (const std::string& filename, int line, const std::string& message)
{
  assert (CurrentReporter);

  if (CurrentTest)
    CurrentTest->failure ();
  CurrentReporter->ReportFailure ({ filename, message, line });
}


}
