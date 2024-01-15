#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "psi/json/JTree.h"

using namespace ::testing;
using namespace psi::json;

TEST(JTypesTests, toString)
{
    const std::string expected = "{\"first_name\":\"Fàrkas\",\"last_name\":\"Smith\",\"is_alive\":true,\"age\":27,"
                                 "\"address\":{\"street_address\":\"21 2nd Street\",\"city\":\"New "
                                 "York\",\"state\":\"NY\",\"postal_code\":\"10021-3100\"},\"phone_numbers\":[{\"type\":"
                                 "\"home\",\"number\":\"212 555-1234\"},{\"type\":\"office\",\"number\":\"646 "
                                 "555-4567\"}],\"children\":[\"Catherine\",\"Thomas\",\"Trevor\"],\"spouse\":null}";
    JObject head;
    head.insert("first_name", "Fàrkas");
    head.insert("last_name", "Smith");
    head.insert("is_alive", true);
    head.insert("age", JNumber(27));
    JObject address;
    address.insert("street_address", "21 2nd Street");
    address.insert("city", "New York");
    address.insert("state", "NY");
    address.insert("postal_code", "10021-3100");
    head.insert("address", address);
    JObject phone1;
    phone1.insert("type", "home");
    phone1.insert("number", "212 555-1234");
    JObject phone2;
    phone2.insert("type", "office");
    phone2.insert("number", "646 555-4567");
    head.insert("phone_numbers", JArray {phone1, phone2});
    head.insert("children", JArray {"Catherine", "Thomas", "Trevor"});
    head.insert("spouse", JNull());

    EXPECT_EQ(head.toString(), expected);
}
