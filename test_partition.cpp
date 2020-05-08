#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "partition.hpp"


using namespace testing;
using namespace std;


TEST(PartitionTest, EmptyFile)
{
  istringstream s("");
  ASSERT_TRUE(recipes(s).empty());
}
