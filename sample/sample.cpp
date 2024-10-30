/*
  UTPP - A New Generation of UnitTest++
  (c) Mircea Neacsu 2017-2023

  See LICENSE file for full copyright information.

  Sample test program for UTPP library
*/

#include <utpp/utpp.h>

/*-------------------------- Functions under test ---------------------------*/
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>

bool earth_is_round () {
  return true;
}

double earth_radius_km () {
  return 6370;
}

const char *planet_name () {
  return "Earth";
}

class flat_earth_exception : public std::exception {
public:
  const char *what () const noexcept { return "Earth is not flat!"; }
};

void go_to_end_of_earth ()
{
  throw flat_earth_exception();
}

void fibonacci (int n, std::vector<int>& numbers)
{
  if (n <= 0)
    throw std::invalid_argument ("Invalid fibonacci n");
  else if (n > 100)
    throw std::out_of_range ("Fibonacci n is too big");

  numbers.clear ();
  numbers.push_back(1);
  if (!--n)
    return;
  numbers.push_back (1);
  if (!--n)
    return;
  size_t i = 2;
  while (n--)
  {
    numbers.push_back (numbers[i - 1] + numbers[i - 2]);
    i++;
  }
}

double exchange_rate_eur = 0.85;
double exchange_rate_chf = 0.99;

void exchange_to_eur (double& usd, double& eur)
{
  eur = usd * exchange_rate_eur;
  usd = 0;
}

void exchange_to_chf (double& usd, double& chf)
{
  chf = usd *exchange_rate_chf;
  usd = 0;
}

struct Account_fixture {
  Account_fixture () : amount_usd (100), amount_eur (0), amount_chf (0) 
  {
    std::cout << "Account_fixture constructed" << std::endl;
  }

  ~Account_fixture () 
  { 
    std::cout << "Account_fixture destructed" << std::endl; 
  }

  double amount_usd;
  double amount_eur;
  double amount_chf;
};

void throw_2 ()
{
  int x = 2;
  throw x;
}

/*------------------------- End of functions under test ---------------------*/


SUITE (EarthSuite)
{
  // Example of CHECK macro
  TEST (EarthShape)
  {
    CHECK (earth_is_round ());
    EXPECT_TRUE (earth_is_round ()); //GTest form
  }

  // Example of CHECK_CLOSE macro
  TEST (HowBigIsEarth)
  {
    CHECK_CLOSE (6371., earth_radius_km ());
    CHECK_EQUAL_EX (6371.0, earth_radius_km (), "difference=%lf", fabs(6371.0-earth_radius_km ()) );
    CHECK_CLOSE_EX (6371., earth_radius_km (), 0.5, "This is an expected failure");
  }

  // Example of CHECK_EQUAL macro
  TEST (PlanetName)
  {
    char name[] = "Earth";
    CHECK_EQUAL ("Earth", planet_name ());
    EXPECT_EQ (planet_name (), "Earth"); //GTest form; notice swapped args
    //alternate specialization
    CHECK_EQUAL (name, planet_name ());
  }

  // Example of a test that fails
  TEST (Martians)
  {
    CHECK_EQUAL ("Mars", planet_name ());
  }

  // Example of CHECK_THROW macro
  TEST (EndOfTheEarth)
  {
    CHECK_THROW (go_to_end_of_earth (), flat_earth_exception);
    CHECK_THROW_EX (planet_name (), flat_earth_exception, "just testing CHECK_THROW_EX macro");
  }
}

// Example of CHECK_NAN
TEST (Should_be_NaN)
{
  auto nan = std::numeric_limits<double>::quiet_NaN ();
  CHECK_NAN (nan);
}

TEST (Not_a_NaN)
{
  long double x = 1.;
  CHECK_NAN (x); //this should fail
}

// Example of CHECK_THROW_EQUAL
TEST (CheckThrowEqual)
{
  CHECK_THROW_EQUAL (throw_2(), 2, int);

  int val = 3;
  CHECK_THROW_EQUAL_EX (throw_2 (), val, int, "Value is %d - This is expected", val);

  //Handling unexpected exceptions - logs an error
  //Shows also how small closures can become arguments to CHECK macros
  CHECK_THROW ([]() {throw std::runtime_error{ "Other exception" };}(), int);
}


// Example of CHECK_ARRAY_EQUAL
int ex[] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
TEST (Fibonacci_10)
{
  std::vector<int> expected;
  std::vector<int> fibs;
  for(size_t i=0;i<sizeof(ex)/sizeof(ex[0]);i++)
    expected.push_back(ex[i]);
  fibonacci (10, fibs);
  CHECK_ARRAY_EQUAL (&expected[0], &fibs[0], 10);

  //for vectors we can also use CHECK_EQUAL
  CHECK_EQUAL (expected, fibs);
  fibs[0] = 2;
  //this shows error message produced when vectors are different
  CHECK_EQUAL (expected, fibs);
}

