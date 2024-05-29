#include "TelegramNewsBot.h"

int main() {
  std::string token = "7188455874:AAE0zm4N88nmEGYwqlwoi59n8479CNMbR3g";
  TelegramNewsBot bot(token);
  bot.start();
  return 0;
}
