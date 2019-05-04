#pragma once
/*!
  \file test.h
  \brief Definition of UnitTest::Test class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <string>
#include <cassert>
#include <utpp/test_suite.h>

namespace UnitTest {

/// Exception thrown by ABORT macro
struct test_abort : public std::runtime_error
{
  test_abort (const char *file_, int line_, const char* msg) 
    : std::runtime_error (msg)
  , file (file_)
  , line (line_)
  {};
  const char *file;
  int line;
};


///Representation of a test case
class Test
{
public:
  Test (const std::string& testName);
  virtual ~Test ();
  void no_time_constraint ();

  int failure_count () const;
  int test_time_ms () const;
  const std::string& test_name () const;

  void failure ();
  void run ();

  /// Actual body of test.
  virtual void RunImpl () = 0;

protected:
  std::string name;                   ///< Name of this test
  int failures;                       ///< Number of failures in this test
  int time;                           ///< Run time or -1 if exempt from time constraints 

private:
  Test (Test const&) = delete;
  Test& operator =(Test const&) = delete;
  friend class TestSuite;
};


/// Return the number of failures in this test
inline 
int Test::failure_count () const
{
  return failures;
}

/// Return test running time in milliseconds
inline
int Test::test_time_ms () const
{
  return time;
}

/// Return test name
inline
const std::string& Test::test_name () const
{
  return name;
}

/// Flags the test as exempt from global time constraint
inline
void Test::no_time_constraint ()
{
  time = -1;
}

///Currently executing test
extern Test* CurrentTest;

/// Main error reporting function
void ReportFailure (const std::string& filename, int line, const std::string& message);
}
