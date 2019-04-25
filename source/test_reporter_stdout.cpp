/*!
  \file test_reporter_stdout.cpp
  \brief Implementation of UnitTest::ReporterStdout class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test_reporter_stdout.h>
#include <utpp/test.h>
#include <iostream>
#include <iomanip>
using namespace std;


namespace UnitTest {

/// Return the default reporter object. 
Reporter& GetDefaultReporter ()
{
  static ReporterStdout the_default_reporter;
  return the_default_reporter;
}

/// If tracing is enabled, show a suite start message 
void ReporterStdout::SuiteStart (const TestSuite& suite)
{
  Reporter::SuiteStart (suite);
  if (!trace)
    return;
  cout << "Suite starting: " << suite.name << endl;
}

/// If tracing is enabled, show a test start message 
void ReporterStdout::TestStart (const Test & test)
{
  Reporter::TestStart (test);
  if (!trace)
    return;
  cout << "Test starting: " << test.test_name () << endl;
}

/// If tracing is enabled, show a test finish message 
void ReporterStdout::TestFinish (const Test & test)
{
  if (trace)
    cout << "Test finished: " << test.test_name () << endl;
  Reporter::TestFinish (test);
}

/// If tracing is enabled, show a suite finish message 
int ReporterStdout::SuiteFinish (const TestSuite & suite)
{
  if (trace)
    cout << "Suite finishing: " << suite.name << endl;
  return Reporter::SuiteFinish (suite);
}


/*!
  Output to stdout a failure message. If a test is in progress (the normal case)
  the message includes the name of the test and suite.

  \param failure - the failure information (filename, line number and message)
*/
void ReporterStdout::ReportFailure (const Failure& failure)
{
  cout << "Failure in ";
  if (CurrentTest)
  {
    if (CurrentSuite != DEFAULT_SUITE)
      cout << " suite " << CurrentSuite;
    cout << " test " << CurrentTest->test_name ();
  }
  cout << endl << failure.filename << "(" << failure.line_number << "):"
    << failure.message << endl;
  Reporter::ReportFailure (failure);
}

/*!
  Prints a test run summary including number of tests, number of failures,
  running time, etc.
*/
int ReporterStdout::Summary ()
{
  if (total_failed_count > 0)
  {
    cout << "FAILURE: " << total_failed_count << " out of "
      << total_test_count << " tests failed (" << total_failures_count
      << " failures)." << endl;
  }
  else
  {
    cout << "Success: " << total_test_count << " tests passed." << endl;
  }
  cout.setf (ios::fixed);
  cout << "Run time: " << setprecision (2) << total_time_msec / 1000. << endl;
  return Reporter::Summary ();
}

}
