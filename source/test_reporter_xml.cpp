/*!
  \file test_reporter_xml.cpp
  \brief Implementation of UnitTest::ReporterXml class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/utpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#endif

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
  char tmp[80];
  time_t now;
  time (&now);
  struct tm* t = gmtime (&now);
  strftime (tmp, sizeof (tmp), "%Y-%m-%d %H:%M:%SZ", t);
  start_time = tmp;
}

/// Generate XML report
int ReporterXml::Summary ()
{
  string suite;
  string cmd;
#ifdef _WIN32
  wstring wcmd{ GetCommandLineW () };
  int nsz = WideCharToMultiByte (CP_UTF8, 0, wcmd.c_str (), -1, 0, 0, 0, 0);
  if (nsz)
  {
    cmd.resize (nsz);
    WideCharToMultiByte (CP_UTF8, 0, wcmd.c_str (), -1, &cmd[0], nsz, 0, 0);
    cmd.resize (nsz - 1); //output is null-terminated
  }
#endif
  time_t now;
  char tmp[80];
  time (&now);
  struct tm* t = gmtime (&now);
  strftime (tmp, sizeof (tmp), "%Y-%m-%d %H:%M:%SZ", t);

  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;

  os << "<utpp-results"
    << " total=\"" << total_test_count << '\"'
    << " failed=\"" << total_failed_count << '\"'
    << " failures=\"" << total_failures_count << '\"'
    << " duration=\"" << fixed << setprecision (3) << total_time_msec / 1000. << '\"'
    << '>' << endl;
  os << " <start-time>" << start_time << "</start-time>" << endl;

  os << " <command-line>" <<  XmlEscape (cmd) << "</command-line>" << endl;

  std::deque<TestResult>::iterator i;
  for (i = results.begin (); i != results.end (); ++i)
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
  os << " <end-time>"
    << tmp
    << "</end-time>" << endl;
  os << "</utpp-results>" << endl;
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

  deque<Failure>::const_iterator it;
  for (it = result.failures.begin() ; it != result.failures.end (); ++it)
  {
    string const escapedMessage = XmlEscape (it->message);
    string const message = BuildFailureMessage (it->filename, it->line_number, escapedMessage);

    os << "   <failure" << " message=\"" << message << "\"" << "/>" << endl;
  }
}

}
