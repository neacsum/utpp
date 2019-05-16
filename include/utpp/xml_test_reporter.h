#pragma once
/*!
  \file xml_test_reporter.h
  \brief Definition of UnitTest::ReporterXml class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/deferred_test_reporter.h>

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
#if __cplusplus > 201103L
  ReporterXml (ReporterXml const&) = delete;
  ReporterXml& operator=(ReporterXml const&) = delete;
#endif

  std::ostream& os;
};

}
