#include "TelegramNewsBot.h"
#include <iostream>

TelegramNewsBot::TelegramNewsBot(const std::string& token) : bot(token) {
  bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    this->onMessage(message);
    });
}

void TelegramNewsBot::start() {
  try {
    std::cout << "Bot is running..." << std::endl;
    TgBot::TgLongPoll longPoll(bot);
    while (true) {
      longPoll.start();
    }
  }
  catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void TelegramNewsBot::onMessage(TgBot::Message::Ptr message) {
  if (StringTools::startsWith(message->text, "/news")) {
    auto news = fetchNews();
    for (const auto& item : news) {
      bot.getApi().sendMessage(message->chat->id, item);
    }
  }
  else {
    bot.getApi().sendMessage(message->chat->id, "Send /news to get the latest news.");
  }
}

std::vector<std::string> TelegramNewsBot::fetchNews() {
  // Заглушка для примера. Реализуйте получение новостей по API.
  return {
      "News 1: Example headline",
      "News 2: Example headline",
      "News 3: Example headline"
  };
}
