#include "TestHelper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define private public
#include "psi/json/JsonParser.h"
#undef private

#include <fstream>
#include <stack>

using namespace ::testing;
using namespace psi::json;
using namespace psi::test;

TEST(JsonParserTests, loadFromFile_test1)
{
    auto tree = JsonParser::loadFromFile("test.json");
    JObject expectedTree;
    expectedTree.insert("first_name", "John");
    expectedTree.insert("last_name", "Smithærson");
    expectedTree.insert("is_alive", true);
    expectedTree.insert("age", JNumber(27));
    JObject address;
    address.insert("street_address", "21 2nd Street");
    address.insert("city", "New York");
    address.insert("state", "NY");
    address.insert("postal_code", "10021-3100");
    expectedTree.insert("address", address);
    JObject phone1;
    phone1.insert("type", "home");
    phone1.insert("number", "212 555-1234");
    JObject phone2;
    phone2.insert("type", "office");
    phone2.insert("number", "646 555-4567");
    expectedTree.insert("phone_numbers", JArray {phone1, phone2});
    expectedTree.insert("children", JArray {"Catherine", "Thomas", "Trevor"});
    expectedTree.insert("spouse", JNull());

    EXPECT_EQ(tree.toString(), expectedTree.toString());
}

TEST(JsonParserTests, loadFromFile_test2)
{
    std::ifstream f("test2_expected.json");
    std::string expectedTree;
    f.seekg(0, std::ios::end);
    expectedTree.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    expectedTree.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    auto tree2 = JsonParser::loadFromFile("test2_expected.json");
    EXPECT_EQ(tree2.toString(), expectedTree);
    auto tree1 = JsonParser::loadFromFile("test2.json");
    EXPECT_EQ(tree1.toString(), expectedTree);
}

TEST(JsonParserTests, loadFromFile_test3)
{
    std::ifstream f("test3_expected.json");
    std::string expectedTree;
    f.seekg(0, std::ios::end);
    expectedTree.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    expectedTree.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    auto tree2 = JsonParser::loadFromFile("test3_expected.json");
    EXPECT_EQ(tree2.toString(), expectedTree);
    auto tree1 = JsonParser::loadFromFile("test3.json");
    EXPECT_EQ(tree1.toString(), expectedTree);
}

TEST(JsonParserTests, loadFromFile_test4)
{
    std::ifstream f("test4_expected.json");
    std::string expectedTree;
    f.seekg(0, std::ios::end);
    expectedTree.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    expectedTree.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    auto tree2 = JsonParser::loadFromFile("test4_expected.json");
    EXPECT_EQ(tree2.toString(), expectedTree);
    auto tree1 = JsonParser::loadFromFile("test4.json");
    EXPECT_EQ(tree1.toString(), expectedTree);
}

TEST(JsonParserTests, small_dict)
{
    auto jtree = JsonParser::loadFromFile("small-dict.json");
    size_t objSz = 1;

    std::stack<JObject *> st;
    st.push(jtree.asObject().value());
    while (!st.empty()) {
        auto obj = st.top();
        st.pop();

        for (const auto &key : obj->getKeys()) {
            if (auto next = obj->getObject(key); next.has_value()) {
                ++objSz;
                st.push(next.value());
            }

            for (const auto &next : obj->getArrayObject(key)) {
                ++objSz;
                st.push(next.value());
            }
        }
    }
    EXPECT_EQ(objSz, 2846u);
}

TEST(JsonParserTests, small_dict_performance)
{
    std::ifstream f("small-dict.json");
    std::string to_parse;
    f.seekg(0, std::ios::end);
    to_parse.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    to_parse.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    constexpr size_t N = 100;
    JTree result[N];
    size_t k = 0;
    TestHelper::timeFn(
        "small-dict current", [&]() { result[k++] = JsonParser::loadFromString(to_parse); }, N);
}

TEST(JsonParserTests, medium_dict)
{
    auto jtree = JsonParser::loadFromFile("medium-dict.json");
    size_t objSz = 1;

    std::stack<JObject *> st;
    st.push(jtree.asObject().value());
    while (!st.empty()) {
        auto obj = st.top();
        st.pop();

        for (const auto &key : obj->getKeys()) {
            if (auto next = obj->getObject(key); next.has_value()) {
                ++objSz;
                st.push(next.value());
            }

            for (const auto &next : obj->getArrayObject(key)) {
                ++objSz;
                st.push(next.value());
            }
        }
    }
    EXPECT_EQ(objSz, 32297u);
}

TEST(JsonParserTests, DISABLED_medium_dict_performance)
{
    std::ifstream f("medium-dict.json");
    std::string to_parse;
    f.seekg(0, std::ios::end);
    to_parse.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    to_parse.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    constexpr size_t N = 10;
    JTree result[N];
    size_t k = 0;
    TestHelper::timeFn(
        "medium-dict current", [&]() { result[k++] = JsonParser::loadFromString(to_parse); }, N);
}

TEST(JsonParserTests, DISABLED_large_dict)
{
    auto jtree = JsonParser::loadFromFile("large-dict.json");
    size_t objSz = 1;

    std::stack<JObject *> st;
    st.push(jtree.asObject().value());
    while (!st.empty()) {
        auto obj = st.top();
        st.pop();

        for (const auto &key : obj->getKeys()) {
            if (auto next = obj->getObject(key); next.has_value()) {
                ++objSz;
                st.push(next.value());
            }

            for (const auto &next : obj->getArrayObject(key)) {
                ++objSz;
                st.push(next.value());
            }
        }
    }
    EXPECT_EQ(objSz, 154140u);
}

TEST(JsonParserTests, DISABLED_large_dict_performance)
{
    std::ifstream f("large-dict.json");
    std::string to_parse;
    f.seekg(0, std::ios::end);
    to_parse.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    to_parse.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    constexpr size_t N = 1;
    JTree result[N];
    size_t k = 0;
    TestHelper::timeFn(
        "large-dict current", [&]() { result[k++] = JsonParser::loadFromString(to_parse); }, N);
}

TEST(JsonParserTests, loadFromString)
{
    const std::string src =
        "{\"first_name\":\"F\u00e0r水ka\\u6c34s\\ufFfc\",\"last_name\":\"Smith\",\"is_alive\":true,\"age\":27,"
        "\"address\":{\"street_address\":\"21 2nd Street\",\"city\":\"New "
        "York\",\"state\":\"NY\",\"postal_code\":\"10021-3100\"},\"phone_numbers\":[{\"type\":"
        "\"home\",\"number\":\"212 555-1234\"},{\"type\":\"office\",\"number\":\"646 "
        "555-4567\"}],\"children\":[\"Catherine\",\"Thomas\",\"Trevor\"],\"spouse\":null}";
    JTree head = JsonParser::loadFromString(src);

    const std::string expected = "{\"first_name\":\"Fàr水ka水s￼\",\"last_name\":\"Smith\",\"is_alive\":true,\"age\":27,"
                                 "\"address\":{\"street_address\":\"21 2nd Street\",\"city\":\"New "
                                 "York\",\"state\":\"NY\",\"postal_code\":\"10021-3100\"},\"phone_numbers\":[{\"type\":"
                                 "\"home\",\"number\":\"212 555-1234\"},{\"type\":\"office\",\"number\":\"646 "
                                 "555-4567\"}],\"children\":[\"Catherine\",\"Thomas\",\"Trevor\"],\"spouse\":null}";
    EXPECT_EQ(head.toString(), expected);
}
