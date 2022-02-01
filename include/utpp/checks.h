#pragma once
/*!
  \file checks.h
  \brief Definition of Check... template functions

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <sstream>
#include <vector>
#include <array>
#include <string.h>
#include <math.h>

#ifdef CHECK
#error Macro CHECK is already defined
#endif
/// Generate a failure if value is 0. Failure message is the value itself.
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
/// Generate a failure if actual value is different from expected.
#define CHECK_EQUAL(expected, actual)                                         \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string __msg;                                                      \
      if (!UnitTest::CheckEqual((expected), (actual), __msg))                 \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL_EX(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_EQUAL_EX
#error Macro CHECK_EQUAL_EX is already defined
#endif
/// Generate a failure if actual value is different from expected.
/// Output message is appended with given message. 
#define CHECK_EQUAL_EX(expected, actual, message)                             \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string __msg;                                                      \
      if (!UnitTest::CheckEqual((expected), (actual), __msg))                 \
      {                                                                       \
        __msg += " - ";                                                       \
        __msg += message;                                                     \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
      }                                                                       \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL_EX(" #expected ", " #actual ")"); \
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
      std::string __msg;                                                      \
      if (!UnitTest::CheckClose ((expected), (actual), (tolerance), __msg))   \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
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
      std::string __msg;                                                      \
      if (!UnitTest::CheckArrayEqual ((expected), (actual), (count), __msg))  \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
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
      std::string __msg;                                                      \
      if (!UnitTest::CheckArrayClose ((expected), (actual), (count), (tolerance), __msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_ARRAY2D_EQUAL
#error Macro CHECK_ARRAY2D_EQUAL is already defined
#endif
/*!
  Generate a failure if an array differs from expected values with more than
  given tolerance.
*/
#define CHECK_ARRAY2D_EQUAL(expected, actual, rows, columns)                  \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string __msg;                                                      \
      if (!UnitTest::CheckArray2DEqual ((expected), (actual), (rows), (columns), __msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
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
  with more than given tolerance.
*/
#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string __msg;                                                      \
      if (!UnitTest::CheckArray2DClose (expected, actual, rows, columns, tolerance, __msg)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

#ifdef CHECK_THROW
#error Macro CHECK_THROW is already defined
#endif
/// Checks if evaluating the expression triggers an exception of the given type.
#define CHECK_THROW(ExpectedExceptionType, expression) \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { try { (expression); }                                               \
          catch (const ExpectedExceptionType& ) { caught_ = true; } }         \
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
    try { try { expression; }                                                 \
          catch (const ExpectedExceptionType& actual) {                       \
            caught_ = true;                                                   \
            std::string msg;                                                  \
            if (!UnitTest::CheckEqual(expected, actual, msg))                 \
              UnitTest::ReportFailure (__FILE__, __LINE__, msg);              \
          } }                                                                 \
    catch (...) {}                                                            \
    if (!caught_)                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Expected exception: \"" #ExpectedExceptionType "\" not thrown");     \
  } while(0)

#ifdef CHECK_FILE_EQUAL
#error Macro CHECK_FILE_EQUAL is already defined
#endif
/// Generate a failure if the two files are different.
#define CHECK_FILE_EQUAL(expected, actual)                                    \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string __msg;                                                      \
      if (!UnitTest::CheckFileEqual((expected), (actual), __msg))             \
        UnitTest::ReportFailure (__FILE__, __LINE__, __msg);                  \
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

///------------------ Check functions -----------------------------------------
namespace UnitTest {

/// Check if value is true (or not 0)
template <typename Value>
bool Check (Value const value)
{
  return !!value; // doing double negative to avoid silly VS warnings
}

/*!
  Check if two values are equal. If not, generate a failure message.
*/
template <typename Expected, typename Actual>
bool CheckEqual (const Expected& expected, const Actual& actual, std::string& msg)
{
  if (!(expected == actual))
  {
    std::stringstream stream;
    stream << "Expected " << expected << " but was " << actual;
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Specialization of CheckEqual for vectors
template <typename T>
inline
bool CheckEqual (const std::vector<T>& expected, const std::vector<T>& actual, std::string& msg)
{
  if (expected != actual)
  {
    std::stringstream stream;
    stream << "Expected [ ";
    typename std::vector<T>::const_iterator p;
    for (p = expected.begin(); p != expected.end(); ++p)
      stream << *p << " ";

    stream << "] but was [ ";
    for (p = actual.begin(); p != actual.end(); ++p)
      stream << *p << " ";

    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Specialization of CheckEqual for arrays
template <typename T, size_t N>
inline
bool CheckEqual (const std::array<T,N>& expected, const std::array<T,N>& actual, std::string& msg)
{
  if (expected != actual)
  {
    std::stringstream stream;
    stream << "Expected [ ";
    for (size_t i = 0; i < N; ++i)
      stream << expected[i] << " ";

    stream << "] but was [ ";
    for (size_t i = 0; i < N; ++i)
      stream << actual[i] << " ";

    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/*!
  Specializations of CheckEqual function for C strings
  @{
*/
inline
bool CheckEqual (const char* expected, const char* actual, std::string& msg)
{
  if (strcmp (expected, actual))
  {
    std::stringstream stream;
    stream << "Expected \'" << expected << "\' but was \'" << actual << "\'";
    msg = stream.str ();
    return false;
  }
  return true;
}

inline
bool CheckEqual (char* expected, char* actual, std::string& msg)
{
  return CheckEqual (const_cast<const char *>(expected), const_cast<const char*>(actual), msg);
}

inline
bool CheckEqual (const char* expected, char* actual, std::string& msg)
{
  return CheckEqual (const_cast<const char *>(expected), const_cast<const char*>(actual), msg);
}

inline
bool CheckEqual (char* expected, const char* actual, std::string& msg)
{
  return CheckEqual (const_cast<const char *>(expected), const_cast<const char*>(actual), msg);
}
///@}

/*!
  Check if two values are closer than specified tolerance. If not, generate a
  failure message.
*/
template <typename Expected, typename Actual, typename Tolerance>
bool CheckClose (const Expected& expected, const Actual& actual, const Tolerance& tolerance,
                 std::string& msg)
{
  if (abs(actual - expected) > tolerance)
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected " << expected << " +/- " << tolerance << " but was " << actual;
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if two arrays are equal
template <typename Expected, typename Actual>
bool Equal1D (const Expected& expected, const Actual& actual, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    if (expected[i] != actual[i])
      return false;
  return true;
}

/*!
  Check if two arrays are equal. If not, generate a failure message.
*/
template <typename Expected, typename Actual>
bool CheckArrayEqual (const Expected& expected, const Actual& actual,
                      size_t count, std::string& msg)
{
  if (!Equal1D (expected, actual, count))
  {
    std::stringstream stream;
    stream << "Expected [ ";
    for (size_t i = 0; i < count; ++i)
      stream << expected[i] << " ";

    stream << "] but was [ ";
    for (size_t i = 0; i < count; ++i)
      stream << actual[i] << " ";

    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if values in two arrays are closer than specified tolerance.
template <typename Expected, typename Actual, typename Tolerance>
bool Close1D (const Expected& expected, const Actual& actual, size_t count, const Tolerance& tolerance)
{
  bool equal = true;
  for (size_t i = 0; equal && i < count; ++i)
    equal = abs (expected[i] - actual[i]) <= tolerance;
  return equal;
}


/*!
  Check if values in two arrays are closer than specified tolerance. If not,
  generate a failure message.
*/
template <typename Expected, typename Actual, typename Tolerance>
bool CheckArrayClose (const Expected& expected, const Actual& actual,
                      size_t count, const Tolerance& tolerance, std::string& msg)
{
  if (!Close1D (expected, actual, count, tolerance))
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected [ ";
    for (size_t i = 0; i < count; ++i)
      stream << expected[i] << " ";

    stream << "] +/- " << tolerance << " but was [ ";
    for (size_t i = 0; i < count; ++i)
      stream << actual[i] << " ";
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Specialization of CheckClose function for vectors
template <typename T>
bool CheckClose (const std::vector<T>& expected, const std::vector<T>& actual, const T& tolerance,
  std::string& msg)
{
  if (expected.size () != actual.size () || !Close1D (&expected[0], &actual[0], expected.size(), tolerance))
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected [ ";
    typename std::vector<T>::const_iterator p;
    for (p = expected.begin(); p != expected.end(); ++p)
      stream << *p << " ";

    stream << "] +/- " << tolerance << " but was [ ";
    for (p = actual.begin(); p != actual.end(); ++p)
      stream << *p << " ";
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Specialization of CheckClose function for arrays
template <typename T, size_t N>
bool CheckClose (const std::array<T, N>& expected, const std::array<T, N>& actual, const T& tolerance,
  std::string& msg)
{
  if (!Close1D (&expected[0], &actual[0], N, tolerance))
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected [ ";
    typename std::array<T,N>::const_iterator p;
    for (p = expected.begin (); p != expected.end (); ++p)
      stream << *p << " ";

    stream << "] +/- " << tolerance << " but was [ ";
    for (p = actual.begin (); p != actual.end (); ++p)
      stream << *p << " ";
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if two 2D arrays are equal
template <typename Expected, typename Actual>
bool Equal2D (const Expected& expected, const Actual& actual, size_t rows, size_t columns)
{
  for (size_t i = 0; i < rows; ++i)
    if (!Equal1D (expected[i], actual[i], columns))
      return false;
  return true;
}

/*!
  Check if two 2D arrays are equal. If not, generate a failure message.
*/
template <typename Expected, typename Actual>
bool CheckArray2DEqual (const Expected& expected, const Actual& actual,
                        size_t rows, size_t columns, std::string& msg)
{
  if (!Equal2D (expected, actual, rows, columns))
  {
    std::stringstream stream;
    size_t i, j;
    stream << "Expected [\n";
    for (i = 0; i < rows; ++i)
    {
      stream << " [";
      for (j = 0; j < columns; ++j)
        stream << expected[i][j] << " ";
      stream << "]\n";
    }

    stream << "] but was [\n";
    for (i = 0; i < rows; ++i)
    {
      stream << " [";
      for (j = 0; j < columns; ++j)
        stream << actual[i][j] << " ";
      stream << "]\n";
    }
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if values in two 2D arrays are closer than specified tolerance.
template <typename Expected, typename Actual, typename Tolerance>
bool Close2D (const Expected& expected, const Actual& actual, size_t rows, size_t columns, const Tolerance& tolerance)
{
  for (size_t i = 0; i < rows; ++i)
    if (!Close1D (expected[i], actual[i], columns, tolerance))
      return false;
  return true;
}

/*!
  Check if values in two 2D arrays are closer than specified tolerance. If not,
  generate a failure message.
*/
template <typename Expected, typename Actual, typename Tolerance>
bool CheckArray2DClose (const Expected& expected, const Actual& actual,
                        size_t rows, size_t columns, const Tolerance& tolerance, std::string& msg)
{
  if (!Close2D (expected, actual, rows, columns, tolerance))
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected [\n";
    size_t i, j;
    for (i = 0; i < rows; ++i)
    {
      stream << " [ ";
      for (j = 0; j < columns; ++j)
        stream << expected[i][j] << " ";
      stream << "]\n";
    }

    stream << "] +/- " << tolerance << " but was [\n";
    for (i = 0; i < rows; ++i)
    {
      stream << " [ ";
      for (j = 0; j < columns; ++j)
        stream << actual[i][j] << " ";
      stream << "]\n";
    }
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

bool CheckFileEqual (const char* ref_file, const char *actual_file, std::string& msg);
}
