#pragma once
/*!
  \file test_reporter.h
  \brief Definition of UnitTest::Reporter class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <string>

namespace UnitTest {

/// The failure records the file name, the line number and a message
struct Failure 
{
  std::string filename;     ///< Name of file where a failure has occurred
  std::string message;      ///< Description of failure
  int line_number;          ///< Line number where the failure has occured
};

class Test;
class TestSuite;

/// Abstract base for all reporters
class Reporter
{
public:
  Reporter ();
  virtual ~Reporter () {};

  /// Controls test tracing feature
  void SetTrace (bool on_off) { trace = on_off; }

  /// Invoked at the beginning of a test suite
  virtual void SuiteStart (const TestSuite& suite);

  /// Invoked at the beginning of a test
  virtual void TestStart (const Test& test);

  /// Called when a test has failed
  virtual void ReportFailure (const Failure& failure);

  /// Invoked at the end of a test
  virtual void TestFinish (const Test& test);

  /// Invoked at the end of a test suite
  virtual int SuiteFinish (const TestSuite& suite);

  /// Generate results report
  virtual int Summary () { return total_failed_count; }

protected:
  int suite_test_count,     ///< number of tests in suite
    suite_failed_count,     ///< number of failed tests in suite
    suite_failures_count,   ///< number of failures in suite
    suite_time_msec;        ///< total suite running time in milliseconds

  int total_test_count,     ///< total number of tests
    total_failed_count,     ///< total number of failed tests
    total_failures_count,   ///< total number of failures
    total_time_msec;        ///< total running time in milliseconds
  
  int suites_count;         ///< number of suites ran
  bool trace;               ///< true if tracing is enabled

};

/// Return the default reporter object
Reporter& GetDefaultReporter ();

/// Pointer to current reporter object
extern Reporter* CurrentReporter;
}

