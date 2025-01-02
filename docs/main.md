\mainpage Light C++ Unit Test Framework
This is a new version of the excellent [UnitTest++](https://github.com/unittest-cpp) 
Some of the reasons for making this new version are detailed in the
[architecture document](architecture.md).

Latest version can be downloaded from [GitHub](https://neacsum.github.com/utpp).

## Installation
This is a headers only library. There is nothing to build. 

## Usage
### Getting Started
Here is a simple example of a test program:
```
#include <utpp/utpp.h>

bool earth_is_round ();
double earth_radius_km ();

TEST (EarthShape)
{
  CHECK (earth_is_round ());
}

TEST (HowBigIsEarth)
{
  CHECK_CLOSE (6371., earth_radius_km(), 1.);
}

TEST_MAIN (int argc, char** argv)
{
  return UnitTest::RunAllTests ();
}
```
The program contains two tests: one that checks if the `earth_is_round` function returns `true` and another one that checks if the `earth_radius_km` function is close enough to the expected value.

The main program is defined using the #TEST_MAIN macro. It runs all the tests and, if all goes well, returns 0.

Tests are introduced by the #TEST macro followed by a block of code. Throughout the test you can check different conditions using one of the CHECK_... macros.

The example above showed two of these macros: CHECK verifies that a condition is true, while CHECK_CLOSE verifies that two values are closer than a specified limit.

Here is another example:
```
const char *planet_name () {
  return "Earth";
}

TEST (PlanetName)
{
  CHECK_EQUAL ("Earth", planet_name ());
}

```
The CHECK_EQUAL macro verifies that two values compare for equality. It can
compare any values for which an equality operator is defined. 

You can also test if an exception is thrown:
```
class flat_earth_exception : public std::exception {
public:
  const char *what () { return "Earth is not flat!"; }
};

void go_to_end_of_earth ()
{
  throw flat_earth_exception();
}

TEST (EndOfTheEarth)
{
  CHECK_THROW (go_to_end_of_earth (), flat_earth_exception);
}
```

### Grouping Tests
Tests can be defined in different source files. The magic behind UTPP will make sure that they are all executed but it doesn't make any promise as to the order of execution between different files. If you need some kind of ordering, you can group tests in _suites_. UTPP will execute all the tests in one suite before starting another. Suites can also be used to get synthetic results for a group of tests.

Example:
```
SUITE (MyNiceTests)
{
// ... many tests here
}
```

> [!note] 
> Test names must be unique within a suite.

> [!warning]
> A suite must be contiguous in a translation unit (source file). You can however have tests belonging to the same suite in different translation units.

### Fixtures
When performing test you need certain objects and values to be in a known state before the beginning of the test. This is called a _fixture_. In UTPP any object with a default constructor can be used as a fixture. Your tests will be derived from that object and the state of the object is defined by the fixture constructor.

Example:
```
void exchange_to_eur (double& usd, double& eur);

struct Account_fixture {
  Account_fixture () : amount_usd (100), amount_eur (0), amount_chf (0) {}
  ~Account_fixture () {}

  double amount_usd;
  double amount_eur;
  double amount_chf;
};

TEST_FIXTURE (Account_fixture, TestExchangeEur)
{
  exchange_to_eur (amount_usd, amount_eur);
  CHECK_EQUAL (0, amount_usd);
  CHECK (amount_eur > 0);
}
```
A test that uses a fixture is defined using a TEST_FIXTURE macro that takes as arguments the name of the fixture and the name of the test. The fixture constructor is invoked right before the beginning of the test and it insures that `amount_usd` is set to 100. Because the test object is derived from the fixture object any public or protected members of the fixture are directly available in the test body.

When the test finishes, the fixture destructor gets called and can release any resources allocated by the constructor.

### Aborting a Test
If something goes terribly wrong in a test, the execution can be aborted using the ABORT or ABORT_EX macros. They work exactly like CHECK and CHECK_EX macros but, once triggered, the remaining test is abandoned. 

Example:
````
TEST_FIXTURE (Account_fixture, Test_Abort)
{
  exchange_to_chf (amount_usd, amount_chf);
  ABORT (amount_usd);
  printf ("Never gets here");
}
````

### Results
UTPP results are produced by an object called a _Reporter_. There can be different type of reporters depending on where the output must be directed. By default The RunAllTests() function uses a reporter that sends results to `stdout`. The library provides two other reporters: one that generates the output using the `OutputDebugString` function and one that sends results to an XML file with a structure similar to the files created by NUnit.

Here is an example how to use the XML reporter:
```
TEST_MAIN (int argc, char **argv)
{
  std::ofstream os ("tests.xml");
  UnitTest::ReporterXml xml (os);
  UnitTest::RunAllTests (xml);
}
```
### Execution Control
While in most cases `RunAllTests`` function is all that is needed to execute all tests, there may be cases where you need a finer control. The UnitTest::RunSuite()
function allows you to run one particular suite while the UnitTest::DisbleSuite() allows you to disable a suite.

---

<div>Icon made from <a href="https://www.onlinewebfonts.com/icon">svg icons</a> is licensed by CC BY 4.0</div>
