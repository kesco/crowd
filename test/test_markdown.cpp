#define BOOST_TEST_MODULE TEST_MARKDOWN
#define BOOST_TEST_DYN_LINK

#include "parser.hpp"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <memory>

using namespace std;
using namespace crowd;

struct Fixture {
  Fixture() {
    parser_ = make_unique<MarkdownParser>();
    expected_html_ =
        "<h1>Markdown Test</h1>\n"
        "\n"
        "<p>Test for the crow::MarkdownParser.\n"
        "See below.</p>\n"
        "\n"
        "<h2>Header 2</h2>\n"
        "\n"
        "<p>Parse Code.</p>\n"
        "\n"
        "<p>See the code below.</p>\n"
        "\n"
        "<p><code>shell\n"
        "echo $PATH\n"
        "</code></p>\n";
  }

  virtual ~Fixture() {}

  unique_ptr<Parser> parser_;
  string expected_html_;
};

BOOST_FIXTURE_TEST_SUITE(markdown, Fixture)

/* 测试内存字符串装换 */
BOOST_AUTO_TEST_CASE(memory_str_parse) {
  string parsed_str = parser_->parse(
      "# Markdown Test\n"
      "\n"
      "Test for the crow::MarkdownParser.\n"
      "See below.\n"
      "\n"
      "## Header 2\n"
      "\n"
      "Parse Code.\n"
      "\n"
      "See the code below.\n"
      "\n"
      "```shell\n"
      "echo $PATH\n"
      "```");
  BOOST_TEST(expected_html_ == parsed_str);
}

BOOST_AUTO_TEST_CASE(file_str_parse) {
  ifstream file("test_markdown.md");
  string parsed = parser_->parse(file);
  BOOST_TEST(expected_html_ == parsed);
}

BOOST_AUTO_TEST_SUITE_END()