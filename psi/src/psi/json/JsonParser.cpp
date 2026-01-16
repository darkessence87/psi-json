#include "psi/json/JsonParser.h"

#include <errno.h>
#include <fstream>
#include <stack>

#include "psi/tools/Tools.h"

#include <iostream>
#include <sstream>
#define LOG_INFO_STATIC(x)                                                                                             \
    do {                                                                                                               \
        std::cerr << x << std::endl;                                                                                   \
    } while (0)
#define LOG_ERROR_STATIC(x) LOG_INFO_STATIC(x)

namespace psi::json {

const char JsonParser::TRUE_STR[4] = {'t', 'r', 'u', 'e'};
const char JsonParser::FALSE_STR[5] = {'f', 'a', 'l', 's', 'e'};
const char JsonParser::NULL_STR[4] = {'n', 'u', 'l', 'l'};
const char JsonParser::FLOAT_STR[5] = {'.', 'e', 'E', '+', '-'};
const size_t JsonParser::NUMBER_MAX_LEN = 0x20; // 32u;     // 2^5;
const size_t JsonParser::KEY_MAX_LEN = 0x100;   // 256u;    // 2^8;
const size_t JsonParser::STR_MAX_LEN = 0x10000; // 65536u;  // 2^16;

JTree JsonParser::loadFromFile(const std::string &fName)
{
    std::ifstream f(fName);
    if (!f.is_open()) {
        LOG_ERROR_STATIC("Could not open fName: " << fName);
        return {};
    }

    if (f.eof()) {
        LOG_ERROR_STATIC("File is empty, fName: " << fName);
        f.close();
        return {};
    }

    auto jtree = parse(f);
    f.close();

    return jtree;
}

JTree JsonParser::loadFromFile(const std::wstring &fName)
{
    std::ifstream f(fName);
    if (!f.is_open()) {
        LOG_ERROR_STATIC("Could not open fName: " << tools::wstring_to_utf8(fName));
        return {};
    }

    if (f.eof()) {
        LOG_ERROR_STATIC("File is empty, fName: " << tools::wstring_to_utf8(fName));
        f.close();
        return {};
    }

    auto jtree = parse(f);
    f.close();

    return jtree;
}

JTree JsonParser::loadFromString(const std::string &str)
{
    std::istringstream ss(str, std::ios_base::binary);
    return parse(ss);
}

JTree JsonParser::parse(auto &is)
{
    std::stack<std::variant<JObject *, JArray *>> stack;

    JValue head;
    if (!parseValue(is, (JObject *)nullptr, head)) {
        return {};
    }

    bool isValidInput = false;
    std::visit(
        [&](auto &&v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<std::unique_ptr<JObject>, T>) {
                isValidInput = true;
                stack.emplace(v.get());
            } else if constexpr (std::is_same_v<std::unique_ptr<JArray>, T>) {
                isValidInput = true;
                stack.emplace(v.get());
            }
        },
        head);

    if (!isValidInput) {
        return {};
    }

    bool isFailed = false;
    while (!stack.empty()) {
        auto s = stack.top();
        stack.pop();

        std::visit(
            [&](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<JObject *, T>) {
                    isFailed = !parseObject(is, *v, stack);
                } else if constexpr (std::is_same_v<JArray *, T>) {
                    isFailed = !parseArray(is, *v, stack);
                }
            },
            s);

        if (isFailed) {
            break;
        }
    }

    if (isFailed) {
        return {};
    }

    return JTree {std::move(head)};
}

bool JsonParser::parseTrue(auto &is, JValue &value)
{
    uint8_t index = 1;
    while (index < 4) {
        char t;
        is.read(&t, 1);
        if (t != TRUE_STR[index]) {
            return false;
        }
        ++index;
    }
    value = true;
    return true;
}

bool JsonParser::parseFalse(auto &is, JValue &value)
{
    uint8_t index = 1;
    while (index < 5) {
        char t;
        is.read(&t, 1);
        if (t != FALSE_STR[index]) {
            return false;
        }
        ++index;
    }
    value = false;
    return true;
}

bool JsonParser::parseNull(auto &is, JValue &value)
{
    uint8_t index = 1;
    while (index < 4) {
        char t;
        is.read(&t, 1);
        if (t != NULL_STR[index]) {
            return false;
        }
        ++index;
    }
    value = JNull();
    return true;
}

