#pragma once
/*!
  \file utpp.h
  \brief Master include file 

  This is the only header that users have to include. It takes care of pulling
  in all the other required headers.

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#if defined (_MSVC_LANG) && (_MSVC_LANG > 199711l)
#define UTPP_CPP11    //use newer features of C++
#endif

#include "test.h"
#include "test_suite.h"
#include "test_reporter.h"
#include "test_reporter_stdout.h"
#include "test_reporter_dbgout.h"
#include "test_reporter_xml.h"
#include "test_macros.h"
#include "suites_list.h"
#include "check_macros.h"
#include "time_constraint.h"

namespace UnitTest {

/// Run all tests from all test suites
int RunAllTests (Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

/// Disable a test suite
void DisableSuite (const std::string& suite_name);

/// Enable a test suite
void EnableSuite (const std::string& suite_name);

/// Run all tests from one suite
int RunSuite (const char *suite_name, Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

}
#ifdef _MSC_VER
#pragma comment (lib, "utpp.lib")
#endif