#include <boost/test/unit_test.hpp>
#include "../src/hello.hpp"


BOOST_AUTO_TEST_CASE( initializer_populates_all_elements )
{
  float data[2];
  data[0] = 0.0;
  data[1] = 0.0;

  initialise_input_data(data, 2);

  BOOST_CHECK(data[0] != 0.0);
  BOOST_CHECK(data[0] != 0.0);
}