bool JsonParser::parseNumber(auto &is, char c, JValue &value)
{
    size_t index = 0;
    char temp[NUMBER_MAX_LEN];
    temp[index++] = c;
    char t;
    bool containsFloat = false;

    auto isFloatChar = [&](char tt) {
        bool check = false;
        for (uint8_t i = 0; i < 5; ++i) {
            if (tt == FLOAT_STR[i]) {
                check = true;
                break;
            }
        }
        if (!containsFloat && check) {
            containsFloat = true;
        }
        return check;
    };

    while (!is.eof()) {
        is.read(&t, 1);
        if (std::isdigit(t) || isFloatChar(t)) {
            temp[index++] = t;
            if (index >= NUMBER_MAX_LEN) {
                return false;
            }
            continue;
        }
        is.seekg(-1, std::ios_base::cur);
        break;
    }
    temp[index++] = '\0';

    if (index == 1 && c == '-') {
        return false;
    }

    if (containsFloat) {
        value = std::strtod(temp, nullptr);
        if (errno == ERANGE) {
            errno = 0;
            // LOG_ERROR_STATIC("Failed parsing data. Error parse number value: " << temp);
        }
        return true;
    }

    if (c == '-') {
        int64_t v = std::strtoll(temp, nullptr, 10);
        if (std::abs(v) < 0x10000) {
            value = static_cast<int16_t>(v);
        } else if (std::abs(v) < 0x100000000) {
            value = static_cast<int32_t>(v);
        } else {
            value = v;
        }
    } else {
        uint64_t v = std::strtoull(temp, nullptr, 10);
        if (v < 0x10000) {
            value = static_cast<uint16_t>(v);
        } else if (v < 0x100000000) {
            value = static_cast<uint32_t>(v);
        } else {
            value = v;
        }
    }
    if (errno == ERANGE) {
        errno = 0;
        // LOG_ERROR_STATIC("Failed parsing data. Error parse number value: " << temp);
    }
    return true;
}

char JsonParser::nextChar(auto &is)
{
    char c = '\0';
    while (!is.eof()) {
        is.read(&c, 1);
        if (std::isspace(c) || c == '}' || c == ']') {
            continue;
        }
        break;
    }
    return c;
}

bool JsonParser::nextChildAvailable(auto &is)
{
    char c;
    do {
        is.read(&c, 1);
        if (std::isspace(c)) {
            continue;
        }
        return c == ',';
    } while (true);
    return false;
}

bool JsonParser::nextParentAvailable(auto &is)
{
    char c;
    do {
        is.read(&c, 1);
        if (std::isspace(c)) {
            continue;
        }
        return c == '}' || c == ']';
    } while (true);
    return false;
}

int8_t JsonParser::convertUtf8Char(uint8_t c, uint8_t result[2])
{
    result[0] = 0b10000000 | (c & 0b00111111);
    result[1] = 0b11000000 | ((c >> 6) & 0b00011111);
    return 2;
}

int8_t JsonParser::convertUtf8Char(uint16_t c, uint8_t result[3])
{
    if (c >> 8) {
        result[0] = 0b10000000 | (c & 0b00111111);
        result[1] = 0b10000000 | ((c >> 6) & 0b00111111);
        result[2] = 0b11100000 | ((c >> 12) & 0b00001111);
        return 3;
    }

    return convertUtf8Char(uint8_t(c), result);
}

template <size_t SZ>
bool JsonParser::parseString(auto &is, auto &value)
{
    if (nextChar(is) != '"') {
        return false;
    }

    char c = 0, pC = 0, ppC = 0;
    size_t index = 0;
    do {
        is.read(&c, 1);
        if (c != '"') {
            if (c < 0) {
                if (pC < 0) {
                    if (ppC >= 0) {
                        value[index++] = pC;
                        if (index >= SZ - 1) {
                            return false;
                        }
                    }
                    value[index++] = c;
                    if (index >= SZ - 1) {
                        return false;
                    }
                }
            } else if (pC < 0 && ppC >= 0) {
                if (index + 3 >= SZ - 1) {
                    return false;
                }
                uint8_t converted[2];
                int8_t len = convertUtf8Char(uint8_t(pC), converted);
                while (len > 0) {
                    value[index++] = converted[len - 1];
                    --len;
                }
                value[index++] = c;
            } else if (pC == '\\' && c == 'u') {
                if (index + 4 >= SZ - 1) {
                    return false;
                }
                uint16_t temp = 0;
                uint8_t t = 0;
                int8_t j = 3;
                while (j >= 0) {
                    is.read((char *)&t, 1);
                    if (std::isdigit(t)) {
                        temp |= (t - 0x30) << (j * 4);
                    } else if (std::isxdigit(t)) {
                        if (t < 0x61) {
                            temp |= (t - 0x37) << (j * 4);
                        } else {
                            temp |= (t - 0x57) << (j * 4);
                        }
                    }
                    --j;
                }

                --index;
                uint8_t converted[3];
                int8_t len = convertUtf8Char(temp, converted);
                while (len > 0) {
                    value[index++] = converted[len - 1];
                    --len;
                }
            } else {
                value[index++] = c;
                if (index >= SZ - 1) {
                    return false;
                }
            }
        } else {
            if (pC == '\\' && ppC != '\\') {
                if (index + 2 >= SZ - 1) {
                    return false;
                }
                value[index++] = c;
            } else {
                if (index >= SZ - 1) {
                    return false;
                }
                value[index] = '\0';
                return true;
            }
        }
        ppC = pC;
        pC = c;
    } while (!is.eof());

    return false;
}

