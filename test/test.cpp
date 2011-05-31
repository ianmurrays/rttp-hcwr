#include "UnitTest++/UnitTest++.h"
#include "rttp_test.cpp"

/*TEST(FailSpectacularly)
{
  CHECK(false);
}

TEST(TheTruth)
{
  CHECK(true);
}*/

// -----------------------------------------------------------------------------------

#define _RTTP_TEST // Important!
int main()
{
  return UnitTest::RunAllTests();
}