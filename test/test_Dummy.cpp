#include <boost/test/unit_test.hpp>
#include <pcde/Dummy.hpp>

using namespace pcde;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    pcde::DummyClass dummy;
    dummy.welcome();
}
