#pragma once
/*!
  \file checks.h
  \brief Definition of Check... template functions

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <sstream>
#include <vector>

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
    for (auto p = expected.begin (); p != expected.end(); p++)
      stream << *p << " ";

    stream << "] but was [ ";
    for (auto p = actual.begin (); p != actual.end(); p++)
      stream << *p << " ";

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
  if (abs(actual - expected) <= tolerance)
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
    for (size_t expectedIndex = 0; expectedIndex < count; ++expectedIndex)
      stream << expected[expectedIndex] << " ";

    stream << "] but was [ ";
    for (int actualIndex = 0; actualIndex < count; ++actualIndex)
      stream << actual[actualIndex] << " ";

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
    for (size_t expectedIndex = 0; expectedIndex < count; ++expectedIndex)
      stream << expected[expectedIndex] << " ";

    stream << "] +/- " << tolerance << " but was [ ";
    for (size_t actualIndex = 0; actualIndex < count; ++actualIndex)
      stream << actual[actualIndex] << " ";
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
    for (auto p = expected.begin(); p != expected.end(); ++p)
      stream << *p << " ";

    stream << "] +/- " << tolerance << " but was [ ";
    for (auto p = actual.begin (); p != actual.end (); ++p)
      stream << *p << " ";
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if two 2D arrays are equal
template <typename Expected, typename Actual, typename Tolerance>
bool Equal2D (const Expected& expected, const Actual& actual, size_t rows, size_t columns)
{
  for (size_t i = 0; i < rows; i++)
    if (!Equal1D (expected[i], actual[i], columns))
      return false;
  return true;
}

/*!
  Check if two 2D arrays are equal. If not, generate a failure message.
*/
template <typename Expected, typename Actual, typename Tolerance>
bool CheckArray2DEqual (const Expected& expected, const Actual& actual,
                        size_t rows, size_t columns, const Tolerance& tolerance, std::string& msg)
{
  if (!Equal2D (expected, actual, rows, columns))
  {
    std::stringstream stream;
    stream << "Expected [ ";
    for (size_t expectedRow = 0; expectedRow < rows; ++expectedRow)
    {
      stream << "[ ";
      for (int expectedColumn = 0; expectedColumn < columns; ++expectedColumn)
        stream << expected[expectedRow][expectedColumn] << " ";
      stream << "] ";
    }

    stream << "] +/- " << tolerance << " but was [ ";
    for (size_t actualRow = 0; actualRow < rows; ++actualRow)
    {
      stream << "[ ";
      for (int actualColumn = 0; actualColumn < columns; ++actualColumn)
        stream << actual[actualRow][actualColumn] << " ";
      stream << "] ";
    }
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

/// Return true if values in two 2D arrays are closer than specified tolerance.
template <typename Expected, typename Actual, typename Tolerance>
bool Close2D (const Expected& expected, const Actual& actual, size_t rows, size_t columns)
{
  for (size_t i = 0; i < rows; i++)
    if (!Close1D (expected[i], actual[i], columns))
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
  bool equal = true;

  if (!Close2D (expected, actual, rows, columns))
  {
    int prec = (int)(1 - log10 ((double)tolerance));
    std::stringstream stream;
    stream.precision (prec);
    stream.setf (std::ios::fixed);
    stream << "Expected [ ";
    for (size_t expectedRow = 0; expectedRow < rows; ++expectedRow)
    {
      stream << "[ ";
      for (int expectedColumn = 0; expectedColumn < columns; ++expectedColumn)
        stream << expected[expectedRow][expectedColumn] << " ";
      stream << "] ";
    }

    stream << "] +/- " << tolerance << " but was [ ";
    for (size_t actualRow = 0; actualRow < rows; ++actualRow)
    {
      stream << "[ ";
      for (int actualColumn = 0; actualColumn < columns; ++actualColumn)
        stream << actual[actualRow][actualColumn] << " ";
      stream << "] ";
    }
    stream << "]";
    msg = stream.str ();
    return false;
  }
  return true;
}

}
