/*!
  \file deferred_test_reporter.cpp
  \brief Implementation of UnitTest::ReporterDeferred class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/deferred_test_reporter.h>
#include <utpp/test.h>

using namespace UnitTest;

/*!
  Called at the beginning of a new test.
  \param  test    %Test that is about to start

  Creates a new TestResult object and adds it to the results container
*/
void ReporterDeferred::TestStart (const Test& test)
{
  Reporter::TestStart (test);
  results.push_back (TestResult (CurrentSuite, test.test_name()));
}

/*!
  Add a new failure to current test
  \param failure  The failure record
*/
void ReporterDeferred::ReportFailure (const Failure& failure)
{
  assert (!results.empty ());

  Reporter::ReportFailure (failure);
  results.back ().failures.push_back (failure);
}

/*!
  Store test runtime when the test finishes
  \param  test    %Test that is about to end
*/
void ReporterDeferred::TestFinish (const Test& test)
{
  Reporter::TestFinish (test);
  results.back ().test_time_ms = test.test_time_ms();
}

