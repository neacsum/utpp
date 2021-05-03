#pragma once
/*!
  \file test_reporter_xml.h
  \brief Definition of UnitTest::ReporterXml class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include "deferred_test_reporter.h"

#include <iosfwd>

namespace UnitTest
{

/// A Reporter that sends results to an XML file.
class ReporterXml : public ReporterDeferred
{
public:
  ReporterXml (std::ostream& ostream);

  int Summary ();

protected:
  void BeginTest (const ReporterDeferred::TestResult& result);
  void AddFailure (const ReporterDeferred::TestResult& result);
  void EndTest (const ReporterDeferred::TestResult& result);

private:
#ifdef UTPP_CPP11
  ReporterXml (ReporterXml const&) = delete;
  ReporterXml& operator=(ReporterXml const&) = delete;
#endif

  std::ostream& os;
  std::string start_time;
};

}
