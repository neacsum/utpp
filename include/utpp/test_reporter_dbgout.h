#pragma once
/*!
  \file test_reporter_dbgout.h
  \brief Definition of UnitTest::ReporterDbgout class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/test_reporter.h>

namespace UnitTest {

/// A Reporter that sends messages to debug output
class ReporterDbgout : public Reporter
{
protected:
  virtual void ReportFailure (const Failure& failure);
  virtual int Summary ();
};

}
