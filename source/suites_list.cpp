/*!
  \file suites_list.cpp
  \brief Implementation of UnitTest::SuitesList class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/suites_list.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace UnitTest {

/*!
  Add a test to a suite

  \param suite_name name of suite that will contain the test
  \param inf test information
*/
void SuitesList::Add (const std::string& suite_name, const TestSuite::Inserter* inf)
{
  auto p = find_if (suites.begin (), suites.end (), 
    [&suite_name](TestSuite& s) {return s.name == suite_name; });
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
  auto p = find_if (suites.begin (), suites.end (),
    [&suite_name](TestSuite& s) {return s.name == suite_name; });
  
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
  for_each (suites.begin (), suites.end (), 
    [&reporter, max_time_ms](TestSuite& s) 
    {
      std::cout << "SuitesList::RunAll" << std::endl;
      s.RunTests (reporter, max_time_ms); 
    }
  );

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
  auto s = std::find_if (suites.begin (), suites.end (), 
    [&suite](TestSuite s) {return s.name == suite; });
  if (s != suites.end ())
    s->Enable (false);
}

}