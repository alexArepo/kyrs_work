#ifndef TELEGRAMNEWSBOT_H
#define TELEGRAMNEWSBOT_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <atomic>

class TelegramNewsBot {
public:
  TelegramNewsBot(const std::string& token, const std::string& newsApiKey);
  void start();
  void stop();

  virtual std::vector<std::string> fetchNews(const std::string& category = "");

private:
  TgBot::Bot bot;
  std::string newsApiKey;
  std::atomic<bool> running;
  void onMessage(TgBot::Message::Ptr message);
  static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

#endif // TELEGRAMNEWSBOT_H
