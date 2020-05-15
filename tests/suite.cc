#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace testing;

int main(int argc, char *argv[]) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
