#pragma once
/*!
  \file utpp.h
  \brief Master include file 

  This is the only header that users have to include. It takes care of pulling
  in all the other required headers.

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/

#ifdef _WIN32
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <windows.h>
#endif

#include <string>
#include <deque>
#include <stdexcept>
#include <sstream>
#include <assert.h>

/// Name of default suite
#define DEFAULT_SUITE "DefaultSuite"

//------------------------------ Test macros ----------------------------------
#ifdef TEST
#error Macro TEST is already defined
#endif

#ifdef TEST_FIXTURE
#error Macro TEST_FIXTURE is already defined
#endif

#ifdef SUITE
#error Macro SUITE is already defined
#endif

#define TEST_MAIN(A, B) \
UnitTest::Test *UnitTest::CurrentTest; \
UnitTest::Reporter *UnitTest::CurrentReporter; \
std::string UnitTest::CurrentSuite; \
__int64 UnitTest::Timer::frequency; \
int main (A,B)

/// Declares the beginning of a new test suite
#define SUITE(Name)                                                           \
  namespace Suite##Name                                                       \
  {                                                                           \
      inline char const* GetSuiteName () { return #Name ; }                   \
  }                                                                           \
  namespace Suite##Name

/// Defines a test case
#define TEST(Name)                                                            \
  class Test##Name : public UnitTest::Test                                    \
  {                                                                           \
  public:                                                                     \
    Test##Name() : Test(#Name) {}                                             \
  private:                                                                    \
    void RunImpl();                                                           \
  };                                                                          \
  UnitTest::Test* Name##_maker() {return new Test##Name; }                   \
  UnitTest::TestSuite::Inserter Name##_inserter (GetSuiteName(), #Name, __FILE__, __LINE__,\
    Name##_maker);                                                            \
  void Test##Name::RunImpl()

/// Defines a test case with an associated fixture
#define TEST_FIXTURE(Fixture, Name)                                           \
  class Test##Name : public Fixture, public UnitTest::Test                    \
  {                                                                           \
  public:                                                                     \
    Test##Name() : Fixture (), Test(#Name) {}                                 \
  private:                                                                    \
    void RunImpl();                                                           \
  };                                                                          \
  UnitTest::Test* Name##_maker() {return new Test##Name;}                     \
  UnitTest::TestSuite::Inserter Name##_inserter (GetSuiteName(), #Name,       \
    __FILE__, __LINE__, Name##_maker);                                        \
  void Test##Name::RunImpl()

/// Defines a local (per scope) time constraint
#define UTPP_TIME_CONSTRAINT(ms) \
  UnitTest::TimeConstraint unitTest__timeConstraint__(ms, __FILE__, __LINE__)

/// Flags a test as not subject to the global time constraint
#define UTPP_TIME_CONSTRAINT_EXEMPT() no_time_constraint ()

namespace UnitTest {

// forward declarations
struct Failure;
void ReportFailure(const std::string& filename, int line, const std::string& message);

///Representation of a test case
class Test
{
public:
  Test (const std::string& testName);
  virtual ~Test() {};
  void no_time_constraint ();
  bool is_time_constraint () const;

  int failure_count () const;
  int test_time_ms () const;
  const std::string& test_name () const;

  void failure ();
  void run ();

  /// Actual body of test.
  virtual void RunImpl () = 0;

protected:
  std::string name;                   ///< Name of this test
  int failures;                       ///< Number of failures in this test
  int time;                           ///< Run time
  bool time_exempt;                   ///< _true_ if exempt from time constraints

private:
  Test (Test const&) = delete;
  Test& operator =(Test const&) = delete;
  friend class TestSuite;
};

/// The failure object records the file name, the line number and a message
struct Failure
{
  std::string filename;     ///< Name of file where a failure has occurred
  std::string message;      ///< Description of failure
  int line_number;          ///< Line number where the failure has occurred
};


/// Abstract base for all reporters
class Reporter
{
public:
  Reporter ();
  virtual ~Reporter () {};

  /// Controls test tracing feature
  void SetTrace (bool on_off) { trace = on_off; }

  /// Invoked at the beginning of a test suite
  virtual void SuiteStart (const TestSuite& suite);

  /// Invoked at the beginning of a test
  virtual void TestStart (const Test& test);

  /// Called when a test has failed
  virtual void ReportFailure (const Failure& failure);

  /// Invoked at the end of a test
  virtual void TestFinish (const Test& test);

  /// Invoked at the end of a test suite
  virtual int SuiteFinish (const TestSuite& suite);

  /// Generate results report
  virtual int Summary () { return total_failed_count; }

protected:
  int suite_test_count,     ///< number of tests in suite
    suite_failed_count,     ///< number of failed tests in suite
    suite_failures_count,   ///< number of failures in suite
    suite_time_msec;        ///< total suite running time in milliseconds

  int total_test_count,     ///< total number of tests
    total_failed_count,     ///< total number of failed tests
    total_failures_count,   ///< total number of failures
    total_time_msec;        ///< total running time in milliseconds

  int suites_count;         ///< number of suites ran
  bool trace;               ///< true if tracing is enabled

};

/// A Reporter that keeps a list of test results.
class ReporterDeferred : public Reporter
{
public:
  ReporterDeferred () {};
  void SuiteStart (const TestSuite& suite);
  void TestStart (const Test& test);
  void ReportFailure (const Failure& failure);
  void TestFinish (const Test& test);

protected:
  /// %Test results including all failure messages
  struct TestResult
  {
    TestResult ();
    TestResult (const std::string& suite, const std::string& test);

    std::string suite_name;         ///< suite name
    std::string test_name;          ///< test name
    int test_time_ms;               ///< test running time in milliseconds
    std::deque<Failure> failures;   ///< All failures of a test
  };

  std::deque<TestResult> results;   ///< Results of all tests
};

/// Function pointer to a function that creates a test object
typedef UnitTest::Test* (*Testmaker)();

/*!
  A set of test cases that are run together.

  A suite maintains a container with information about all tests included in
  the suite.
*/
class TestSuite
{
public:
  /// Constructor of this objects inserts the test in suite
  class Inserter
  {
  public:
    Inserter (const std::string& suite,
      const std::string& test,
      const std::string& file,
      int line,
      Testmaker func);

