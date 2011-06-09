#include "../hcwr.h"

SUITE(HCWR)
{
  TEST(Constructor)
  {
    HCWR * hcwr = new HCWR("data4.txt", 100, 4);
    delete hcwr;
  }
}