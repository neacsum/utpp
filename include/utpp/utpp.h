#pragma once
/*!
  \file utpp.h
  \brief Master include file 

  This is the only header that users have to include. It takes care of pulling
  in all the other required headers.

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/test.h>
#include <utpp/test_suite.h>
#include <utpp/test_reporter.h>
#include <utpp/test_reporter_dbgout.h>
#include <utpp/xml_test_reporter.h>
#include <utpp/test_macros.h>

#include <utpp/check_macros.h>
#include <utpp/time_constraint.h>

namespace UnitTest {

/// Run all tests from all test suites
int RunAllTests (Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

/// Disable a test suite
void DisableSuite (const std::string& suite_name);

/// Run all tests from one suite
int RunSuite (const char *suite_name, Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

}
#ifdef _MSC_VER
#pragma comment (lib, "utpp.lib")
#endif