  private:
    const std::string test_name,      ///< Test name
      file_name;                      ///< Filename where test was declared
    const int line;                   ///< Line number where test was declared
    const Testmaker maker;            ///< Test maker function

    friend class TestSuite;
  };

  TestSuite (const std::string& name);
  void Add (const Inserter* inf);
  bool IsEnabled () const;
  void Enable (bool on_off);
  int RunTests (Reporter& reporter, int max_runtime_ms);

  std::string name;     ///< Suite name

private:
  std::deque <const Inserter*> test_list;  ///< tests included in this suite
  int max_runtime;
  bool enabled;

  bool SetupCurrentTest (const Inserter* inf);
  void RunCurrentTest (const Inserter* inf);
  void TearDownCurrentTest (const Inserter* inf);
};

/// An object that can be interrogated to get elapsed time
class Timer
{
public:
  Timer ();
  void Start ();
  int GetTimeInMs () const;
  long long GetTimeInUs () const;

private:
  long long GetTime () const;
  long long startTime;
  static long long frequency;
};

///Defines maximum run time of a test
class TimeConstraint
{
public:
  TimeConstraint (int ms, const char* file, int line);
  ~TimeConstraint ();

private:
  void operator=(TimeConstraint const&) = delete;
  TimeConstraint (TimeConstraint const&) = delete;

  Timer timer;
  std::string filename;
  int line_number;
  int const max_ms;
};

/// A singleton object containing all test suites
class SuitesList {
public:
  void Add (const std::string& suite, const TestSuite::Inserter* inf);
  int Run (const std::string& suite, Reporter& reporter, int max_time_ms);
  int RunAll (Reporter& reporter, int max_time_ms);
  static SuitesList& GetSuitesList ();
  void Enable (const std::string& suite, bool enable = true);

private:

  std::deque <TestSuite> suites;
};

/// Exception thrown by ABORT macro
struct test_abort : public std::runtime_error
{
  test_abort (const char* file_, int line_, const char* msg)
    : std::runtime_error (msg)
    , file (file_)
    , line (line_)
  {};
  const char* file;
  int line;
};

///Currently executing test
extern Test* CurrentTest;

/// Name of currently running suite
extern std::string CurrentSuite;

/// Pointer to current reporter object
extern Reporter* CurrentReporter;

/// Return the default reporter object
Reporter& GetDefaultReporter ();

