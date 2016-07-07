#define BOOST_TEST_MODULE TEST_BOOST
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/date_time.hpp>

namespace bg = boost::gregorian;
using namespace std;

BOOST_AUTO_TEST_SUITE(boost)

  BOOST_AUTO_TEST_CASE(date) {
    string date_str("2016-07-16");
    bg::date d(bg::from_simple_string(date_str));
    std::cout << bg::to_iso_extended_string(d) << std::endl;
    try {
      date_str = "2016-07-19";
      bg::date dn(bg::from_simple_string(date_str));
      std::cout << bg::to_iso_extended_string(dn) << std::endl;
      if (dn > d) {
        cout << "Yes" << endl;
      }
    } catch (std::exception &ex) {
      cerr << ex.what() << endl;
    }
  }

BOOST_AUTO_TEST_SUITE_END()
