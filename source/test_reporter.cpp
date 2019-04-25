/*!
  \file test_reporter.cpp
  \brief Implementation of UnitTest::Reporter class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test_reporter.h>
#include <utpp/test.h>

namespace UnitTest {

Reporter* CurrentReporter;

Reporter::Reporter ()
  : suite_test_count (0)
  , suite_failed_count (0)
  , suite_failures_count (0)
  , suite_time_msec (0)
  , total_test_count (0)
  , total_failed_count (0)
  , total_failures_count (0)
  , total_time_msec (0)
  , suites_count (0)
  , trace (false)
{
}

/*!
  Records the beginning of a new test suite
*/
void Reporter::SuiteStart (const TestSuite&)
{
  suites_count++;
  suite_test_count = suite_failed_count = suite_failures_count = 0;
}

void Reporter::TestStart (const Test&)
{
  suite_test_count++;
  total_test_count++;
}

void Reporter::ReportFailure (const Failure &f)
{
}

void Reporter::TestFinish (const Test& t)
{
  int f = t.failure_count ();
  if (f)
  {
    suite_failed_count++;
    suite_failures_count += f;
    total_failed_count++;
    total_failures_count += f;
  }
  int ms = t.test_time_ms ();
  suite_time_msec += ms;
  total_time_msec += ms;
}

/*!
  \return number of failures in this suite
*/
int Reporter::SuiteFinish (const TestSuite&)
{
  return suite_failures_count;
}

}
