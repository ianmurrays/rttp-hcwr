#define _RTTP_TEST // Important!

#include "UnitTest++/UnitTest++.h"
#include "rttp_test.cpp"
#include "hcwr_test.cpp"

/*TEST(FailSpectacularly)
{
  CHECK(false);
}

TEST(TheTruth)
{
  CHECK(true);
}*/

// -----------------------------------------------------------------------------------

int main()
{
  return UnitTest::RunAllTests();
}