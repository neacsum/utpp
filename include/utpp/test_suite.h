#ifndef TEST_SUITE_H
#define TEST_SUITE_H
#pragma once
/*!
  \file test_suite.h
  \brief Definition of UnitTest::TestSuite class

  (c) Mircea Neacsu 2017
  See README.md file for full copyright information.
*/
#include <string>
#include <deque>
#include <utpp/test_reporter.h>
#include <utpp/time_helpers.h>

/// Name of default suite
#define DEFAULT_SUITE "DefaultSuite"

namespace UnitTest {

class Test;

/// Function pointer to a function that creates a test object
typedef UnitTest::Test* (*Testmaker)();

///A set of test cases that are run together
class TestSuite
{
public:
  /// Constructor of this objects inserts the test in suite
  class Inserter
  {
  public:
    Inserter (const std::string& suite,
      const std::string& test,
      const std::string& file,
      int line,
      Testmaker func);

  private:
    const std::string test_name,      ///< Test name
      file_name;                      ///< Filename where test was declared
    const int line;                   ///< Line number where test was declared
    const Testmaker maker;            ///< Test maker function

    friend class TestSuite;
  };


  TestSuite (const std::string& name);
  void Add (const Inserter* inf);
  bool IsEnabled () const;
  void Enable (bool on_off);
  int RunTests (Reporter& reporter, int max_runtime_ms);

  std::string name;     ///< Suite name

private:
  std::deque <const Inserter*> test_list;  ///< tests included in this suite
  int max_runtime;
  bool enabled;

  bool SetupCurrentTest (const Inserter* inf);
  void RunCurrentTest (const Inserter* inf);
  void TearDownCurrentTest (const Inserter* inf);
};

/// Returns true if suite is enabled
inline
bool TestSuite::IsEnabled () const
{
  return enabled;
}

///Enables or disables this suite
inline
void TestSuite::Enable (bool on_off)
{
  enabled = on_off;
}

/// Name of currently running suite
extern std::string CurrentSuite;

}

/*!
  Return current suite name for default suite. All other suites have the same
  function defined inside their namespaces.
*/
inline const char* GetSuiteName ()
{
  return DEFAULT_SUITE;
}

#endif

