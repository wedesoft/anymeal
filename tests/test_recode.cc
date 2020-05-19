#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "recode.hh"


using namespace testing;
using namespace std;

TEST(RecodeTest, Constructor) {
  Recoder r("latin1..utf8");
}

TEST(RecodeTest, RecodeString) {
  Recoder r("latin1..utf8");
  string s("\xc4pfel");
  EXPECT_EQ("Äpfel", r.process(s));
}

TEST(RecodeTest, RecodeException) {
  Recoder r("latin1..ascii");
  string s("\xc4pfel");
  EXPECT_THROW(r.process(s), recode_exception);
}
