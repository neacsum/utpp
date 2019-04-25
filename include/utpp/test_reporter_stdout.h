#pragma once
/*!
  \file test_reporter_stdout.h
  \brief Definition of UnitTest::ReporterStdout class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/test_reporter.h>

namespace UnitTest {

/// A Reporter that sends messages to stdout
class ReporterStdout : public Reporter
{
protected:
  virtual void SuiteStart (const TestSuite& suite);
  virtual void TestStart (const Test& test);
  virtual void TestFinish (const Test& test);
  virtual int SuiteFinish (const TestSuite& suite);

  virtual void ReportFailure (const Failure& failure);
  virtual int Summary ();
};

}
