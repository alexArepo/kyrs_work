#include "TelegramNewsBot.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

class MockTelegramNewsBot : public TelegramNewsBot {
public:
  MockTelegramNewsBot(const std::string& token, const std::string& newsApiKey)
    : TelegramNewsBot(token, newsApiKey) {}

  std::vector<std::string> fetchNews(const std::string& category = "") override {
    if (category == "sports") {
      return { "Sports News 1", "Sports News 2" };
    }
    else if (category == "business") {
      return { "Business News 1", "Business News 2" };
    }
    return {};
  }
};

class TelegramNewsBotTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  static void TearDownTestCase() {
    curl_global_cleanup();
  }
};

TEST_F(TelegramNewsBotTest, FetchNewsGeneral) {
  MockTelegramNewsBot bot("dummy_token", "dummy_api_key");
  std::vector<std::string> news = bot.fetchNews();
  EXPECT_TRUE(news.empty()) << "Expected no news for general category";
}

TEST_F(TelegramNewsBotTest, FetchNewsSports) {
  MockTelegramNewsBot bot("dummy_token", "dummy_api_key");
  std::vector<std::string> news = bot.fetchNews("sports");
  ASSERT_EQ(news.size(), 2) << "Expected 2 news items for sports category";
  EXPECT_EQ(news[0], "Sports News 1");
  EXPECT_EQ(news[1], "Sports News 2");
}

TEST_F(TelegramNewsBotTest, FetchNewsBusiness) {
  MockTelegramNewsBot bot("dummy_token", "dummy_api_key");
  std::vector<std::string> news = bot.fetchNews("business");
  ASSERT_EQ(news.size(), 2) << "Expected 2 news items for business category";
  EXPECT_EQ(news[0], "Business News 1");
  EXPECT_EQ(news[1], "Business News 2");
}

