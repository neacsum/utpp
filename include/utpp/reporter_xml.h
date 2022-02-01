#pragma once
/*!
  \file test_reporter_xml.h
  \brief Definition of UnitTest::ReporterXml class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/utpp.h>

#include <iostream>
#include <iomanip>
#include <time.h>

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
  std::string xml_escape (const std::string& value);
  std::string build_failure_message (const std::string& file, int line, std::string const& message);

  ReporterXml (ReporterXml const&) = delete;
  ReporterXml& operator=(ReporterXml const&) = delete;

  std::ostream& os;
  std::string start_time;
};

inline
std::string ReporterXml::xml_escape (const std::string& value)
{
  std::string escaped = value;
  auto replace_char = [&escaped] (char c, const char* repl){
    for (auto pos = escaped.find(c); pos != std::string::npos; pos = escaped.find(c, pos + 1))
      escaped.replace(pos, 1, repl);

  };
  replace_char ('&', "&amp;");
  replace_char ('<', "&lt;");
  replace_char ('>', "&gt;");
  replace_char ('\'', "&apos;");
  replace_char ('\"', "&quot;");

  return escaped;
}

inline
std::string ReporterXml::build_failure_message (const std::string& file, int line, std::string const& message)
{
  std::ostringstream failureMessage;
  failureMessage << file << "(" << line << ") : " << message;
  return failureMessage.str();
}

/// Constructor
inline
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
inline
int ReporterXml::Summary ()
{
  std::string suite;
  std::string cmd;
#ifdef _WIN32
  std::wstring wcmd{ GetCommandLineW () };
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

  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

  os << "<utpp-results"
    << " total=\"" << total_test_count << '\"'
    << " failed=\"" << total_failed_count << '\"'
    << " failures=\"" << total_failures_count << '\"'
    << " duration=\"" << std::fixed << std::setprecision (3) << total_time_msec / 1000. << '\"'
    << '>' << std::endl;
  os << " <start-time>" << start_time << "</start-time>" << std::endl;

  os << " <command-line>" << xml_escape (cmd) << "</command-line>" << std::endl;

  std::deque<TestResult>::iterator i;
  for (i = results.begin (); i != results.end (); ++i)
  {
    if (i->test_name.empty ()) // New suite flag
    {
      if (!suite.empty ())
        os << " </suite>" << std::endl;
      suite = i->suite_name;
      os << " <suite name=\"" << suite << '\"';
      if ((i + 1) == results.end () || (i + 1)->test_name.empty ())
      {
        // Next record is another suite. This suite is either empty or disabled
        os << " /";
        suite.clear ();
      }
      os << '>' << std::endl;
    }
    else
    {
      BeginTest (*i);

      if (!i->failures.empty ())
        AddFailure (*i);

      EndTest (*i);
    }
  }
  if (!suite.empty ())
    os << " </suite>" << std::endl;
  os << " <end-time>"
    << tmp
    << "</end-time>" << std::endl;
  os << "</utpp-results>" << std::endl;
  return ReporterDeferred::Summary ();
}

inline
void ReporterXml::BeginTest (const ReporterDeferred::TestResult& result)
{
  os << "  <test"
    << " name=\"" << result.test_name << "\""
    << " time_ms=\"" << result.test_time_ms << "\"";
}

inline
void ReporterXml::EndTest (const ReporterDeferred::TestResult& result)
{
  if (result.failures.empty ())
    os << "/>";
  else
    os << "  </test>";

  os << std::endl;
}

inline
void ReporterXml::AddFailure (const ReporterDeferred::TestResult& result)
{
  os << ">" << std::endl; // close <test> element

  std::deque<Failure>::const_iterator it;
  for (it = result.failures.begin (); it != result.failures.end (); ++it)
  {
    std::string escapedMessage = xml_escape (it->message);
    std::string message = build_failure_message (it->filename, it->line_number, escapedMessage);

    os << "   <failure" << " message=\"" << message << "\"" << "/>" << std::endl;
  }
}

}
