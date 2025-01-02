
# Architecture

## Motivation ##
UnitTest++ is an excellent lightweight test framework but it has a number of
irritants like:

* complete lack of comments

* baroque internal structure with tests, test suites, test lists, test results, test details,
test reporters, etc.

* stylistic issues with 'const madness' like this:
  ```
  void RunTest(TestResults* const result, Test* const curTest, int const maxTestTimeInMs) const;
  ```
  (an int parameter is always const)

* the GitHub project seems dormant now.

This rewrite tried to maintain as much as possible the pubic "API" of the original
UnitTest++ but on the inside almost everything has been rewritten.

## Terminology ##
The library allows you to define test cases (called _tests_) and group those cases
in _test suites_. Test suites are executed and the results are displayed using
a _reporter_.

Included are three reporters: 
* [ReporterStream](@ref UnitTest::ReporterStream) sends results to an output stream. The derived [ReporterStdout](@ref UnitTest::ReporterStdout) sends results to `stdout`.
* [ReporterXml](@ref UnitTest::ReporterXml) generates results in an XML file
  with a structure similar to the files created by NUnit.
* [ReporterDbgout](@ref UnitTest::ReporterDbgout) writes messages to debug output
  using `OutputDebugString` (for Windows platform only)

The function GetDefaultTestReporter() returns an instance of the `ReporterStdout`
object as the default test reporter.

Throughout the execution of a test, one can verify the results and compare them with
expected results using _CHECK macros_ (see below).

## Overview ##
In its simplest form, a test is defined using the `TEST` macro similarly with a
standard function call:
```
TEST (MyFirstTest)
{
  // test code goes here
}
```
A number of things happen behind the scenes when TEST macro is invoked:

1. It defines a class called _TestMyFirstTest_ derived from
`Test` class. The new class has a method called `RunImpl` and the block of code
following the TEST macro becomes the body of the _RunImpl_ method.

2. It creates a small factory function (called `MyFirstTest_maker`) with the
following body:
  ```
  Test* MyFirstTest_maker ()
  {
    return new MyFirstTest;
  }
  ```
  We are going to call this function the _maker function_.

3. A pointer to the maker together with the name of the current test suite and
some additional information is used to create a `TestSuite::Inserter` object 
(with the name `MyFirstTest_inserter`). The current test suite has to be established
using a macro like in the following example:
  ```
  SUITE (LotsOfTests)
  {
    // tests definitions go here
  }
  ```
  If no suite has been declared, tests are by default appended to the default suite.

4. The `TestSuite::Inserter` constructor appends the newly created object to 
current test suite.

5. There is a global `SuitesList` object that is returned by GetSuitesList()
function. This object maintains a container with all currently defined suites.

The main program contains a call to `RunAllTests()` that triggers the following 
sequence of events:

1. One of the parameters to the RunAllTests() function is a TestReporter object,
either one explicitly created or a default reporter that sends all results to stdout.

2. The RunAllTests() function calls `SuitesList::RunAll()` function.

3. `SuitesList::RunAll()` iterates through the list  test suites mentioned before
and, for each suite calls the TestSuite::RunTests() function. 

4. TestSuite::RunTests() iterates through the list of tests and for each test does
the following:
  + Calls maker function to instantiate a new Test-derived object (like TestMyFirstTest).
  + Calls the Test::Run method which in turn calls the TestMyFirstTest::RunImpl.
     This is actually the test code that was placed after the TEST macro.
  + When the test has finished, the Test-derived object is deleted.

Throughout this process, different methods of the reporter are called at appropriate
moments (beginning of test suite, beginning of test, end of test, end of suite,
end of run).


## Checking Test Results ##
There are a a number of macro-definitions for testing abnormal conditions while
running a test:

`CHECK(value)`  Verifies that value is true (or not 0).

`CHECK_EQUAL(expected, actual)` Compares two values for equality.

`CHECK_CLOSE(expected, actual, tolerance)` Checks that two values are closer than
 specified tolerance.
 
`CHECK_ARRAY_EQUAL(expected, actual, count)` Compares two arrays for equality.

`CHECK_ARRAY_CLOSE(expected, actual, count, tolerance)` Checks that two arrays
 are closer than specified tolerance.
 
`CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance)` Checks that
 two matrices are within the specified tolerance.
 
`CHECK_THROW(expression, ExceptionType)` Verifies that expression throws an 
 of the given type.

`CHECK_THROW_EQUAL(expression, expected, ExceptionType)` Verifies that expression
 throws an of the given type and with the expected value.

CHECK_EQUAL and CHECK_THROW_EQUAL macros use a template function UnitTest::CheckEqual()
to compare their arguments. That means they can be used to compare any objects
that define a suitable equality operator.

## Using Test Suites ##
Tests can be grouped together in _suites_ like in the following example:
``````
SUITE (BigSuite)
{
  TEST (MyFirstTest)
  {
    // test code goes here
  }
  TEST (MySecondTest)
  {
    //another test case
  }
  // ....
}
``````
The SUITE macro defines in effect a namespace (called _SuiteBigSuite_) and makes
all tests, objects inside the namespace.

It also defines a function GetSuiteName() inside this namespace that returns the
name of the current suite. This function is called by the TEST macros to obtain
the suite name.

## Fixtures ##
In many (most) cases one needs a certain environment for executing the test and
the same environment might be reused in multiple tests. This environment is
represented by a 'fixture' class. The constructor of the fixture is responsible
for bringing up the environment. Here is an example of some tests with a fixture:
````
struct Account_fixture {
  Account_fixture () : amount_usd(100), amount_eur(0), amount_chf(0) {};

  int amount _usd;
  int amount_eur;
  int amount_chf;
}
TEST_FIXTURE (Account_fixture, TestExchangeEur)
{
  ExchangeToEur(&amount_usd, &amount_eur);
  CHECK_EQUAL (0, amount_usd);
  CHECK (amount_eur > 0);
}
TEST_FIXTURE (Account_fixture, TestExchangeChf)
{
  ExchangeToChf(&amount_usd, &amount_chf);
  CHECK_EQUAL (0, amount_usd);
  CHECK (amount_chf > 0);
}
````
At the beginning of each test, the amounts are initialized by the `Account_fixture`
constructor. Because each test object inherits from the fixture, all members
(or methods) of the fixture can be freely accessed during the test.

TEST_FIXTURE macro simply defines an object that inherits from both the fixture
and the Test object. Otherwise tests with fixtures are treated same as tests without
fixture. When the test is run the maker function invokes the object constructor 
which in turn invokes the fixture constructor (and the Test constructor).

## Global Objects ##
There are two global object pointers: `CurrentTest` and `CurrentReporter`.
They need to be global to allow calls to check macros from anywhere.
The TestSuite::RunTests function also initializes the `CurrentReporter` pointer.

In addition, the global string object `CurrentSuite` contains the name of the
currently running suite.

Unfortunately, prior to C++17, global objects cannot be easily used in C++ header-only libraries.
To solve this problem, UTPP replaces the `main` with a macro `TEST_MAIN` that can be used just like
the usual main function. Behind the scenes, `TEST_MAIN` defines all the required global objects
and generates the `main` function.

If you are using C++17 or higher, you can use the usual `main` function. In this case `TEST_MAIN`
is defined only for compatibility with previous C++ versions.

