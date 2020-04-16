#include <utpp/utpp.h>

/*-------------------------- Functions under test ---------------------------*/
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>

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
  const char *what () const { return "Earth is not flat!"; }
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


using namespace UnitTest;

SUITE (EarthSuite)
{
  // Example of CHECK macro
  TEST (EarthShape)
  {
    CHECK (earth_is_round ());
  }

  // Example of CHECK_CLOSE macro
  TEST (HowBigIsEarth)
  {
    CHECK_CLOSE (6371., earth_radius_km (), 1.);
  }

  // Example of CHECK_EQUAL macro
  TEST (PlanetName)
  {
    CHECK_EQUAL ("Earth", planet_name ());
  }

  // Example of a test that fails
  TEST (Martians)
  {
    CHECK_EQUAL ("Mars", planet_name ());
  }

  // Example of CHECK_THROW macro
  TEST (EndOfTheEarth)
  {
    CHECK_THROW (flat_earth_exception, go_to_end_of_earth ());
  }
}

// Example of CHECK_THROW_EQUAL
TEST (CheckThowEqual)
{
  CHECK_THROW_EQUAL (int, 2, throw_2());
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

  actual[1] += 1;
  //this shows error message produced when arrays are different
  CHECK_CLOSE (expected, actual, 0.05);
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
      expected[i][j] = i * 10 + j;
      actual[i][j] = i * 10 + j;
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
      expected[i][j] = i * 10. + j;
      actual[i][j] = i * 10. + j + (double)std::rand () / RAND_MAX / 10. - 0.05;
    }
  }
  CHECK_ARRAY2D_CLOSE (expected, actual, 10, 8, 0.05);

  //the next CHECK will fail
  CHECK_ARRAY2D_CLOSE (expected, actual, 10, 8, 0.001);
}


// CHECK_THROW can catch a base class of the exception
TEST (AnotherException)
{
  std::vector<int> fibs;
  CHECK_THROW (std::exception, fibonacci (-1, fibs));
  CHECK_THROW (std::exception, fibonacci (200, fibs));
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
  ABORT (amount_usd);
  printf ("Never gets here");
}
/// Same thing but with ABORT_EX
TEST_FIXTURE (Account_fixture, Test_AbortEx)
{
  exchange_to_chf (amount_usd, amount_chf);
  ABORT_EX (amount_usd, "USD amount is 0!!");
  printf ("Never gets here");
}

/* Abort while setting up a fixture */
struct ImpossibleFixture {
  ImpossibleFixture () {
    ABORT_EX (0, "This fixture cannot be setup");
  }
};

TEST_FIXTURE (ImpossibleFixture, ImpossibleTest)
{
  printf ("Never gets here");
}


/* An example of a disabled suite*/
SUITE (not_run)
{
  TEST (Fails_always)
  {
    CHECK (false);
  }
}

int main (int argc, char** argv)
{
  //Suites can be disabled using the "DisableSuite" function
  DisableSuite ("not_run");

  GetDefaultReporter ().SetTrace (true);

  int ret = RunAllTests ();
  std::ofstream os ("test.xml");
  UnitTest::ReporterXml xml (os);
  UnitTest::RunAllTests (xml);

  // CHECK macros can be used outside of tests also.
  // The following check should fail
  CHECK_EQUAL (0, ret);

  const char* abc = "abc";
  const char* def = "def";
  //and this one too
  CHECK_EQUAL (def, abc);

  return ret;
}

