#pragma once
/*!
  \file suites_list.h
  \brief Definition of UnitTest::SuitesList class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <deque>
#include "test_suite.h"

namespace UnitTest {
class Reporter;

/// A singleton object containing all test suites
class SuitesList {
public:
  void Add (const std::string& suite, const TestSuite::Inserter* inf);
  int Run (const std::string& suite, Reporter& reporter, int max_time_ms);
  int RunAll (Reporter& reporter, int max_time_ms);
  static SuitesList& GetSuitesList ();
  void Enable (const std::string& suite, bool enable = true);

private:

  std::deque <TestSuite> suites;
};

}
