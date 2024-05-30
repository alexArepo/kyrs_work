#include "TelegramNewsBot.h"
#include <iostream>
#include <json/json.h>

TelegramNewsBot::TelegramNewsBot(const std::string& token, const std::string& newsApiKey)
  : bot(token), newsApiKey(newsApiKey), running(true) {
  bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    this->onMessage(message);
    });

  bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
    if (query->data == "get_news") {
      auto news = fetchNews();
      if (news.empty()) {
        bot.getApi().sendMessage(query->message->chat->id, "No news available.");
      }
      else {
        for (const auto& item : news) {
          bot.getApi().sendMessage(query->message->chat->id, item);
        }
      }
    }
    });
}

void TelegramNewsBot::start() {
  try {
    TgBot::TgLongPoll longPoll(bot);
    while (running) {
      longPoll.start();
    }
  }
  catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void TelegramNewsBot::stop() {
  running = false;
}

void TelegramNewsBot::onMessage(TgBot::Message::Ptr message) {
  if (StringTools::startsWith(message->text, "/news")) {
    std::string category;
    if (message->text.find(" ") != std::string::npos) {
      category = message->text.substr(message->text.find(" ") + 1);
    }
    auto news = fetchNews(category);
    if (news.empty()) {
      bot.getApi().sendMessage(message->chat->id, "No news available in this category.");
    }
    else {
      for (const auto& item : news) {
        try {
          bot.getApi().sendMessage(message->chat->id, item);
        }
        catch (TgBot::TgException& e) {
          std::cerr << "Error sending message: " << e.what() << std::endl;
        }
      }
    }
  }
  else if (StringTools::startsWith(message->text, "/start")) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons;
    TgBot::InlineKeyboardButton::Ptr button(new TgBot::InlineKeyboardButton);
    button->text = "Get News";
    button->callbackData = "get_news";
    buttons.push_back(button);
    keyboard->inlineKeyboard.push_back(buttons);

    bot.getApi().sendMessage(message->chat->id, "Welcome! Press the button below to get the latest news.", false, 0, keyboard);
  }
  else if (StringTools::startsWith(message->text, "/help")) {
    bot.getApi().sendMessage(message->chat->id, "Commands:\n/news - Get the latest news\n/news [category] - Get news by category (e.g., /news sports)\n/start - Start the bot\n/help - Get help\n/stop - Stop the bot");
  }
  else if (StringTools::startsWith(message->text, "/stop")) {
    bot.getApi().sendMessage(message->chat->id, "Bot is stopping...");
    stop();
  }
  else {
    bot.getApi().sendMessage(message->chat->id, "Unknown command. Send /help for a list of commands.");
  }
}

std::vector<std::string> TelegramNewsBot::fetchNews(const std::string& category) {
  std::vector<std::string> newsHeadlines;
  std::string readBuffer;
  CURL* curl = curl_easy_init();

  if (curl) {
    std::string url = "https://newsapi.org/v2/top-headlines?country=us&apiKey=" + newsApiKey;
    if (!category.empty()) {
      url += "&category=" + category;
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: TelegramNewsBot/1.0");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
      return newsHeadlines;
    }

    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonData;
    std::string errs;

    std::istringstream ss(readBuffer);
    if (!Json::parseFromStream(readerBuilder, ss, &jsonData, &errs)) {
      std::cerr << "Failed to parse JSON: " << errs << std::endl;
      return newsHeadlines;
    }

    if (jsonData["status"].asString() != "ok") {
      std::cerr << "Error: API returned status " << jsonData["status"].asString() << std::endl;
      return newsHeadlines;
    }

    for (const auto& article : jsonData["articles"]) {
      std::string title = article["title"].asString();
      std::string url = article["url"].asString();
      std::string newsItem = title + "\n" + url;
      newsHeadlines.push_back(newsItem);
    }
  }

  return newsHeadlines;
}

size_t TelegramNewsBot::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
  size_t newLength = size * nmemb;
  s->append((char*)contents, newLength);
  return newLength;
}
