#pragma once
/*
  UTPP - A New Generation of UnitTest++
  (c) Mircea Neacsu 2017-2023

  See LICENSE file for full copyright information.
*/

/*!
  \file checks.h
  \brief Definition of Check... template functions and 
  CHECK_... macro-definitions
*/

/// \cond
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
/// \endcond

#include <sstream>
#include <vector>
#include <array>
#include <list>
#include <string.h>
#include <math.h>

/*!
  \ingroup checks
@{
*/

/*!
  \def CHECK
  \brief Generate a failure if value is 0. Failure message is the value itself.

  \hideinitializer
*/
#ifdef CHECK
#error Macro CHECK is already defined
#endif
#define CHECK(value)                                                          \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      if (!UnitTest::Check(value))                                            \
        UnitTest::ReportFailure (__FILE__, __LINE__, "Check failed: " #value);\
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK(" #value ")");                          \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_EX
  \brief Generate a failure with the given message if value is 0

  \hideinitializer
*/
#ifdef CHECK_EX
#error Macro CHECK_EX is already defined
#endif
#define CHECK_EX(value, ...)                                                  \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      if (!UnitTest::Check(value)){                                           \
        char message[UnitTest::MAX_MESSAGE_SIZE];                             \
        sprintf (message, __VA_ARGS__);                                       \
        UnitTest::ReportFailure (__FILE__, __LINE__, message);                \
      }                                                                       \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EX(" #value ")");                       \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_EQUAL
  \brief Generate a failure if actual value is different from expected.

  \hideinitializer
*/
#ifdef CHECK_EQUAL
#error Macro CHECK_EQUAL is already defined
#endif

#define CHECK_EQUAL(expected, actual)                                         \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckEqual((expected), (actual), str__))                 \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_EQUAL_EX
  \brief  Generate a failure if actual value is different from expected.
          The given message is appended to the standard CHECK_EQUAL message. 

  \hideinitializer
*/
#ifdef CHECK_EQUAL_EX
#error Macro CHECK_EQUAL_EX is already defined
#endif
#define CHECK_EQUAL_EX(expected, actual, ...)                                 \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckEqual((expected), (actual), str__))                 \
      {                                                                       \
        char message[UnitTest::MAX_MESSAGE_SIZE];                             \
        sprintf (message, __VA_ARGS__);                                       \
        str__ += " - ";                                                       \
        str__ += message;                                                     \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
      }                                                                       \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL_EX(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_CLOSE
  \brief  Generate a failure if actual value differs from expected value with
          more than given tolerance

  \hideinitializer
*/

#ifdef CHECK_CLOSE
#error Macro CHECK_CLOSE is already defined
#endif
#define CHECK_CLOSE(expected, actual, tolerance)                              \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckClose ((expected), (actual), (tolerance), str__))   \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_CLOSE_EX
  \brief  Generate a failure if actual value differs from expected value with
          more than given tolerance.
          The given message is appended to the standard CHECK_CLOSE message.
  \hideinitializer
*/

#ifdef CHECK_CLOSE_EX
#error Macro CHECK_CLOSE_EX is already defined
#endif
#define CHECK_CLOSE_EX(expected, actual, tolerance, ...)                      \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckClose ((expected), (actual), (tolerance), str__))   \
      {                                                                       \
        char message[UnitTest::MAX_MESSAGE_SIZE];                             \
        sprintf (message, __VA_ARGS__);                                       \
        str__ += " - ";                                                       \
        str__ += message;                                                     \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
      }                                                                       \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_CLOSE_EX(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_ARRAY_EQUAL
  \brief Generate a failure if actual array is different from expected one

  \hideinitializer
*/
#ifdef CHECK_ARRAY_EQUAL
#error Macro CHECK_ARRAY_EQUAL is already defined
#endif

#define CHECK_ARRAY_EQUAL(expected, actual, count) \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckArrayEqual ((expected), (actual), (count), str__))  \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
       "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_ARRAY_CLOSE
  \brief  Generate a failure if `actual` array elements differs from `expected`
          ones with more than given `tolerance`.

  \hideinitializer
*/
#ifdef CHECK_ARRAY_CLOSE
#error Macro CHECK_ARRAY_CLOSE is already defined
#endif

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance)                 \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckArrayClose ((expected), (actual), (count), (tolerance), str__)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_ARRAY2D_EQUAL
  \brief  Generate a failure if 2D array `actual` differs from `expected` values.

  \hideinitializer
*/

#ifdef CHECK_ARRAY2D_EQUAL
#error Macro CHECK_ARRAY2D_EQUAL is already defined
#endif

#define CHECK_ARRAY2D_EQUAL(expected, actual, rows, columns)                  \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckArray2DEqual ((expected), (actual), (rows), (columns), str__)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY2D_EQUAL(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)


/*!
  \def CHECK_ARRAY2D_CLOSE
  \brief  Generate a failure if 2D array `actual` differs from `expected` values
  with more than given `tolerance`.

  \hideinitializer
*/
#ifdef CHECK_ARRAY2D_CLOSE
#error Macro CHECK_ARRAY2D_CLOSE is already defined
#endif

#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckArray2DClose (expected, actual, rows, columns, tolerance, str__)) \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_ARRAY2D_CLOSE(" #expected ", " #actual ")"); \
    }                                                                         \
  } while (0)

