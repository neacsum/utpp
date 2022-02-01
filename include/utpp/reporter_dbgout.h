#pragma once
/*!
  \file reporter_dbgout.h
  \brief Definition of UnitTest::ReporterDbgout class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/utpp.h>
#include <iomanip>

namespace UnitTest {

/// A Reporter that sends messages to debug output
class ReporterDbgout : public Reporter
{
protected:
  void SuiteStart (const TestSuite& suite);
  void TestStart (const Test& test);
  void TestFinish (const Test& test);
  int SuiteFinish (const TestSuite& suite);

  void ReportFailure (const Failure& failure);
  int Summary ();
private:
  std::wstring widen (const std::string& s);
};


/// If tracing is enabled, show a suite start message 
inline
void ReporterDbgout::SuiteStart(const TestSuite& suite)
{
    Reporter::SuiteStart(suite);
    if (!trace)
        return;

    std::stringstream ss;
    ss << "Suite starting: " << suite.name << std::endl;
    OutputDebugString(widen(ss.str()).c_str());
}

/// If tracing is enabled, show a test start message 
inline
void ReporterDbgout::TestStart(const Test& test)
{
    Reporter::TestStart(test);
    if (!trace)
        return;
    std::stringstream ss;
    ss << "Test starting: " << test.test_name() << std::endl;
    OutputDebugString(widen(ss.str()).c_str());
}

/// If tracing is enabled, show a test finish message 
inline
void ReporterDbgout::TestFinish(const Test& test)
{
    if (trace)
    {
        std::stringstream ss;
        ss << "Test finished: " << test.test_name() << std::endl;
        OutputDebugString(widen(ss.str()).c_str());
    }
    Reporter::TestFinish(test);
}

/// If tracing is enabled, show a suite finish message 
inline
int ReporterDbgout::SuiteFinish(const TestSuite& suite)
{
    if (trace)
    {
        std::stringstream ss;
        ss << "Suite finishing: " << suite.name << std::endl;
        OutputDebugString(widen(ss.str()).c_str());
    }
    return Reporter::SuiteFinish(suite);
}


/*!
  Output to debug output a failure message. If a test is in progress (the normal case)
  the message includes the name of the test and suite.

  \param failure - the failure information (filename, line number and message)
*/
inline
void ReporterDbgout::ReportFailure(const Failure& failure)
{
    std::stringstream ss;
    ss << "Failure in ";
    if (CurrentTest)
    {
        if (CurrentSuite != DEFAULT_SUITE)
            ss << " suite " << CurrentSuite;
        ss << " test " << CurrentTest->test_name();
    }
    ss << std::endl;

    OutputDebugString(widen(ss.str()).c_str());
    ss.clear();
    ss.str("");
    ss << failure.filename << "(" << failure.line_number << "):"
        << failure.message << std::endl;
    OutputDebugString(widen(ss.str()).c_str());
    Reporter::ReportFailure(failure);
}

/*!
  Prints a test run summary including number of tests, number of failures,
  running time, etc.
*/
inline
int ReporterDbgout::Summary()
{
    std::stringstream ss;
    if (total_failed_count > 0)
    {
        ss << "FAILURE: " << total_failed_count << " out of "
            << total_test_count << " tests failed (" << total_failures_count
            << " failures).";
    }
    else
    {
        ss << "Success: " << total_test_count << " tests passed.";
    }
    ss << std::endl;
    OutputDebugString(widen(ss.str()).c_str());
    ss.clear();
    ss.str("");
    ss.setf(std::ios::fixed);
    ss << "Run time: " << std::setprecision(2) << total_time_msec / 1000.;
    OutputDebugString(widen(ss.str()).c_str());

    return Reporter::Summary();
}

/// Conversion from UTF-16 to UTF-8
inline
std::wstring ReporterDbgout::widen(const std::string& s)
{
    int nsz = (int)s.size();
    int wsz = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), nsz, 0, 0);
    std::wstring out(wsz, 0);
    if (wsz)
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), nsz, &out[0], wsz);
    return out;
}

}
