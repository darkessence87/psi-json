#include "psi/test/psi_mock.h"

#include <sstream>

#include "psi/json/JArray.h"
#include "psi/json/JObject.h"
#include "psi/json/JTree.h"

using namespace psi::json;
using namespace psi::test;

TEST(JTypesTests, toString)
{
    const std::string expected = "{\"first_name\":\"Fàrkas\",\"last_name\":\"Smith\",\"is_alive\":true,\"age\":27,"
                                 "\"address\":{\"street_address\":\"21 2nd Street\",\"city\":\"New "
                                 "York\",\"state\":\"NY\",\"postal_code\":\"10021-3100\"},\"phone_numbers\":[{\"type\":"
                                 "\"home\",\"number\":\"212 555-1234\"},{\"type\":\"office\",\"number\":\"646 "
                                 "555-4567\"}],\"children\":[\"Catherine\",\"Thomas\",\"Trevor\"],\"spouse\":null}";
    auto head = std::make_unique<JObject>();
    head->insert("first_name", "Fàrkas");
    head->insert("last_name", "Smith");
    head->insert("is_alive", true);
    head->insert("age", JNumber(27));
    auto address = std::make_unique<JObject>();
    address->insert("street_address", "21 2nd Street");
    address->insert("city", "New York");
    address->insert("state", "NY");
    address->insert("postal_code", "10021-3100");
    head->insert("address", std::move(address));
    auto phone1 = std::make_unique<JObject>();
    phone1->insert("type", "home");
    phone1->insert("number", "212 555-1234");
    auto phone2 = std::make_unique<JObject>();
    phone2->insert("type", "office");
    phone2->insert("number", "646 555-4567");
    auto phone_numbers = std::make_unique<JArray>();
    phone_numbers->add(std::move(phone1));
    phone_numbers->add(std::move(phone2));
    head->insert("phone_numbers", std::move(phone_numbers));
    auto children = std::make_unique<JArray>();
    children->add("Catherine");
    children->add("Thomas");
    children->add("Trevor");
    head->insert("children", std::move(children));
    head->insert("spouse", JNull());

    EXPECT_EQ(head->toString(), expected);
}

TEST(JTypesTests, JObject_at_get_missing_key)
{
    JObject obj;
    obj.insert("x", JNumber(int32_t{1}));

    EXPECT_EQ(obj.at("missing"), nullptr);
    EXPECT_FALSE(obj.get("missing").has_value());
    EXPECT_TRUE(obj.get("x").has_value());
}

TEST(JTypesTests, JObject_getKeys)
{
    JObject obj;
    obj.insert("a", JNumber(int32_t{1}));
    obj.insert("b", JNumber(int32_t{2}));
    auto keys = obj.getKeys();
    EXPECT_EQ(keys.size(), size_t{2});
}

TEST(JTypesTests, JObject_data_and_parent)
{
    JObject parent_obj;
    JObject child(static_cast<JObject *>(&parent_obj));
    child.insert("k", JNumber(int32_t{7}));
    EXPECT_EQ(child.data().size(), size_t{1});
    EXPECT_TRUE(std::holds_alternative<JObject *>(child.parent()));
}

TEST(JTypesTests, JObject_getArray_missing_and_present)
{
    JObject obj;
    EXPECT_FALSE(obj.getArray("nope").has_value());

    auto arr = std::make_unique<JArray>();
    arr->add(JString(std::string{"hello"}));
    obj.insert("arr", std::move(arr));
    EXPECT_TRUE(obj.getArray("arr").has_value());
}

TEST(JTypesTests, JObject_getArray_wrong_type)
{
    JObject obj;
    obj.insert("x", JNumber(int32_t{1}));
    // at("x") returns non-null, but get_if<JArray> fails -> nullopt
    EXPECT_FALSE(obj.getArray("x").has_value());
}

