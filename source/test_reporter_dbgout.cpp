/*!
  \file test_reporter_dbgout.cpp
  \brief Implementation of UnitTest::ReporterDbgout class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test_reporter_dbgout.h>
#include <utpp/test.h>
#include <sstream>
#include <iomanip>
#include <Windows.h>

using namespace std;


namespace UnitTest {

static wstring widen (const std::string& s)
{
  int nsz = (int)s.size();
  int wsz = MultiByteToWideChar (CP_UTF8, 0, s.c_str(), nsz, 0, 0);
  wstring out (wsz, 0);
  if (wsz)
    MultiByteToWideChar (CP_UTF8, 0, s.c_str(), nsz, &out[0], wsz);
  return out;
}

/// If tracing is enabled, show a suite start message 
void ReporterDbgout::SuiteStart (const TestSuite & suite)
{
  Reporter::SuiteStart (suite);
  if (!trace)
    return;

  stringstream ss;
  ss << "Suite starting: " << suite.name << endl;
  OutputDebugString (widen (ss.str ()).c_str ());
}

/// If tracing is enabled, show a test start message 
void ReporterDbgout::TestStart (const Test & test)
{
  Reporter::TestStart (test);
  if (!trace)
    return;
  stringstream ss;
  ss << "Test starting: " << test.test_name () << endl;
  OutputDebugString (widen (ss.str ()).c_str ());
}

/// If tracing is enabled, show a test finish message 
void ReporterDbgout::TestFinish (const Test & test)
{
  if (trace)
  {
    stringstream ss;
    ss << "Test finished: " << test.test_name () << endl;
    OutputDebugString (widen (ss.str ()).c_str ());
  }
  Reporter::TestFinish (test);
}

/// If tracing is enabled, show a suite finish message 
int ReporterDbgout::SuiteFinish (const TestSuite & suite)
{
  if (trace)
  {
    stringstream ss;
    ss << "Suite finishing: " << suite.name << endl;
    OutputDebugString (widen (ss.str ()).c_str ());
  }
  return Reporter::SuiteFinish (suite);
}


/*!
  Output to debug output a failure message. If a test is in progress (the normal case)
  the message includes the name of the test and suite.

  \param failure - the failure information (filename, line number and message)
*/
void ReporterDbgout::ReportFailure (const Failure& failure)
{
  stringstream ss;
  ss << "Failure in ";
  if (CurrentTest)
  {
    if (CurrentSuite != DEFAULT_SUITE)
      ss << " suite " << CurrentSuite;
    ss << " test " << CurrentTest->test_name ();
  }
  ss << endl;

  OutputDebugString (widen(ss.str()).c_str());
  ss.clear ();
  ss.str ("");
  ss << failure.filename << "(" << failure.line_number << "):"
    << failure.message << endl;
  OutputDebugString (widen (ss.str ()).c_str ());
  Reporter::ReportFailure (failure);
}

/*!
  Prints a test run summary including number of tests, number of failures,
  running time, etc.
*/
int ReporterDbgout::Summary ()
{
  stringstream ss;
  if (total_failed_count > 0)
  {
    ss << "FAILURE: " << total_failed_count << " out of "
      << total_test_count << " tests failed (" << total_failures_count
      << " failures).";
  }
  else
  {
    ss << "Success: " << total_test_count << " tests passed.";
  }
  ss << endl;
  OutputDebugString (widen (ss.str ()).c_str ());
  ss.clear ();
  ss.str ("");
  ss.setf (ios::fixed);
  ss << "Run time: " << setprecision (2) << total_time_msec / 1000.;
  OutputDebugString (widen (ss.str ()).c_str ());

  return Reporter::Summary ();
}

}
