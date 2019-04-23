#pragma once
/*!
  \file test_macros.h
  \brief Macro-definitions for test generation.

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test.h>
#include <sstream>


#ifdef TEST
  #error Macro TEST is already defined
#endif

#ifdef TEST_FIXTURE
  #error Macro TEST_FIXTURE is already defined
#endif

#ifdef SUITE
#error Macro SUITE is already defined
#endif

/// Declares the beginning of a new test suite
#define SUITE(Name)                                                           \
  namespace Suite##Name                                                       \
  {                                                                           \
      inline char const* GetSuiteName () { return #Name ; }                   \
  }                                                                           \
  namespace Suite##Name

/// Defines a test case
#define TEST(Name)                                                            \
  class Test##Name : public UnitTest::Test                                    \
  {                                                                           \
  public:                                                                     \
    Test##Name() : Test(#Name) {}                                             \
  private:                                                                    \
    void RunImpl();                                                           \
  };                                                                          \
  UnitTest::Test* Name##_maker() {return new Test##Name; }                   \
  UnitTest::TestSuite::Inserter Name##_inserter (GetSuiteName(), #Name, __FILE__, __LINE__,\
    Name##_maker);                                                            \
  void Test##Name::RunImpl()

/// Defines a test case with an associated fixture
#define TEST_FIXTURE(Fixture, Name)                                           \
  class Test##Name : public Fixture, public UnitTest::Test                    \
  {                                                                           \
  public:                                                                     \
    Test##Name() : Fixture (), Test(#Name) {}                                 \
  private:                                                                    \
    void RunImpl();                                                           \
  };                                                                          \
  UnitTest::Test* Name##_maker() {return new Test##Name;}                     \
  UnitTest::TestSuite::Inserter Name##_inserter (GetSuiteName(), #Name,       \
    __FILE__, __LINE__, Name##_maker);                                        \
  void Test##Name::RunImpl()