// Example of CHECK_CLOSE for vectors
TEST (Vector_Close)
{
  std::vector<double> expected;//{ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
  std::vector<double> actual (10);
  for(size_t i=0;i<sizeof(ex)/sizeof(ex[0]);i++)
    expected.push_back(ex[i]);
  for (size_t i=0; i<expected.size(); i++)
  {
    actual[i] = expected[i] + (double)std::rand () / RAND_MAX / 10. - 0.05;
  }

  //for vectors we can use CHECK_CLOSE
  CHECK_CLOSE (expected, actual, 0.05);
  actual[1] += 1;
  //this shows error message produced when vectors are different
  CHECK_CLOSE (expected, actual, 0.05);
}

// Use CHECK_EQUAL and CHECK_CLOSE macros for std::array
TEST (Array_Checks)
{
  std::array<double, 10> expected { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
  std::array<double, 10> actual;
  for (size_t i = 0; i < expected.size (); i++)
  {
    actual[i] = expected[i];
  }

  CHECK_EQUAL (expected, actual);

  for (size_t i = 0; i < expected.size (); i++)
    actual [i] += (double)std::rand () / RAND_MAX / 10. - 0.05;

  CHECK_CLOSE (expected, actual, 0.05);

  //this shows error message produced when arrays are different
  CHECK_CLOSE (expected, actual);
}

// Example of CHECK_ARRAY2D_EQUAL
TEST (Array2D_Equal)
{
  int expected[10][8];
  int actual[10][8];

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      expected[i][j] = (i+1) * 10 + j;
      actual[i][j] = (i+1) * 10 + j;
    }
  }
  CHECK_ARRAY2D_EQUAL (expected, actual, 10, 8);

  //the next CHECK will fail
  actual[2][3] = 0;
  CHECK_ARRAY2D_EQUAL (expected, actual, 10, 8);
}

// Example of CHECK_ARRAY2D_CLOSE
TEST (Array2D_Close)
{
  double expected[10][8];
  double actual[10][8];

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      expected[i][j] = (i+1) * 10. + j;
      actual[i][j] = (i+1) * 10. + j + (double)std::rand () / RAND_MAX / 10. - 0.05;
    }
  }
  CHECK_ARRAY2D_CLOSE (expected, actual, 10, 8, 0.05);

  //the next CHECK will fail
  CHECK_ARRAY2D_CLOSE (expected, actual, 10, 8);
}

// CHECK_EQUAL should work for two const void* pointers, too
TEST (VoidPointer) {
  void *a = reinterpret_cast<void*>(4), *b = reinterpret_cast<void*>(4);

  CHECK_EQUAL(static_cast< void*>(a), static_cast<void*>(b));
}

// CHECK_THROW can catch a base class of the exception
TEST (AnotherException)
{
  std::vector<int> fibs;
  CHECK_THROW (fibonacci (-1, fibs), std::exception);
  CHECK_THROW (fibonacci (200, fibs), std::exception);
}

// Example of a test with a fixture
TEST_FIXTURE (Account_fixture, TestExchangeEur)
{
  exchange_to_eur (amount_usd, amount_eur);
  CHECK_EQUAL (0, amount_usd);
  CHECK (amount_eur > 0);
}

// Another test with the same fixture
TEST_FIXTURE (Account_fixture, TestExchangeChf)
{
  exchange_to_chf (amount_usd, amount_chf);
  CHECK_EQUAL (0, amount_usd);
  CHECK (amount_chf > 0);
}

TEST_FIXTURE (Account_fixture, Uncaught_exception)
{
  throw_2 ();
}

// How to use the ABORT macro
TEST_FIXTURE (Account_fixture, Test_Abort)
{
  exchange_to_chf (amount_usd, amount_chf);
  ABORT (amount_usd == 0); //this condition is true so we abort test
  printf ("Never gets here\n");
}

/// Same thing but with ABORT_EX
TEST_FIXTURE (Account_fixture, Test_AbortEx)
{
  exchange_to_chf (amount_usd, amount_chf);
  ABORT_EX (amount_usd == 0, "USD amount is 0!!");
  printf ("Never gets here\n");
}