/*!
  \def CHECK_THROW
  \brief  Generate a failure if evaluating the expression __does not__ throw
          an exception of the `expected` type.
  \param except   Expected exception type
  \param expr     Expression to evaluate
  \hideinitializer
*/
#ifdef CHECK_THROW
#error Macro CHECK_THROW is already defined
#endif
#define CHECK_THROW(except, expr) \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { (expr); }                                                           \
    catch (const except& ) { caught_ = true; }                                \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unexpected exception in CHECK_THROW");                               \
    }                                                                         \
    if (!caught_)                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "expected: exception: \"" #except "\" not thrown");                   \
  } while(0)

/*!
  \def CHECK_THROW_EX
  \brief  Generate a failure if evaluating the expression __does not__ throw
          an exception of the `expected` type.

  Appends a printf type string to standard failure message.
  \hideinitializer
*/
#ifdef CHECK_THROW_EX
#error Macro CHECK_THROW_EX is already defined
#endif
#define CHECK_THROW_EX(except, expr, ...) \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { (expr); }                                                           \
    catch (const except& ) { caught_ = true; }                                \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unexpected exception in CHECK_THROW_EX");                            \
    }                                                                         \
    if (!caught_) {                                                           \
      std::string str__{"expected: exception: \"" #except "\" not thrown"};   \
      char message[UnitTest::MAX_MESSAGE_SIZE];                               \
      sprintf (message, __VA_ARGS__);                                         \
      str__ += " - ";                                                         \
      str__ += message;                                                       \
      UnitTest::ReportFailure (__FILE__, __LINE__, str__);                    \
    }                                                                         \
  } while(0)


/*!
  \def CHECK_THROW_EQUAL
  \brief  Checks if evaluating the expression triggers an exception of the given
          type and with the expected value.

  \hideinitializer
*/

