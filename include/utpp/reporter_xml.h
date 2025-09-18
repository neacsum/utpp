#pragma once
/*
  UTPP - A New Generation of UnitTest++
  (c) Mircea Neacsu 2017-2024

  See LICENSE file for full copyright information.
*/

/*!
  \file reporter_xml.h
  \brief Definition of UnitTest::ReporterXml class
*/

#include <iostream>
#include <iomanip>
#include <ctime>

namespace UnitTest
{

/// A Reporter that generates XML formatted results similar to NUnit
class ReporterXml : public ReporterDeferred
{
public:
  explicit ReporterXml (std::ostream& ostream = std::cout);

  int Summary () override;
  void Clear () override;

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
  std::chrono::utc_clock::time_point start_time;
  std::ios orig_state;
};

inline
std::string ReporterXml::xml_escape (const std::string& value)
{
  //TODO trade style for speed
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

/*!
  Constructor.

  \param ostream Output stream that will contain XML formatted results
*/
inline
ReporterXml::ReporterXml (std::ostream& ostream)
  : os (ostream)
  , orig_state (nullptr)
{
  start_time = std::chrono::utc_clock::now();
  orig_state.copyfmt (os);
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

}

/// Generate XML report
inline
int ReporterXml::Summary ()
{
  using namespace std::chrono;

  std::string suite;
  os.copyfmt (orig_state);
  auto beg_time = std::chrono::time_point_cast<std::chrono::seconds>(start_time);
  auto end_time = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::utc_clock::now ());
  auto total_time_s = duration_cast<duration<float, std::chrono::seconds::period>>(total_time);

  os << "<utpp-results"
    << " total=\"" << total_test_count << '\"'
    << " failed=\"" << total_failed_count << '\"'
    << " failures=\"" << total_failures_count << '\"'
    << " duration=\"" << std::fixed << std::setprecision (3) << total_time_s << '\"'
    << '>' << std::endl;
  os << " <start-time>" << std::format("{0:%F} {0:%T}Z", beg_time) << "</start-time>" << std::endl;

#ifdef _WIN32
  std::string cmd;
  std::wstring wcmd{ GetCommandLineW () };
  int nsz = WideCharToMultiByte (CP_UTF8, 0, wcmd.c_str (), -1, 0, 0, 0, 0);
  if (nsz)
  {
    cmd.resize (nsz);
    WideCharToMultiByte (CP_UTF8, 0, wcmd.c_str (), -1, &cmd[0], nsz, 0, 0);
    cmd.resize (nsz - 1); //output is null-terminated
  }
  os << " <command-line>" << xml_escape (cmd) << "</command-line>" << std::endl;
#endif

  for (auto i = results.cbegin (); i != results.cend (); ++i)
  {
    if (i->test_name.empty ()) // New suite flag
    {
      if (!suite.empty ())
        os << " </suite>" << std::endl;
      suite = i->suite_name;
      if (suite == DEFAULT_SUITE)
        os << " <suite";
      else
        os << " <suite name=\"" << suite << '\"';
      if ((i + 1) == results.cend () || (i + 1)->test_name.empty ())
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

  os << " <end-time>" << std::format ("{0:%F} {0:%T}Z", end_time) << "</end-time>" << std::endl;
  os << "</utpp-results>" << std::endl;
  return ReporterDeferred::Summary ();
}

inline
void ReporterXml::Clear ()
{
  os.seekp (0);
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  start_time = std::chrono::utc_clock::now();

  ReporterDeferred::Clear ();
}

inline
void ReporterXml::BeginTest (const ReporterDeferred::TestResult& result)
{
  os << "  <test"
    << " name=\"" << result.test_name << "\""
    << " time=\"" << result.test_time << "\"";
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

  for (auto& fail : result.failures)
  {
    std::string escapedMessage = xml_escape (fail.message);
    std::string message = build_failure_message (fail.filename, fail.line_number, escapedMessage);

    os << "   <failure" << " message=\"" << message << "\"" << "/>" << std::endl;
  }
}

}