TEST (GTest_macros)
{
std::string v1 = "v1", v2 = "v2";

  EXPECT_FALSE (v1 == v2);
  EXPECT_NE (v1, v2);
  EXPECT_LT (v1, v2);
  EXPECT_LE (v1, v2);
  EXPECT_LE (v1, v1);
  EXPECT_GT (v2, v1);
  EXPECT_GE (v2, v1);
  EXPECT_GE (v1, v1);

  bool thrown;
  ASSERT_TRUE (v1 == v1);
  try {
    thrown = false;
    ASSERT_FALSE (v1 == v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_FALSE (v1 == v2);
  try {
    thrown = false;
    ASSERT_TRUE (v1 == v2);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_EQ (v1, v1);
  try {
    thrown = false;
    ASSERT_NE (v1, v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_NE (v1, v2);
  try {
    thrown = false;
    ASSERT_EQ (v1, v2);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_LE (v1, v2);
  try {
    thrown = false;
    ASSERT_GT (v1, v2);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_LE (v1, v1);
  try {
    thrown = false;
    ASSERT_GT (v1, v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_LT (v1, v2);
  try {
    thrown = false;
    ASSERT_GE (v1, v2);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_GE (v2, v1);
  try {
    thrown = false;
    ASSERT_LT (v2, v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_GE (v1, v1);
  try {
    thrown = false;
    ASSERT_LT (v1, v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);

  ASSERT_GT (v2, v1);
  try {
    thrown = false;
    ASSERT_LE (v2, v1);
  }
  catch (UnitTest::test_abort&) {
    thrown = true;
  }
  CHECK (thrown);
}

/* Abort while setting up a fixture */
struct ImpossibleFixture {
  ImpossibleFixture () {
    ABORT_EX (true, "This fixture cannot be setup");
  }
};

TEST_FIXTURE (ImpossibleFixture, ImpossibleTest)
{
  printf ("Never gets here - fixture aborted\n");
}

/* An example of a disabled suite*/
SUITE (not_run)
{
  TEST (Fails_always)
  {
    CHECK (false);
  }
}

SUITE (time_limits)
{
  // No errors as this is less than the global time constraint
  TEST (SlowTest)
  {
    std::cout << "This suite takes about 12 seconds to run. Please be patient.\n";
    UnitTest::SleepMs (2000);
  }

  // This should trigger the global time constraint limit
  TEST (TooSlowTest)
  {
    UnitTest::SleepMs (4000);
  }

  // This breaks a local time constraint
  TEST (MustBeQuick)
  {
    UTPP_TIME_CONSTRAINT (1000);
    UnitTest::SleepMs (2000);
  }

  // Test exempt from global limit
  TEST (LetMeRun)
  {
    UTPP_TIME_CONSTRAINT_EXEMPT ();
    UnitTest::SleepMs (4000);
  }
}

SUITE (utf16_checks)
{
  TEST (conversion)
  {
    const wchar_t* yee = L"𐐷";
    std::string exp = u8"𐐷";
    auto s = UnitTest::to_utf8 (yee);
    CHECK_EQUAL (exp, s);
  }

  TEST (const_wchar)
  {
    const wchar_t *s1 {L"😁 widechar string"};
    const wchar_t *s2 {L"😒 widechar string"};
    CHECK_EQUAL (s1, s2);
    CHECK_EQUAL_EX (s1, s2, "s1 and s2 are different");
  }

  TEST (const_wstring)
  {
    const std::wstring s1{ L"😁 widechar string" };
    const std::wstring s2{ L"😒 widechar string" };
    CHECK_EQUAL (s1, s2);
    CHECK_EQUAL_EX (s1, s2, "s1 and s2 are different");
  }

  TEST (nonconst_wstring)
  {
    std::wstring s1 = L"😁 widechar string";
    std::wstring s2 = L"😒 widechar string";
    CHECK_EQUAL (s1, s2);
    CHECK_EQUAL_EX (s1, s2, "s1 and s2 are different");
  }
}

SUITE (typecasting)
{
  TEST (closeTest) {
    float fValue = 0.005f;
    CHECK_CLOSE (0.0, fValue, 0.01);

    unsigned char ucValue = 122;
    CHECK_CLOSE (123, ucValue, 3);

    long lValue = 122;
    CHECK_CLOSE (123, lValue, 3);
  }

  TEST (arrayCloseTest) {
    float arr1[] = { 1.0f, 2.0f, 3.0f };
    float* arr2 = arr1;
    CHECK_ARRAY_CLOSE (arr1, arr2, 3, 0.1f);
  }
}

TEST_MAIN (int argc, char** argv)
{
  (void)argc;
  (void)argv;
  int ret, ret1;

  //Suites can be disabled using the "DisableSuite" function
  UnitTest::DisableSuite ("not_run");
  UnitTest::DisableSuite ("time_limits"); //
  UnitTest::default_tolerance = .001;

  ret = UnitTest::RunAllTests ();
  std::cout << "RunAllTests() returned " << ret << std::endl;

  //Now we run one of the disabled suites with trace turned on.
  UnitTest::CurrentReporter->SetTrace (true);

  //Suites that are disabled are skipped by RunAllTests() but can be executed
  //by RunSuite()
  UnitTest::RunSuite ("not_run");

  std::cout << "Running again with results sent to TEST.XML file..."
    << std::endl;
  std::ofstream os ("test.xml");
  UnitTest::ReporterXml xml (os);
  UnitTest::EnableSuite ("time_limits");
  //Run tests setting a general time limit for each test
  ret1 = UnitTest::RunAllTests (xml, 3000);
  std::cout << "RunAllTests() returned " << ret1 << std::endl;

  UnitTest::CurrentReporter = &UnitTest::GetDefaultReporter ();
  // CHECK macros can also be used outside of tests. Example:
  CHECK_EQUAL (0, ret); // should fail

  const char* def = "def";
  const char* abc = "abc";
  CHECK_EQUAL (def, abc); //this one fails too

  //Expecting 18 tests to fail
  return (ret1 == 18)? 0 : 1;
}

