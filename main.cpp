 #include "TelegramNewsBot.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  if (result == 0) {
    std::string token = "7188455874:AAE0zm4N88nmEGYwqlwoi59n8479CNMbR3g";
    std::string newsApiKey = "e93c7a5b3bda4beeb899dde7c2c882bd";
    TelegramNewsBot bot(token, newsApiKey);
    bot.start();
    return 0;
  }
}

