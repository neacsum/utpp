/*!
  \file suites_list.cpp
  \brief Implementation of UnitTest::SuitesList class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/utpp.h>
#include <algorithm>

namespace UnitTest {

/*!
  Add a test to a suite

  \param suite_name name of suite that will contain the test
  \param inf test information
*/
void SuitesList::Add (const std::string& suite_name, const TestSuite::Inserter* inf)
{
  std::deque <TestSuite>::iterator p;
  for (p = suites.begin (); p != suites.end (); p++)
    if (p->name == suite_name)
      break;

  if (p == suites.end ())
  {
    suites.push_back (TestSuite (suite_name));
    suites.back().Add (inf);
  }
  else
    p->Add (inf);
}

/*!
  Run tests in a suite

  \param suite_name name of the suite to run
  \param reporter test reporter to be used for results
  \param max_time_ms global time constraint in milliseconds

  \return number of tests that failed or -1 if there is no such suite
*/
int SuitesList::Run (const std::string& suite_name, Reporter& reporter, int max_time_ms)
{
  std::deque <TestSuite>::iterator p;
  for (p = suites.begin (); p != suites.end (); p++)
    if (p->name == suite_name)
      break;
  if (p != suites.end())
  {
    p->RunTests (reporter, max_time_ms);
    return reporter.Summary ();
  }
  return -1;
}

/*!
  Run tests in all suites
  \param reporter test reporter to be used for results
  \param max_time_ms global time constraint in milliseconds

  \return total number of failed tests
*/
int SuitesList::RunAll (Reporter& reporter, int max_time_ms)
{
  std::deque <TestSuite>::iterator p;
  for (p = suites.begin (); p != suites.end (); p++)
    p->RunTests (reporter, max_time_ms);


  return reporter.Summary ();
}

/*!
  Accesses the singleton object.
  \return The one and only SuitesList object
*/
SuitesList& SuitesList::GetSuitesList ()
{
  static SuitesList all_suites;
  return all_suites;
}

void SuitesList::Disable (const std::string& suite)
{
  std::deque <TestSuite>::iterator s;
  for (s = suites.begin (); s != suites.end (); s++)
  {
    if (s->name == suite)
      break;
  }
  if (s != suites.end ())
    s->Enable (false);
}

}