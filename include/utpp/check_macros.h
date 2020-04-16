#pragma once
/*!
  \file check_macros.h
  \brief CHECK... macro-definitions.

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include "checks.h"
#include "test_reporter.h"

#ifdef CHECK
  #error Macro CHECK is already defined
#endif
///Generate a failure if value is 0. Failure message is the value itself.
#define CHECK(value)                                                          \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      if (!UnitTest::Check(value))                                            \
      {                                                                       \
        UnitTest::ReportFailure (__FILE__, __LINE__, "Check failed: " #value);\
      }                                                                       \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK(" #value ")");                          \
    }                                                                         \
  } while (0)

#ifdef CHECK_EX
  #error Macro CHECK_EX is already defined
#endif
///Generate a failure with the given message if value is 0
#define CHECK_EX(value, message)                                              \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      if (!UnitTest::Check(value))                                            \
        UnitTest::ReportFailure (__FILE__, __LINE__, message);                \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EX(" #value #message ")");              \
    }                                                                         \
  } while (0)

#ifdef CHECK_EQUAL
  #error Macro CHECK_EQUAL is already defined
#endif
///Generate a failure if actual value is different from expected
#define CHECK_EQUAL(expected, actual)                                         \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckEqual((expected), (actual), msg))                   \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

#ifdef CHECK_CLOSE
  #error Macro CHECK_CLOSE is already defined
#endif
/*!
  Generate a failure if actual value differs from expected value with more than
  given tolerance
*/
#define CHECK_CLOSE(expected, actual, tolerance)                              \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckClose ((expected), (actual), (tolerance), msg))     \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

#ifdef CHECK_ARRAY_EQUAL
  #error Macro CHECK_ARRAY_EQUAL is already defined
#endif
///Generate a failure if an array is different from expected results
#define CHECK_ARRAY_EQUAL(expected, actual, count) \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckArrayEqual ((expected), (actual), (count), msg))    \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
       "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_ARRAY_CLOSE
  #error Macro CHECK_ARRAY_CLOSE is already defined
#endif
/*!
  Generate a failure if an array differs from expected values with more than
  given tolerance
*/
#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance)                 \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckArrayClose ((expected), (actual), (count), (tolerance), msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_ARRY2D_EQUAL
  #error Macro CHECK_ARRAY2D_EQUAL is already defined
#endif
/*!
  Generate a failure if an array differs from expected values with more than
  given tolerance
*/
#define CHECK_ARRAY2D_EQUAL(expected, actual, rows, columns)                  \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckArray2DEqual ((expected), (actual), (rows), (columns), msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY2D_EQUAL(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)


#ifdef CHECK_ARRAY2D_CLOSE
  #error Macro CHECK_ARRAY2D_CLOSE is already defined
#endif
/*!
  Generate a failure if a two-dimensional array differs from expected values
  with more than given tolerance
*/
#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckArray2DClose (expected, actual, rows, columns, tolerance, msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_THROW
  #error Macro CHECK_THROW is already defined
#endif
/// Checks if evaluating the expression triggers an exception of the given type
#define CHECK_THROW(ExpectedExceptionType, expression) \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { expression; }                                                       \
    catch (const ExpectedExceptionType&) { caught_ = true; }                  \
    catch (...) {}                                                            \
    if (!caught_)                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Expected exception: \"" #ExpectedExceptionType "\" not thrown");     \
  } while(0)

#ifdef CHECK_THROW_EQUAL
  #error Macro CHECK_THROW_EQUAL is already defined
#endif
/*!
  Checks if evaluating the expression triggers an exception of the given type
  and with the expected value.
*/
#define CHECK_THROW_EQUAL(ExpectedExceptionType, expected, expression) \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { expression; }                                                       \
    catch (const ExpectedExceptionType& actual) {                             \
      caught_ = true;                                                         \
      std::string msg;                                                        \
      if (!UnitTest::CheckEqual(expected, actual, msg))                       \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {}                                                            \
    if (!caught_)                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Expected exception: \"" #ExpectedExceptionType "\" not thrown");     \
  } while(0)

#ifdef CHECK_FILE_EQUAL
#error Macro CHECK_FILE_EQUAL is already defined
#endif
///Generate a failure if the two files are different
#define CHECK_FILE_EQUAL(expected, actual)                                    \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string msg;                                                        \
      if (!UnitTest::CheckFileEqual((expected), (actual), msg))               \
        UnitTest::ReportFailure (__FILE__, __LINE__, msg);                    \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

#ifdef ABORT
#error Macro ABORT is already defined
#endif

/// Abort current test is value is 0. Abort message is the value itself
#define ABORT(value) \
  do                                                                          \
  {                                                                           \
    if (!UnitTest::Check(value))                                              \
      throw UnitTest::test_abort (__FILE__, __LINE__, #value);                \
  } while (0)

#ifdef ABORT_EX
#error Macro ABORT_EX is already defined
#endif

/// Abort current test is value is 0.
#define ABORT_EX(value, message) \
  do                                                                          \
  {                                                                           \
    if (!UnitTest::Check(value))                                              \
      throw UnitTest::test_abort (__FILE__, __LINE__, message);               \
  } while (0)
