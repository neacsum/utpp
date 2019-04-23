/*!
  \file test.cpp
  \brief Implementation of UnitTest::Test class

  (c) Mircea Neacsu 2017
  See README file for full copyright information.
*/
#include <utpp/test.h>
#include <utpp/test_suite.h>
#include <utpp/test_reporter.h>

#include <sstream>
#include <string.h>
#include <sys/stat.h>

//#ifndef _MSC_VER
#define sprintf_s sprintf
//#endif

namespace UnitTest {

/// Constructor
Test::Test (const std::string& test_name)
  : name (test_name)
  , failures (0)
  , time (0)
{
}

Test::~Test ()
{
}

/*!
  If test is not exempted of global time constraint it starts a timer and calls
  RunImpl() to execute test code.

  When RunImpl() returns, it records the elapsed time.
*/
void Test::run ()
{
  Timer test_timer;
  test_timer.Start ();

  RunImpl ();

  time = test_timer.GetTimeInMs ();
}

/*!
  Increment failures count for this test
*/
void Test::failure ()
{
  failures++;
}

/*!
  The function called by the various CHECK_... macros to record a failure.
  \param filename Name of file where the failure has occurred
  \param line     Line number where the failure has occurred
  \param message  Failure description

  It calls the TestReporter::ReportFailure function of the current reporter
  object.
*/
void ReportFailure (const std::string& filename, int line, const std::string& message)
{
  assert (CurrentReporter);

  if (CurrentTest)
    CurrentTest->failure ();
  CurrentReporter->ReportFailure ({ filename, message, line });
}

/*!
  The function called by CHECK_FILE_EQUAL macro to compare two files.
  \param ref      Name of reference file
  \param actual   Name of output file
  \param message  Error message
*/
bool CheckFileEqual (const char* ref, const char* actual, std::string& message)
{
  struct stat st1, st2;
  char buf[1024];

  stat (ref, &st1);
  stat (actual, &st2);
  if (st1.st_size != st2.st_size)
  {
    sprintf_s (buf, "Size is different (%ld vs %ld) while comparing %s and %s",
      st1.st_size, st2.st_size, ref, actual);
    message = buf;
    return false;
  }

  FILE *f1, *f2;
  f1 = fopen (ref, "r");
  f2 = fopen (actual, "r");
  if (!f1 || !f2)
  {
    if (f1) fclose (f1);
    if (f2) fclose (f2);
    sprintf_s (buf, "Failed to open files while comparing %s and %s",
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
    bool c1 = (fgets (ln1, sizeof (ln1), f1) != 0);
    bool c2 = (fgets (ln2, sizeof (ln2), f2) != 0);
    if (c1 && c2)
      ok = !strcmp (ln1, ln2);
    else
      ok = !(c1^c2);
  }
  fclose (f1);
  fclose (f2);
  if (!ok)
  {
    char *p1, *p2;
    int off;
    for (off = 0, p1 = ln1, p2 = ln2;
      *p1 && *p2 && *p1 == *p2;
      p1++, p2++, off++)
      ;
    sprintf_s (buf, "Difference at line %zd position %d while comparing %s and %s",
      ln, off, ref, actual);
    message = buf;
  }
  else
    message = std::string();
  return ok;
}

}