TEST(JTypesTests, JObject_getObject_missing_and_present)
{
    JObject obj;
    EXPECT_FALSE(obj.getObject("nope").has_value());

    auto child = std::make_unique<JObject>();
    child->insert("n", JNumber(int32_t{0}));
    obj.insert("child", std::move(child));
    EXPECT_TRUE(obj.getObject("child").has_value());
}

TEST(JTypesTests, JObject_getObject_wrong_type)
{
    JObject obj;
    obj.insert("x", JNumber(int32_t{1}));
    EXPECT_FALSE(obj.getObject("x").has_value());
}

TEST(JTypesTests, JObject_getArrayObject_missing)
{
    JObject obj;
    auto result = obj.getArrayObject("nope");
    EXPECT_EQ(result.size(), size_t{0});
}

TEST(JTypesTests, JObject_getArrayObject_wrong_type)
{
    JObject obj;
    obj.insert("x", JNumber(int32_t{1}));
    auto result = obj.getArrayObject("x");
    EXPECT_EQ(result.size(), size_t{0});
}

TEST(JTypesTests, JObject_getArrayObject_present)
{
    JObject obj;
    auto arr = std::make_unique<JArray>();
    auto child = std::make_unique<JObject>();
    child->insert("v", JNumber(int32_t{42}));
    arr->add(std::move(child));
    obj.insert("arr", std::move(arr));

    auto result = obj.getArrayObject("arr");
    EXPECT_EQ(result.size(), size_t{1});
    EXPECT_TRUE(result[0].has_value());
}

TEST(JTypesTests, JObject_getArrayObject_non_object_breaks)
{
    // Array with a non-object element → loop breaks after first non-object
    JObject obj;
    auto arr = std::make_unique<JArray>();
    arr->add(JString(std::string{"string_elem"}));
    obj.insert("arr", std::move(arr));

    auto result = obj.getArrayObject("arr");
    EXPECT_EQ(result.size(), size_t{0});
}

TEST(JTypesTests, JObject_getNumbers)
{
    JObject obj;
    obj.insert("i64",  JNumber(int64_t{100}));
    obj.insert("i32",  JNumber(int32_t{200}));
    obj.insert("i16",  JNumber(int16_t{300}));
    obj.insert("u64",  JNumber(uint64_t{400}));
    obj.insert("u32",  JNumber(uint32_t{500}));
    obj.insert("u16",  JNumber(uint16_t{600}));
    obj.insert("dbl",  JNumber(double_t{1.5}));

    EXPECT_EQ(obj.getNumberInt64("i64"),   int64_t{100});
    EXPECT_EQ(obj.getNumberInt32("i32"),   int32_t{200});
    EXPECT_EQ(obj.getNumberInt16("i16"),   int32_t{300});
    EXPECT_EQ(obj.getNumberUInt64("u64"),  uint64_t{400});
    EXPECT_EQ(obj.getNumberUInt32("u32"),  uint32_t{500});
    EXPECT_EQ(obj.getNumberUInt16("u16"),  uint32_t{600});
    EXPECT_EQ(obj.getNumberDouble("dbl"),  double_t{1.5});
}

TEST(JTypesTests, JObject_getNumber_missing_key)
{
    JObject obj;
    EXPECT_EQ(obj.getNumberInt64("x"),  int64_t{0});
    EXPECT_EQ(obj.getNumberInt32("x"),  int32_t{0});
    EXPECT_EQ(obj.getNumberInt16("x"),  int32_t{0});
    EXPECT_EQ(obj.getNumberUInt64("x"), uint64_t{0});
    EXPECT_EQ(obj.getNumberUInt32("x"), uint32_t{0});
    EXPECT_EQ(obj.getNumberUInt16("x"), uint32_t{0});
    EXPECT_EQ(obj.getNumberDouble("x"), double_t{0});
}

TEST(JTypesTests, JObject_getNumber_wrong_type)
{
    JObject obj;
    obj.insert("s", JString(std::string{"hello"}));
    EXPECT_EQ(obj.getNumberInt64("s"),  int64_t{0});
    EXPECT_EQ(obj.getNumberDouble("s"), double_t{0});
}