#ifdef CHECK_THROW_EQUAL
#error Macro CHECK_THROW_EQUAL is already defined
#endif
#define CHECK_THROW_EQUAL(except, value, expression)                          \
  do                                                                          \
  {                                                                           \
    bool caught_ = false;                                                     \
    try { expression; }                                                       \
    catch (const except& actual) {                                            \
      caught_ = true;                                                         \
      std::string str__;                                                      \
      if (!UnitTest::CheckEqual(value, actual, str__))                        \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unexpected exception in CHECK_THROW_EQUAL");                         \
    }                                                                         \
    if (!caught_)                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "expected exception: \"" #except "\" not thrown");                    \
  } while(0)

/*!
  \def CHECK_FILE_EQUAL
  \brief  Generate a failure if the two files are different.

  \hideinitializer
*/
#ifdef CHECK_FILE_EQUAL
#error Macro CHECK_FILE_EQUAL is already defined
#endif
#define CHECK_FILE_EQUAL(expected, actual)                                    \
  do                                                                          \
  {                                                                           \
    try {                                                                     \
      std::string str__;                                                      \
      if (!UnitTest::CheckFileEqual((expected), (actual), str__))             \
        UnitTest::ReportFailure (__FILE__, __LINE__, str__);                  \
    }                                                                         \
    catch (...) {                                                             \
      UnitTest::ReportFailure (__FILE__, __LINE__,                            \
        "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")");    \
    }                                                                         \
  } while (0)

#ifdef ABORT
#error Macro ABORT is already defined
#endif

/*!
  \def ABORT
  \brief   Abort current test is value is 0. Abort message is the value itself

  \hideinitializer
*/
#define ABORT(value) \
  do                                                                          \
  {                                                                           \
    if (!UnitTest::Check(value))                                              \
      throw UnitTest::test_abort (__FILE__, __LINE__, #value);                \
  } while (0)

#ifdef ABORT_EX
#error Macro ABORT_EX is already defined
#endif

/*!
  \def ABORT_EX
  \brief  Abort current test is value is 0. Outputs the given message.

  \hideinitializer
*/
#define ABORT_EX(value, ...) \
  do                                                                          \
  {                                                                           \
    if (!UnitTest::Check(value)) {                                            \
      char message[UnitTest::MAX_MESSAGE_SIZE];                               \
      sprintf (message, __VA_ARGS__);                                         \
      throw UnitTest::test_abort (__FILE__, __LINE__, message);               \
    }                                                                         \
  } while (0)

///@}

namespace UnitTest {

//------------------ Check functions -----------------------------------------

/*!
  Check if value is true (or not 0)

  \param value object to check. Must have convertible to bool
  \return `true` if successful
*/ 

template <typename Value>
bool Check (Value const value)
{
  return (bool)value;
}

/*!
  Check if two values are equal. If not, generate a failure message.

  \param expected - expected_T value
  \param actual   - actual_T value
  \param msg      - generated error message
  \return `true` if values compare as equal
@{
*/
template <typename expected_T, typename actual_T>
bool CheckEqual (const expected_T& expected, const actual_T& actual, std::string& msg)
{
  if (!(expected == actual))
  {
    std::stringstream stream;
    stream << "Expected " << expected << " but was " << actual;
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
  return true;
}

template <typename expected_T, typename actual_T>
bool CheckEqual (const expected_T* expected, const actual_T* actual, std::string& msg)
{
  if (!(*expected == *actual))
  {
    std::stringstream stream;
    stream << "Expected " << *expected << " but was " << *actual;
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
  return true;
}
///@}

/*!
  CheckEqual function for C strings.

  \param expected - expected_T string value
  \param actual   - actual_T string value
  \param msg      - generated error message

  \return `true` if strings match
*/
template <>
inline
bool CheckEqual<char, char> (const char* expected, const char* actual, std::string& msg)
{
  if (strcmp (expected, actual))
  {
    std::stringstream stream;
    stream << "Expected \'" << expected << "\' but was \'" << actual << "\'";
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
  return true;
}

/*!
  CheckEqual for C++ vectors.

  The generated error message lists the expected and actual vector elements.

  \param expected - expected_T vector values
  \param actual   - actual_T vector values
  \param msg      - generated error message

  \return `true` if vectors compare as equal
*/ 
template <typename T>
inline
bool CheckEqual (const std::vector<T>& expected, const std::vector<T>& actual, std::string& msg)
{
  if (expected != actual)
  {
    std::stringstream stream;
    stream << "Expected [ ";
    typename std::vector<T>::const_iterator p;
    for (p = expected.begin (); p != expected.end (); ++p)
      stream << *p << " ";

    stream << "] but was [ ";
    for (p = actual.begin (); p != actual.end (); ++p)
      stream << *p << " ";

    stream << "]";
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
  return true;
}

/*!
  CheckEqual for C++ arrays.

  The generated error message lists the expected and actual array elements.

  \param expected - expected_T array
  \param actual   - actual_T array
  \param msg      - generated error message

  \return `true` if arrays compare as equal
*/
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
  else
    msg.clear ();
  return true;
}

/*!
  CheckEqual for C++ lists.

  The generated error message shows the expected and actual list elements.

  \param expected - expected_T list
  \param actual   - actual_T list
  \param msg      - generated error message

  \return `true` if lists compare as equal
*/
template <typename T>
inline
bool CheckEqual (const std::list<T>& expected, const std::list<T>& actual, std::string& msg)
{
  if (expected != actual)
  {
    std::stringstream stream;
    stream << "Expected ( ";
    for (auto const& x : expected)
      stream << x << " ";

    stream << ") but was ( ";
    for (auto const& x : actual)
      stream << x << " ";

    stream << ")";
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
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

  \param expected   - expected value
  \param actual     - actual value
  \param tolerance  - allowed tolerance
  \param msg        - generated error message

  \return `true` if actual value is within the tolerance range
*/
template <typename expected_T, typename actual_T, typename Tolerance>
bool CheckClose (const expected_T& expected, const actual_T& actual, const Tolerance& tolerance,
                 std::string& msg)
{
  if (abs (actual - expected) > tolerance)
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected " << expected << " +/- " << tolerance << " but was " << actual;
    msg = stream.str ();
    return false;
  }
  else
    msg.clear ();
  return true;
}

/*!
  Return true if two arrays are equal.
  \param expected   - array of expected values
  \param actual     - array of actual values
  \param count      - number of elements in each array

  \return `true` if the two arrays are equal
*/
template <typename expected_T, typename actual_T>
bool Equal1D (const expected_T& expected, const actual_T& actual, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    if (expected[i] != actual[i])
      return false;
  return true;
}

/*!
  Check if two arrays are equal. If not, generate a failure message.
  \param expected   - Expected value
  \param actual     - Actual value
  \param count      - number of elements in each array
  \param msg        - generated error message

  \return `true` if the two values are equal
*/
template <typename expected_T, typename actual_T>
bool CheckArrayEqual (const expected_T& expected, const actual_T& actual,
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

/*!
  Return true if values in two arrays are closer than specified tolerance.

  \param expected   - array of expected values
  \param actual     - array of actual values
  \param count      - number of elements in each array
  \param tolerance  - allowed tolerance

  \return `true` if all actual values are within the tolerance range
*/
template <typename expected_T, typename actual_T, typename Tolerance>
bool Close1D (const expected_T& expected, const actual_T& actual, size_t count, const Tolerance& tolerance)
{
  for (size_t i = 0; i < count; ++i)
  {
    if (abs (expected[i] - actual[i]) > tolerance)
      return false;
  }
  return true;
}


/*!
  Check if values in two C arrays are closer than specified tolerance. If not,
  generate a failure message.

  \param expected   - array of expected values
  \param actual     - array of actual values
  \param count      - arrays size
  \param tolerance  - allowed tolerance
  \param msg        - generated error message

  \return `true` if all actual values are within the tolerance range
*/
template <typename expected_T, typename actual_T, typename Tolerance>
bool CheckArrayClose (const expected_T& expected, const actual_T& actual,
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

/*!
  Check if values in two C++ vectors are closer than specified tolerance. If not,
  generate a failure message.

  \param expected   - vector of expected values
  \param actual     - vector of actual values
  \param tolerance  - allowed tolerance
  \param msg        - generated error message
  \return `true` if all actual values are within the tolerance range
*/
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

/*!
  Check if values in two C++ arrays are closer than specified tolerance. If not,
  generate a failure message.

  \param expected   - array of expected values
  \param actual     - array of actual values
  \param tolerance  - allowed tolerance
  \param msg        - generated error message
  \return `true` if all actual values are within the tolerance range
*/
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

/*!
  Return true if two 2D arrays are equal.
  \param expected - array of expected values
  \param actual   - array of actual values
  \param rows     - number of rows in each array
  \param columns  - number of columns in each array

  \return `true` if the two arrays are equal
*/
template <typename expected_T, typename actual_T>
bool Equal2D (const expected_T& expected, const actual_T& actual, size_t rows, size_t columns)
{
  for (size_t i = 0; i < rows; ++i)
    if (!Equal1D (expected[i], actual[i], columns))
      return false;
  return true;
}

/*!
  Check if two 2D arrays are equal. If not, generate a failure message.
  \param expected - array of expected values
  \param actual   - array of actual values
  \param rows     - number of rows in each array
  \param columns  - number of columns in each array
  \param msg      - generated error message

  \return `true` if the two arrays are equal
*/
template <typename expected_T, typename actual_T>
bool CheckArray2DEqual (const expected_T& expected, const actual_T& actual,
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

/*!
  Return true if values in two 2D arrays are closer than specified tolerance.
  \param expected   - array of expected values
  \param actual     - array of actual values
  \param rows       - number of rows in each array
  \param columns    - number of columns in each array
  \param tolerance  - allowed tolerance

  \return `true` if all values in the two arrays are within given tolerance
*/
template <typename expected_T, typename actual_T, typename Tolerance>
bool Close2D (const expected_T& expected, const actual_T& actual, size_t rows, size_t columns, const Tolerance& tolerance)
{
  for (size_t i = 0; i < rows; ++i)
    if (!Close1D (expected[i], actual[i], columns, tolerance))
      return false;
  return true;
}

/*!
  Check if values in two 2D arrays are closer than specified tolerance. If not,
  generate a failure message.
  \param expected   - array of expected values
  \param actual     - array of actual values
  \param rows       - number of rows in each array
  \param columns    - number of columns in each array
  \param tolerance  - allowed tolerance
  \param msg        - generated error message

  \return `true` if all values in the two arrays are within given tolerance
*/
template <typename expected_T, typename actual_T, typename Tolerance>
bool CheckArray2DClose (const expected_T& expected, const actual_T& actual,
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
  msg.clear ();
  return true;
}

#ifndef _WIN32
#define sprintf_s sprintf
#endif

/*!
  Function called by CHECK_FILE_EQUAL() macro to compare two files.
  \param ref      Name of reference file
  \param actual   Name of output file
  \param message  Generated error message
  \return `true` if files are equal

  Files are compared as ASCII files and the error message tries to show where
  the first difference is.
*/
inline
bool CheckFileEqual (const char* ref, const char* actual, std::string& message)
{
  struct stat st1, st2;
  char buf[1024];

  stat (ref, &st1);
  stat (actual, &st2);
  if (st1.st_size != st2.st_size)
  {
    sprintf_s (buf, "Size is different (%ld vs %ld) while comparing %s and %s",
      st1.st_size, st2.st_size, ref, actual);
    message = buf;
    return false;
  }

  FILE* f1, * f2;
  f1 = fopen (ref, "r");
  f2 = fopen (actual, "r");
  if (!f1 || !f2)
  {
    if (f1) fclose (f1);
    if (f2) fclose (f2);
    sprintf_s (buf, "Failed to open files while comparing %s and %s",
      ref, actual);
    message = buf;
    return false; //something wrong with files
  }

  size_t ln = 0;
  bool ok = true;
  char ln1[1024], ln2[1024];
  while (ok)
  {
    ln++;
    if (fgets (ln1, sizeof (ln1), f1)
      && fgets (ln2, sizeof (ln2), f2))
      ok = !strcmp (ln1, ln2);
    else
      break;
  }
  fclose (f1);
  fclose (f2);
  if (!ok)
  {
    char* p1, * p2;
    int off;
    for (off = 0, p1 = ln1, p2 = ln2;
      *p1 && *p2 && *p1 == *p2;
      p1++, p2++, off++)
      ;
    sprintf_s (buf, "Difference at line %zu position %d while comparing %s and %s",
      ln, off, ref, actual);
    message = buf;
  }
  else
    message.clear ();
  return ok;
}

}

/// \cond
#ifndef _WIN32
#undef sprintf_s
#endif
/// \endcond

/*!
  \ingroup gt 
  These macro definitions provide some compatibility with GoogleTest
@{
*/

#define EXPECT_TRUE(x) CHECK (x)
#define EXPECT_FALSE(x) CHECK (!(x))
#define EXPECT_EQ(A, B) CHECK ((A) == (B))
#define EXPECT_NE(A, B) CHECK ((A) != (B))
#define EXPECT_GE(A, B) CHECK ((A) >= (B))
#define EXPECT_GT(A, B) CHECK ((A) > (B))
#define EXPECT_LE(A, B) CHECK ((A) <= (B))
#define EXPECT_LT(A, B) CHECK ((A) < (B))

#define EXPECT_NEAR(A, B, tol) CHECK_CLOSE(B, A, tol)
#define EXPECT_THROW(expr, except) CHECK_THROW(except, expr)
#define ASSERT_THROW(expr, except) CHECK_THROW(except, expr)

#define ASSERT_FALSE(expr) ABORT (!expr)
#define ASSERT_TRUE(expr) ABORT (expr)
#define ASSERT_EQ(e1, e2) ABORT ((e1) == (e2))
#define ASSERT_NE(e1, e2) ABORT ((e1) != (e2))
#define ASSERT_GE(e1, e2) ABORT ((e1) >= (e2))
#define ASSERT_GT(e1, e2) ABORT ((e1) > (e2))
#define ASSERT_LE(e1, e2) ABORT ((e1) <= (e2))
#define ASSERT_LT(e1, e2) ABORT ((e1) < (e2))
///@}

/*!
  \defgroup checks  Assertion Checking Macros
  \defgroup tests   Test Definition Macros
  \defgroup time    Time Control Macros
  \defgroup gt      Compatibility Macros
  \defgroup exec    Execution Control 
*/