#include <utpp/utpp.h>

double earth_radius_km ();
/*
  Tests belonging to the same suite can appear in different translation units.
  However a suite must be contiguous in one translation unit.
*/
SUITE (EarthSuite)
{
  TEST (EarthRadius2)
  {
    CHECK_CLOSE (6370., earth_radius_km ());
  }
}