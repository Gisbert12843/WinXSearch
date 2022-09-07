#include "pch.h"
#include "conversions.h"

TEST(toFail, FailingTest) {

  EXPECT_EQ(0, 1);
  EXPECT_TRUE(false);
}

TEST(Test1, to_lower)
{
	EXPECT_EQ(to_lower_string("HaLlO1"), "hallo1");
}