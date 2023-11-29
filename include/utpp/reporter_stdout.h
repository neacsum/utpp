#pragma once
/*
  UTPP - A New Generation of UnitTest++
  (c) Mircea Neacsu 2017-2023

  See LICENSE file for full copyright information.
*/

/*!
  \file reporter_stdout.h
  \brief Definition of UnitTest::ReporterStdout class
*/

#include "utpp.h"
#include <iostream>
#include <iomanip>

namespace UnitTest {

/// A Reporter that sends messages to stdout
class ReporterStdout : public Reporter
{
protected:
  void SuiteStart (const TestSuite& suite) override;
  void TestStart (const Test& test) override;
  void TestFinish (const Test& test) override;
  int SuiteFinish (const TestSuite& suite) override;

  void ReportFailure (const Failure& failure) override;
  int Summary () override;
};

/// If tracing is enabled, show a suite start message
inline
void ReporterStdout::SuiteStart (const TestSuite& suite)
{
  Reporter::SuiteStart (suite);
  if (!trace)
    return;
  std::cout << "Suite starting: " << suite.name << std::endl;
}

/// If tracing is enabled, show a test start message
inline
void ReporterStdout::TestStart (const Test& test)
{
  Reporter::TestStart (test);
  if (!trace)
    return;
  std::cout << "Test starting: " << test.test_name () << std::endl;
}

/// If tracing is enabled, show a test finish message
inline
void ReporterStdout::TestFinish (const Test& test)
{
  if (trace)
    std::cout << "Test finished: " << test.test_name () << std::endl;
  Reporter::TestFinish (test);
}

/// If tracing is enabled, show a suite finish message
inline
int ReporterStdout::SuiteFinish (const TestSuite& suite)
{
  if (trace)
    std::cout << "Suite finishing: " << suite.name << std::endl;
  return Reporter::SuiteFinish (suite);
}


/*!
  Output to stdout a failure message. If a test is in progress (the normal case)
  the message includes the name of the test and suite.

  \param failure - the failure information (filename, line number and message)
*/
inline
void ReporterStdout::ReportFailure (const Failure& failure)
{
  std::cout << "Failure in ";
  if (CurrentTest)
  {
    if (CurrentSuite != DEFAULT_SUITE)
      std::cout << " suite " << CurrentSuite;
    std::cout << " test " << CurrentTest->test_name ();
  }
  std::cout << std::endl << failure.filename << "(" << failure.line_number << "):"
    << failure.message << std::endl;
  Reporter::ReportFailure (failure);
}

/*!
  Prints a test run summary including number of tests, number of failures,
  running time, etc.
*/
inline
int ReporterStdout::Summary ()
{
  if (total_failed_count > 0)
  {
    std::cout << "FAILURE: " << total_failed_count << " out of "
      << total_test_count << " tests failed (" << total_failures_count
      << " failures)." << std::endl;
  }
  else
    std::cout << "Success: " << total_test_count << " tests passed." << std::endl;

  std::cout.setf (std::ios::fixed);
  std::cout << "Run time: " << std::setprecision (2) << total_time_msec / 1000. << std::endl;
  return Reporter::Summary ();
}

}