bool JsonParser::parseValue(auto &is, JParent parent, JValue &value)
{
    auto c = nextChar(is);
    switch (c) {
    case '{':
        value = std::make_unique<JObject>(parent);
        return true;
    case '[':
        value = std::make_unique<JArray>(parent);
        return true;
    case '"': {
        is.seekg(-1, std::ios_base::cur);
        char str[STR_MAX_LEN];
        if (!parseString<STR_MAX_LEN>(is, str)) {
            return false;
        }
        value = std::move(str);
        return true;
    }
    case '-':
        return parseNumber(is, c, value);
    case 't':
        return parseTrue(is, value);
    case 'f':
        return parseFalse(is, value);
    case 'n':
        return parseNull(is, value);
    }

    if (std::isdigit(c)) {
        return parseNumber(is, c, value);
    }

    return false;
}

JParent JsonParser::getParent(JParent &parent)
{
    std::visit([&](auto &&v) { parent = v->parent(); }, parent);
    return parent;
}

bool JsonParser::parseObject(auto &is, JObject &obj, auto &stack)
{
    char key[KEY_MAX_LEN];
    if (!parseString<KEY_MAX_LEN>(is, key)) {
        return false;
    }

    if (nextChar(is) != ':') {
        return false;
    }

    JValue value;
    if (!parseValue(is, &obj, value)) {
        return false;
    }

    std::visit(
        [&](auto &&v) {
            obj.insert(key, std::move(value));
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<std::unique_ptr<JObject>, T>) {
                if (!nextParentAvailable(is)) {
                    is.seekg(-1, std::ios_base::cur);
                    stack.emplace(std::get_if<std::unique_ptr<JObject>>(obj.at(key))->get());
                }
            } else if constexpr (std::is_same_v<std::unique_ptr<JArray>, T>) {
                if (!nextParentAvailable(is)) {
                    is.seekg(-1, std::ios_base::cur);
                    stack.emplace(std::get_if<std::unique_ptr<JArray>>(obj.at(key))->get());
                }
            }
        },
        value);

    putNextObject(is, &obj, stack);
    return true;
}

bool JsonParser::parseArray(auto &is, JArray &arr, auto &stack)
{
    JValue value;
    if (!parseValue(is, &arr, value)) {
        return false;
    }

    std::visit(
        [&](auto &&v) {
            arr.add(std::move(value));
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<std::unique_ptr<JObject>, T>) {
                if (!nextParentAvailable(is)) {
                    is.seekg(-1, std::ios_base::cur);
                    stack.emplace(std::get<std::unique_ptr<JObject>>(arr.m_data.back()).get());
                }
            } else if constexpr (std::is_same_v<std::unique_ptr<JArray>, T>) {
                if (!nextParentAvailable(is)) {
                    is.seekg(-1, std::ios_base::cur);
                    stack.emplace(std::get<std::unique_ptr<JArray>>(arr.m_data.back()).get());
                }
            }
        },
        value);

    putNextObject(is, &arr, stack);
    return true;
}

void JsonParser::putNextObject(auto &is, JParent parent, auto &stack)
{
    while (true) {
        if (!nextChildAvailable(is)) {
            is.seekg(-1, std::ios_base::cur);
            if (!nextParentAvailable(is)) {
                is.seekg(-1, std::ios_base::cur);
            }
            parent = getParent(parent);

            bool needBreak = false;
            std::visit([&](auto &&v) { needBreak = v == nullptr; }, parent);
            if (needBreak) {
                break;
            }
            continue;
        }

        stack.push(parent);
        break;
    }
}

} // namespace psi::json