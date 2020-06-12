/*!
  \file test_suite.cpp
  \brief Implementation of UnitTest::TestSuite class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#include <utpp/utpp.h>

#include <cassert>
#include <sstream>
#include <deque>

using namespace std;

namespace UnitTest {

//The global CurrentTest object
Test* CurrentTest;

//The global CurrentSuite name
string CurrentSuite = DEFAULT_SUITE;

/*!
  \class UnitTest::TestSuite
  A suite maintains a container with information about all tests included in
  the suite.
*/

/*!
  Constructor
  \param name_  Suite name
*/
TestSuite::TestSuite (const std::string& name_)
  : name (name_)
  , max_runtime (0)
  , enabled (true)
{
}

/*!
  Add a new test information to test_list
  \param inf Pointer to Inserter information that will be added to the container

  Note that the container keeps the pointer itself and that could create
  lifetime issues. However this is not a problem in normal usage as inserter
  objects are statically created by the TEST... macros.
*/
void TestSuite::Add (const Inserter* inf)
{
  test_list.push_back (inf);
}

/*!
  Run all tests in suite

  \param rep Reporter object to be used
  \param maxtime maximum run time for each test
  \return number of failed tests

  Iterate through all test information objects doing the following:
*/
int TestSuite::RunTests (Reporter& rep, int maxtime)
{
  /// Establish reporter as CurrentReporter and suite as CurrentSuite
  CurrentSuite = name;
  CurrentReporter = &rep;
  
  ///Inform reporter that suite has started
  CurrentReporter->SuiteStart (*this);
  if (IsEnabled ())
  {
    std::deque <const Inserter*>::iterator listp = test_list.begin ();
    max_runtime = maxtime;
    while (listp != test_list.end ())
    {
      /// Setup the test context
      if (SetupCurrentTest (*listp))
      {
        RunCurrentTest (*listp); /// Run test
        TearDownCurrentTest (*listp);  /// Tear down test context
      }
      /// Repeat for all tests
      listp++;
    }
  }
  else
    ;

  ///At the end invoke reporter SuiteFinish function
  return CurrentReporter->SuiteFinish (*this);
}

/*!
  Invoke the maker function to create the test object.

  The actual test object might be derived also from a fixture. When maker 
  function is called, it triggers the construction of the fixture and this
  might fail. That is why the construction is wrapped in a try...catch block.

  \return true if constructor was successful
*/
bool TestSuite::SetupCurrentTest (const Inserter* inf)
{
  bool ok = false;
  try {
    CurrentTest = (inf->maker)();
    ok = true;
  }
  catch (UnitTest::test_abort& x)
  {
    std::stringstream stream;
    stream << " Aborted setup of " << inf->test_name << " - " << x.what ();
    ReportFailure (x.file, x.line, stream.str ());
  }
  catch (const std::exception& e)
  {
    std::stringstream stream;
    stream << "Unhandled exception: " << e.what ()
      << " while setting up test " << inf->test_name;
    ReportFailure (inf->file_name, inf->line, stream.str ());
  }
  catch (...)
  {
    ReportFailure (inf->file_name, inf->line, "Setup unhandled exception: Crash!");
  }
  return ok;
}

/// Run the test
void TestSuite::RunCurrentTest (const Inserter* inf)
{
  assert (CurrentTest);
  CurrentReporter->TestStart (*CurrentTest);


  try {
    CurrentTest->run ();
  }
  catch (UnitTest::test_abort& x)
  {
    ReportFailure (x.file, x.line, string ("Test aborted: ") + x.what ());
  }
  catch (const std::exception& e)
  {
    std::stringstream stream;
    stream << "Unhandled exception: " << e.what ()
      << " while running test " << inf->test_name;
    ReportFailure (inf->file_name, inf->line, stream.str ());
  }
  catch (...)
  {
    std::stringstream stream;
    stream << "Unhandled exception while running test " << inf->test_name;
    ReportFailure (inf->file_name, inf->line, stream.str());
  }

  int actual_time = CurrentTest->test_time_ms ();
  if (actual_time >= 0 && max_runtime && actual_time > max_runtime)
  {
    std::stringstream stream;
    stream << "Global time constraint failed while running test " << inf->test_name
      << " Expected under " << max_runtime
      << "ms but took " << actual_time << "ms.";

    ReportFailure (inf->file_name, inf->line, stream.str ());
  }
  CurrentReporter->TestFinish (*CurrentTest);
}

/// Delete current test instance
void TestSuite::TearDownCurrentTest (const Inserter* inf)
{
  try {
    delete CurrentTest;
    CurrentTest = 0;
  }
  catch (const std::exception& e)
  {
    std::stringstream stream;
    stream << "Unhandled exception: " << e.what ()
      << " while tearing down test " << inf->test_name;
    ReportFailure (inf->file_name, inf->line, stream.str ());
  }
  catch (...)
  {
    std::stringstream stream;
    stream << "Unhandled exception tearing down test " << inf->test_name;
    ReportFailure (inf->file_name, inf->line, stream.str());
  }
}

/////////////////////// TestSuite::Inserter ///////////////////////////////////

/*!
  \class TestSuite::Inserter
  %Inserter objects are instantiated by the TEST... macro-definitions. The
  constructor places a pointer to the object into the test queue of the suite.
*/

/*!
  Constructor.
  \param suite        Suite name
  \param test         %Test name
  \param file         Filename associated with this test
  \param ln           Line number associated with this test
  \param func         Factory for test object

  Calls SuiteList::Add() to add the test to a suite.
*/
TestSuite::Inserter::Inserter (const std::string& suite,
                        const std::string& test,
                        const std::string& file,
                        int ln,
                        Testmaker func)
  : test_name (test)
  , file_name (file)
  , line (ln)
  , maker (func)
{
  SuitesList::GetSuitesList ().Add (suite, this);
}


//////////////////////////// RunAll functions /////////////////////////////////

/*!
  Runs all test suites and produces results using the given reporter.
  \param  rpt           Reporter used to generate results
  \param  max_time_ms   Global time constraint or 0 if there is no time constraint.
  \return number of failed tests

  Each test is expected to run in under max_time_ms milliseconds. If a test takes
  longer, it generates a time constraint failure.
*/
int RunAllTests (Reporter& rpt, int max_time_ms)
{
  return SuitesList::GetSuitesList ().RunAll (rpt, max_time_ms);
}

/*!
  Runs all tests from one suite

  \param suite_name   Name of the suite to run
  \param rpt          Test reporter to be used for results
  \param max_time_ms  Global time constraint in milliseconds

  \return number of tests that failed or -1 if there is no such suite

*/
int RunSuite (const char *suite_name, Reporter& rpt, int max_time_ms)
{
  return SuitesList::GetSuitesList ().Run (suite_name, rpt, max_time_ms);
}

void DisableSuite (const std::string& suite_name)
{
  SuitesList::GetSuitesList ().Enable (suite_name, false);
}

void EnableSuite (const std::string& suite_name)
{
  SuitesList::GetSuitesList ().Enable (suite_name, true);
}


}
