/**
 * @file gtest_async.cpp
 *
 * @brief test async
 *  */

#include "async.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>


namespace {

   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)


  std::string get_is(const std::string &path_in, time_t t)
  {
    testing::internal::CaptureStdout();
    BulkAsync ba;
    auto handler = ba.connect(3, [&t](){ return t++;});

    std::ifstream ifsin(path_in);
    EXPECT_TRUE(ifsin.is_open());
    for(std::string line; std::getline(ifsin, line);) {
      ba.receive(handler, line.data(), line.size());
    }
    EXPECT_FALSE(ifsin.bad());
    ba.disconnect(handler);
    ba.closeAll();
    return testing::internal::GetCapturedStdout();
  }

  std::string get_must(const std::string &path_out)
  {
    std::string output_must;
    std::ifstream ifsout(path_out);
    EXPECT_TRUE(ifsout.is_open());
    for(std::string line; std::getline(ifsout, line);) {
      output_must += line + "\n";
    }
    EXPECT_FALSE(ifsout.bad());
    return output_must;
  }

  TEST(async, input1) {
    {
      std::filesystem::remove("bulk10000.log");
      std::filesystem::remove("bulk10001.log");
      EXPECT_EQ(get_is("testinput1.txt", 10000),get_must("testoutput1.txt"));
      EXPECT_TRUE(std::filesystem::exists("bulk10000.log"));
      EXPECT_TRUE(std::filesystem::exists("bulk10001.log"));
    }
  }

  TEST(async, input2) {
    {
      std::filesystem::remove("bulk20000.log");
      std::filesystem::remove("bulk20001.log");
      std::filesystem::remove("bulk20002.log");
      EXPECT_EQ(get_is("testinput2.txt", 20000),get_must("testoutput2.txt"));
      EXPECT_TRUE(std::filesystem::exists("bulk20000.log"));
      EXPECT_TRUE(std::filesystem::exists("bulk20001.log"));
      EXPECT_TRUE(std::filesystem::exists("bulk20002.log"));
    }
  }
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}
