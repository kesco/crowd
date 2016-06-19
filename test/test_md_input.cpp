#define BOOST_TEST_MODULE TEST_MARKDOWN_INPUT
#define BOOST_TEST_DYN_LINK

#include "biz.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

using namespace std;

struct Fixture {
  Fixture() {
    load_str_ = "+++    \n"
        "title: Android判断手机号码分属 \n"
        "+++   \n"
        "\n"
        "# Android判断手机号码分属\n"
        "\n"
        "一般来说，我们可以通过判断网络链接种类来判断手机是使用哪个运营商号码的。但是有时候不大准而且万一用户禁了网络链接就判断不了了。所以最靠谱的方案还是通过手机的IMSI码来判断。";
  }

  virtual ~Fixture() { }

  string load_str_;
};

BOOST_FIXTURE_TEST_SUITE(regex, Fixture)

  BOOST_AUTO_TEST_CASE(test_regex) {
    string md;
    string config;
    boost::regex pattern("^\\+{3}\\s*");
    boost::regex title_pattern("^title\\:");
    boost::smatch what;
    stringstream ss(load_str_);
    string line;
    int nu = 0;
    while (getline(ss, line, '\n')) {
      if (nu > 1) {
        if (line != "") {
          md += line;
          md += '\n';
        }
      } else if (boost::regex_match(line, pattern)) {
        nu += 1;
      } else if (boost::regex_search(line, what, title_pattern)) {
        config = boost::algorithm::erase_first_copy(line, what[0]);
        boost::algorithm::trim(config);
      }
    }
    string expected = "# Android判断手机号码分属\n"
        "一般来说，我们可以通过判断网络链接种类来判断手机是使用哪个运营商号码的。但是有时候不大准而且万一用户禁了网络链接就判断不了了。所以最靠谱的方案还是通过手机的IMSI码来判断。\n";
    BOOST_TEST(md == expected);
    expected = "Android判断手机号码分属";
    BOOST_TEST(config == expected);
  }

  BOOST_AUTO_TEST_CASE(test_post) {
    crowd::Post post("mobile_phone_address.md");
    post.load();
    BOOST_TEST(post.title() == "Android判断手机号码分属");
  }

BOOST_AUTO_TEST_SUITE_END()
