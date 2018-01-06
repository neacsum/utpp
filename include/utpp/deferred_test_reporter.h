#pragma once
/*!
  \file deferred_test_reporter.h
  \brief Definition of UnitTest::ReporterDeferred class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/test_reporter.h>
#include <deque>

namespace UnitTest
{

/// A Reporter that keeps a list of test results.
class ReporterDeferred : public Reporter
{
public:
  ReporterDeferred () {};
  void SuiteStart (const TestSuite& suite);
  void TestStart (const Test& test);
  void ReportFailure (const Failure& failure);
  void TestFinish (const Test& test);

protected:
  /// %Test results including all failure messages
  struct TestResult
  {
    TestResult ();
    TestResult (const std::string& suite, const std::string& test);

    std::string suite_name;         ///< suite name
    std::string test_name;          ///< test name
    int test_time_ms;               ///< test running time in milliseconds
    std::deque<Failure> failures;   ///< All failures of a test
  };

  std::deque<TestResult> results;   ///< Results of all tests
};

/// Default constructor needed container inclusion
inline 
ReporterDeferred::TestResult::TestResult ()
  : test_time_ms (0)
{
}

/// Constructor
inline 
ReporterDeferred::TestResult::TestResult (const std::string& suite, const std::string& test)
  : suite_name (suite)
  , test_name (test)
  , test_time_ms (0)
{
}

}