/// Run all tests from all test suites
int RunAllTests (Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

/// Disable a test suite
void DisableSuite (const std::string& suite_name);

/// Enable a test suite
void EnableSuite (const std::string& suite_name);

/// Run all tests from one suite
int RunSuite (const char *suite_name, Reporter& rpt = GetDefaultReporter (), int max_time_ms = 0);

/// Main error reporting function
void ReportFailure (const std::string& filename, int line, const std::string& message);

//-------------------------- Test member functions ----------------------------
/// Constructor
inline
Test::Test(const std::string& test_name)
    : name(test_name)
    , failures(0)
    , time(0)
    , time_exempt(false)
{
}

/*!
  Starts a timer and calls RunImpl() to execute test code.

  When RunImpl() returns, it records the elapsed time.
*/
inline
void Test::run()
{
    Timer test_timer;
    test_timer.Start();

    RunImpl();
    time = test_timer.GetTimeInMs();
}

/*!
  Increment failures count for this test
*/
inline
void Test::failure()
{
    failures++;
}

/// Return the number of failures in this test
inline
int Test::failure_count () const
{
  return failures;
}

/// Return test running time in milliseconds
inline
int Test::test_time_ms () const
{
  return time;
}

/// Return test name
inline
const std::string& Test::test_name () const
{
  return name;
}

/// Flags the test as exempt from global time constraint
inline
void Test::no_time_constraint ()
{
  time_exempt = true;
}

/// Return _true_ if test must be run under global time constraints
inline
bool Test::is_time_constraint () const
{
  return !time_exempt;
}

//--------------------- Reporter member functions -----------------------------
inline
Reporter::Reporter ()
  : suite_test_count (0)
  , suite_failed_count (0)
  , suite_failures_count (0)
  , suite_time_msec (0)
  , total_test_count (0)
  , total_failed_count (0)
  , total_failures_count (0)
  , total_time_msec (0)
  , suites_count (0)
  , trace (false)
{
}

///  Records the beginning of a new test suite
inline
void Reporter::SuiteStart (const TestSuite&)
{
  suites_count++;
  suite_test_count = suite_failed_count = suite_failures_count = 0;
}

inline
void Reporter::TestStart (const Test&)
{
  suite_test_count++;
  total_test_count++;
}

inline
void Reporter::ReportFailure (const Failure& f)
{
}

inline
void Reporter::TestFinish (const Test& t)
{
  int f = t.failure_count ();
  if (f)
  {
    suite_failed_count++;
    suite_failures_count += f;
    total_failed_count++;
    total_failures_count += f;
  }
  int ms = t.test_time_ms ();
  suite_time_msec += ms;
  total_time_msec += ms;
}

///  \return number of failures in this suite
inline
int Reporter::SuiteFinish (const TestSuite&)
{
  return suite_failures_count;
}

//------------------- ReporterDeferred member functions -----------------------
/// Default constructor needed container inclusion
inline
ReporterDeferred::TestResult::TestResult ()
  : test_time_ms (0)
{
}

/// Constructor
inline
ReporterDeferred::TestResult::TestResult (const std::string& suite, const std::string& test)
  : suite_name (suite)
  , test_name (test)
  , test_time_ms (0)
{
}

/*!
  Called at the beginning of a new suite.
  \param  suite    New suite name

  Creates a new TestResult object with an empty test name as a suite start flag.
  Adds the TestResult object it to the results container.
*/
inline
void ReporterDeferred::SuiteStart (const TestSuite& suite)
{
  results.push_back (TestResult (CurrentSuite, std::string ()));
}

/*!
  Called at the beginning of a new test.
  \param  test    %Test that is about to start

  Creates a new TestResult object and adds it to the results container
*/
inline
void ReporterDeferred::TestStart (const Test& test)
{
  Reporter::TestStart (test);
  results.push_back (TestResult (CurrentSuite, test.test_name ()));
}

/*!
  Add a new failure to current test
  \param failure  The failure record
*/
inline
void ReporterDeferred::ReportFailure (const Failure& failure)
{
  assert (!results.empty ());

  Reporter::ReportFailure (failure);
  results.back ().failures.push_back (failure);
}

/*!
  Store test runtime when the test finishes
  \param  test    %Test that is about to end
*/
inline
void ReporterDeferred::TestFinish (const Test& test)
{
  Reporter::TestFinish (test);
  results.back ().test_time_ms = test.test_time_ms ();
}


//------------------- TestSuite member functions ------------------------------

inline
TestSuite::TestSuite(const std::string& name_)
    : name(name_)
    , max_runtime(0)
    , enabled(true)
{
}

/*!
  Add a new test information to test_list
  \param inf Pointer to Inserter information that will be added to the container

  Note that the container keeps the pointer itself and that could create
  lifetime issues. However this is not a problem in normal usage as inserter
  objects are statically created by the TEST... macros.
*/
inline
void TestSuite::Add(const Inserter* inf)
{
    test_list.push_back(inf);
}

/*!
  Run all tests in suite

  \param rep Reporter object to be used
  \param maxtime maximum run time for each test
  \return number of failed tests

  Iterate through all test information objects doing the following:
*/
inline
int TestSuite::RunTests(Reporter& rep, int maxtime)
{
    /// Establish reporter as CurrentReporter and suite as CurrentSuite
    CurrentSuite = name;
    CurrentReporter = &rep;

    ///Inform reporter that suite has started
    CurrentReporter->SuiteStart(*this);
    if (IsEnabled())
    {
        std::deque <const Inserter*>::iterator listp = test_list.begin();
        max_runtime = maxtime;
        while (listp != test_list.end())
        {
            /// Setup the test context
            if (SetupCurrentTest(*listp))
            {
                RunCurrentTest(*listp); /// Run test
                TearDownCurrentTest(*listp);  /// Tear down test context
            }
            /// Repeat for all tests
            listp++;
        }
    }
    else
        ;

    ///At the end invoke reporter SuiteFinish function
    return CurrentReporter->SuiteFinish(*this);
}

/*!
  Invoke the maker function to create the test object.

  The actual test object might be derived also from a fixture. When maker
  function is called, it triggers the construction of the fixture and this
  might fail. That is why the construction is wrapped in a try...catch block.

  \return true if constructor was successful
*/
inline
bool TestSuite::SetupCurrentTest(const Inserter* inf)
{
    bool ok = false;
    try {
        CurrentTest = (inf->maker)();
        ok = true;
    }
    catch (UnitTest::test_abort& x)
    {
        std::stringstream stream;
        stream << " Aborted setup of " << inf->test_name << " - " << x.what();
        ReportFailure(x.file, x.line, stream.str());
    }
    catch (const std::exception& e)
    {
        std::stringstream stream;
        stream << "Unhandled exception: " << e.what()
            << " while setting up test " << inf->test_name;
        ReportFailure(inf->file_name, inf->line, stream.str());
    }
    catch (...)
    {
        ReportFailure(inf->file_name, inf->line, "Setup unhandled exception: Crash!");
    }
    return ok;
}

/// Run the test
inline
void TestSuite::RunCurrentTest(const Inserter* inf)
{
    assert(CurrentTest);
    CurrentReporter->TestStart(*CurrentTest);


    try {
        CurrentTest->run();
    }
    catch (UnitTest::test_abort& x)
    {
        ReportFailure(x.file, x.line, std::string("Test aborted: ") + x.what());
    }
    catch (const std::exception& e)
    {
        std::stringstream stream;
        stream << "Unhandled exception: " << e.what()
            << " while running test " << inf->test_name;
        ReportFailure(inf->file_name, inf->line, stream.str());
    }
    catch (...)
    {
        std::stringstream stream;
        stream << "Unhandled exception while running test " << inf->test_name;
        ReportFailure(inf->file_name, inf->line, stream.str());
    }

    int actual_time = CurrentTest->test_time_ms();
    if (CurrentTest->is_time_constraint() && max_runtime && actual_time > max_runtime)
    {
        std::stringstream stream;
        stream << "Global time constraint failed while running test " << inf->test_name
            << " Expected under " << max_runtime
            << "ms but took " << actual_time << "ms.";

        ReportFailure(inf->file_name, inf->line, stream.str());
    }
    CurrentReporter->TestFinish(*CurrentTest);
}

/// Delete current test instance
inline
void TestSuite::TearDownCurrentTest(const Inserter* inf)
{
    try {
        delete CurrentTest;
        CurrentTest = 0;
    }
    catch (const std::exception& e)
    {
        std::stringstream stream;
        stream << "Unhandled exception: " << e.what()
            << " while tearing down test " << inf->test_name;
        ReportFailure(inf->file_name, inf->line, stream.str());
    }
    catch (...)
    {
        std::stringstream stream;
        stream << "Unhandled exception tearing down test " << inf->test_name;
        ReportFailure(inf->file_name, inf->line, stream.str());
    }
}

/// Returns true if suite is enabled
inline
bool TestSuite::IsEnabled() const
{
    return enabled;
}

///Enables or disables this suite
inline
void TestSuite::Enable(bool on_off)
{
    enabled = on_off;
}

//------------------ TestSuite::Inserter --------------------------------------
/*!
  Constructor.
  \param suite        Suite name
  \param test         %Test name
  \param file         Filename associated with this test
  \param ln           Line number associated with this test
  \param func         Factory for test object

  Calls SuiteList::Add() to add the test to a suite.
*/
inline
TestSuite::Inserter::Inserter(const std::string& suite,
    const std::string& test,
    const std::string& file,
    int ln,
    Testmaker func)
    : test_name(test)
    , file_name(file)
    , line(ln)
    , maker(func)
{
    SuitesList::GetSuitesList().Add(suite, this);
}

//-----------------Timer member functions -------------------------------------
inline
Timer::Timer ()
  : startTime (0)
{
#ifdef _WIN32
  if (!frequency)
    ::QueryPerformanceFrequency (reinterpret_cast<LARGE_INTEGER*>(&frequency));
#else
  frequency = 1000000;
#endif
}

/// Record starting time
inline
void Timer::Start ()
{
  startTime = GetTime ();
}

/// Return elapsed time in milliseconds since the starting time
inline
int Timer::GetTimeInMs () const
{
  long long elapsedTime = GetTime () - startTime;
  double seconds = double (elapsedTime) / double (frequency);
  return int (seconds * 1000.0);
}

/// Return elapsed time in microseconds since the starting time
inline
long long Timer::GetTimeInUs () const
{
  long long int elapsedTime = GetTime () - startTime;
  double seconds = double (elapsedTime) / double (frequency);
  return (long long int) (seconds * 1000000.0);
}

inline
long long Timer::GetTime () const
{
#ifdef _WIN32
  LARGE_INTEGER curTime;
  ::QueryPerformanceCounter (&curTime);
  return curTime.QuadPart;
#else
  struct timeval currentTime;
  gettimeofday (&currentTime, 0);
  return currentTime.tv_sec * frequency + currentTime.tv_usec;
#endif
}

/// Pause current thread for the specified time
inline
void SleepMs (int const ms)
{
#ifdef _WIN32
  ::Sleep (ms);
#else
  usleep (static_cast<useconds_t>(ms * 1000));
#endif
}

//------------------TimeConstraint member functions ---------------------------

/*!
  Initializes a TimeConstraint object.
  \param ms       Maximum allowed duration in milliseconds
  \param file     Filename associated with this constraint
  \param line     Line number associated with this constraint

  The object contains a timer that is started now. It also keeps track of the
  filename and line number where it has been created.
*/
inline
TimeConstraint::TimeConstraint (int ms, const char* file, int line)
  : filename (file)
  , line_number (line)
  , max_ms (ms)
{
  timer.Start ();
}

/*!
  If the timer is greater than allowed value it records a time constraint failure
  for the current test.
*/
inline
TimeConstraint::~TimeConstraint ()
{
  int t = timer.GetTimeInMs ();
  if (t > max_ms)
  {
    std::stringstream stream;
    stream << "Time constraint failed. Expected to run test under " << max_ms <<
      "ms but took " << t << "ms.";

    ReportFailure (filename, line_number, stream.str ());
  }
}


//-------------------SuitesList member functions ------------------------------

/*!
  Add a test to a suite

  \param suite_name name of suite that will contain the test
  \param inf test information
*/
inline
void SuitesList::Add (const std::string& suite_name, const TestSuite::Inserter* inf)
{
  auto p = suites.begin ();
  while (p != suites.end () && p->name != suite_name)
    p++;

  if (p == suites.end ())
  {
    suites.emplace_back (suite_name);
    suites.back ().Add (inf);
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
inline
int SuitesList::Run (const std::string& suite_name, Reporter& reporter, int max_time_ms)
{
  for (auto p = suites.begin (); p != suites.end (); p++)
  {
    if (p->name == suite_name)
    {
      p->RunTests (reporter, max_time_ms);
      return reporter.Summary ();
    }
  }
  return -1;
}

/*!
  Run tests in all suites
  \param reporter test reporter to be used for results
  \param max_time_ms global time constraint in milliseconds

  \return total number of failed tests
*/
inline
int SuitesList::RunAll (Reporter& reporter, int max_time_ms)
{
  for (auto p = suites.begin (); p != suites.end (); p++)
    p->RunTests (reporter, max_time_ms);

  return reporter.Summary ();
}

/*!
  Accesses the singleton object.
  \return The one and only SuitesList object
*/
inline
SuitesList& SuitesList::GetSuitesList ()
{
  static SuitesList all_suites;
  return all_suites;
}

/*!
  Changes the _enabled_ state of a suite. A suite that is not enabled will not
  be run.

  \param suite name of suite to be enabled or disabled
  \param enable suite state
*/
inline
void SuitesList::Enable (const std::string& suite, bool enable)
{
  for (auto s = suites.begin (); s != suites.end (); s++)
  {
    if (s->name == suite)
    {
      s->Enable (enable);
      break;
    }
  }
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
inline
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
inline
int RunSuite (const char* suite_name, Reporter& rpt, int max_time_ms)
{
  return SuitesList::GetSuitesList ().Run (suite_name, rpt, max_time_ms);
}

inline
void DisableSuite (const std::string& suite_name)
{
  SuitesList::GetSuitesList ().Enable (suite_name, false);
}

inline
void EnableSuite (const std::string& suite_name)
{
  SuitesList::GetSuitesList ().Enable (suite_name, true);
}

/*!
  The function called by the various CHECK_... macros to record a failure.
  \param filename Name of file where the failure has occurred
  \param line     Line number where the failure has occurred
  \param message  Failure description

  It calls the TestReporter::ReportFailure function of the current reporter
  object.
*/
inline
void ReportFailure(const std::string& filename, int line, const std::string& message)
{
    if (CurrentTest)
        CurrentTest->failure();
    Failure f = { filename, message, line };
    CurrentReporter->ReportFailure(f);
}

/*!
  The function called by CHECK_FILE_EQUAL macro to compare two files.
  \param ref      Name of reference file
  \param actual   Name of output file
  \param message  Error message
*/
inline
bool CheckFileEqual(const char* ref, const char* actual, std::string& message)
{
    struct stat st1, st2;
    char buf[1024];

    stat(ref, &st1);
    stat(actual, &st2);
    if (st1.st_size != st2.st_size)
    {
        sprintf_s(buf, "Size is different (%ld vs %ld) while comparing %s and %s",
            st1.st_size, st2.st_size, ref, actual);
        message = buf;
        return false;
    }

    FILE* f1, * f2;
    f1 = fopen(ref, "r");
    f2 = fopen(actual, "r");
    if (!f1 || !f2)
    {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        sprintf_s(buf, "Failed to open files while comparing %s and %s",
            ref, actual);
        message = buf;
        return false; //something wrong with files
    }

    size_t ln = 0;
    bool ok = true;
    char ln1[1024], ln2[1024];
    while (ok)
    {
        ln++;
        if (fgets(ln1, sizeof(ln1), f1)
            && fgets(ln2, sizeof(ln2), f2))
            ok = !strcmp(ln1, ln2);
        else
            break;
    }
    fclose(f1);
    fclose(f2);
    if (!ok)
    {
        char* p1, * p2;
        int off;
        for (off = 0, p1 = ln1, p2 = ln2;
            *p1 && *p2 && *p1 == *p2;
            p1++, p2++, off++)
            ;
        sprintf_s(buf, "Difference at line %zd position %d while comparing %s and %s",
            ln, off, ref, actual);
        message = buf;
    }
    else
        message = std::string();
    return ok;
}

} // end of UnitTest namespace


/*!
  Return current suite name for default suite. All other suites have the same
  function defined inside their namespaces.
*/
inline
const char* GetSuiteName ()
{
  return DEFAULT_SUITE;
}

#include "reporter_stdout.h"
#include "reporter_dbgout.h"
#include "reporter_xml.h"
#include "checks.h"

/// Return the default reporter object. 
inline
UnitTest::Reporter& UnitTest::GetDefaultReporter ()
{
  static UnitTest::ReporterStdout the_default_reporter;
  return the_default_reporter;
}
