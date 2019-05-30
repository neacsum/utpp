/*!
  \file xml_test_reporter.cpp
  \brief Implementation of UnitTest::ReporterXml class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/utpp.h>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

static void ReplaceChar (string& str, char c, string const& replacement)
{
  for (size_t pos = str.find (c); pos != string::npos; pos = str.find (c, pos + 1))
    str.replace (pos, 1, replacement);
}

static string XmlEscape (string const& value)
{
  string escaped = value;

  ReplaceChar (escaped, '&', "&amp;");
  ReplaceChar (escaped, '<', "&lt;");
  ReplaceChar (escaped, '>', "&gt;");
  ReplaceChar (escaped, '\'', "&apos;");
  ReplaceChar (escaped, '\"', "&quot;");

  return escaped;
}

static string BuildFailureMessage (string const& file, int line, string const& message)
{
  ostringstream failureMessage;
  failureMessage << file << "(" << line << ") : " << message;
  return failureMessage.str ();
}


namespace UnitTest {

/// Constructor
ReporterXml::ReporterXml (std::ostream& ostream)
  : os (ostream)
{
}

/// Generate XML report
int ReporterXml::Summary ()
{
  string suite;

  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;

  os << "<unittest-results"
    << " tests=\"" << total_test_count << "\""
    << " failedtests=\"" << total_failed_count << "\""
    << " failures=\"" << total_failures_count << "\""
    << " time_sec=\"" << fixed << setprecision (3) << total_time_msec / 1000. << "\""
    << ">" << endl;

#ifndef UTPP_CPP11
  typedef std::deque<TestResult>::iterator iter;
  for (iter i = results.begin (); i != results.end (); ++i)
#else
  for (auto i = results.begin (); i != results.end (); ++i)
#endif
  {
    if (i->test_name.empty()) // New suite flag
    {
      if (!suite.empty ())
        os << " </suite>" << endl;
      suite = i->suite_name;
      os << " <suite name=\"" << suite << '\"';
      if ((i + 1) == results.end () || (i + 1)->test_name.empty ())
      {
        // Next record is another suite. This suite is either empty or disabled
        os << " /";
        suite.clear ();
      }
      os << '>' << endl;
    }
    else
    {
      BeginTest (*i);

      if (!i->failures.empty ())
        AddFailure (*i);

      EndTest (*i);
    }
  }
  if (!suite.empty())
    os << " </suite>" << endl;
  os << "</unittest-results>" << endl;
  return ReporterDeferred::Summary ();
}

void ReporterXml::BeginTest (const ReporterDeferred::TestResult& result)
{
  os << "  <test"
     << " name=\"" << result.test_name << "\""
     << " time_ms=\"" << result.test_time_ms << "\"";
}

void ReporterXml::EndTest (const ReporterDeferred::TestResult& result)
{
  if (result.failures.empty())
    os << "/>";
  else
    os << "  </test>";

  os << endl;
}

void ReporterXml::AddFailure (const ReporterDeferred::TestResult& result)
{
  os << ">" << endl; // close <test> element

#ifndef UTPP_CPP11
  typedef _Deque_const_iterator<Failure, allocator<Failure> > iter;
  for (iter it = result.failures.begin() ; it != result.failures.end (); ++it)
#else
  for (auto it = result.failures.begin ();
       it != result.failures.end ();
       ++it)
#endif
  {
    string const escapedMessage = XmlEscape (it->message);
    string const message = BuildFailureMessage (it->filename, it->line_number, escapedMessage);

    os << "   <failure" << " message=\"" << message << "\"" << "/>" << endl;
  }
}

}
