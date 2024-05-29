#ifndef TELEGRAMNEWSBOT_H
#define TELEGRAMNEWSBOT_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>

class TelegramNewsBot {
public:
  TelegramNewsBot(const std::string& token);
  void start();

private:
  TgBot::Bot bot;
  void onMessage(TgBot::Message::Ptr message);
  std::vector<std::string> fetchNews();
};

#endif // TELEGRAMNEWSBOT_H