TEST(JTypesTests, JObject_getString_and_getStringW)
{
    JObject obj;
    obj.insert("str", JString(std::string{"world"}));
    EXPECT_EQ(obj.getString("str"), std::string{"world"});
    EXPECT_EQ(obj.getString("missing"), std::string{});
    EXPECT_EQ(obj.getStringW("str"), std::wstring{L"world"});
}

TEST(JTypesTests, JObject_getString_wrong_type)
{
    JObject obj;
    obj.insert("n", JNumber(int32_t{1}));
    EXPECT_EQ(obj.getString("n"), std::string{});
    EXPECT_EQ(obj.getStringW("n"), std::wstring{});
}

TEST(JTypesTests, JObject_getBool)
{
    JObject obj;
    obj.insert("t", true);
    obj.insert("f", false);
    obj.insert("n", JNumber(int32_t{1}));

    EXPECT_EQ(obj.getBool("t"), true);
    EXPECT_EQ(obj.getBool("f"), false);
    EXPECT_EQ(obj.getBool("missing"), false);
    EXPECT_EQ(obj.getBool("n"), false); // wrong type
}

TEST(JTypesTests, JObject_getArrayNumberInt64)
{
    JObject obj;
    auto arr = std::make_unique<JArray>();
    arr->add(JNumber(int64_t{10}));
    arr->add(JNumber(int64_t{20}));
    obj.insert("nums", std::move(arr));

    auto result = obj.getArrayNumberInt64("nums");
    EXPECT_EQ(result.size(), size_t{2});
    EXPECT_EQ(result[0], int64_t{10});
    EXPECT_EQ(result[1], int64_t{20});

    // missing key
    EXPECT_EQ(obj.getArrayNumberInt64("x").size(), size_t{0});
    // wrong type (not array)
    obj.insert("nope", JNumber(int32_t{1}));
    EXPECT_EQ(obj.getArrayNumberInt64("nope").size(), size_t{0});
}

TEST(JTypesTests, JObject_getArrayString)
{
    JObject obj;
    auto arr = std::make_unique<JArray>();
    arr->add(JString(std::string{"alpha"}));
    arr->add(JString(std::string{"beta"}));
    obj.insert("strs", std::move(arr));

    auto result = obj.getArrayString("strs");
    EXPECT_EQ(result.size(), size_t{2});
    EXPECT_EQ(result[0], std::string{"alpha"});

    // wide version
    auto resultW = obj.getArrayStringW("strs");
    EXPECT_EQ(resultW.size(), size_t{2});
    EXPECT_EQ(resultW[0], std::wstring{L"alpha"});
}

TEST(JTypesTests, JObject_operator_stream)
{
    JObject obj;
    obj.insert("k", JNumber(int32_t{1}));
    std::ostringstream os;
    obj.operator<<(os);
    EXPECT_EQ(os.str(), std::string{"{\"k\":1}"});
}

TEST(JTypesTests, JTree_asArray_present_and_nullopt)
{
    // head is JArray → asArray returns value, asObject returns nullopt
    auto arr = std::make_unique<JArray>();
    arr->add(JString(std::string{"x"}));
    JTree tree(std::move(arr));

    EXPECT_TRUE(tree.asArray().has_value());
    EXPECT_FALSE(tree.asObject().has_value());
}

TEST(JTypesTests, JTree_asObject_nullopt_for_non_object)
{
    // head is JString → neither asObject nor asArray should return value
    JTree tree(JString(std::string{"hello"}));
    EXPECT_FALSE(tree.asObject().has_value());
    EXPECT_FALSE(tree.asArray().has_value());
}

TEST(JTypesTests, JTree_toString_non_object_non_array)
{
    // head is JNull → toString returns empty string
    JValue v = JNull{};
    JTree tree(std::move(v));
    EXPECT_EQ(tree.toString(), std::string{});
}
