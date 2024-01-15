#pragma once

#include "JTree.h"

namespace psi::json {

/**
 * @brief 
 * 
 */
class JsonParser
{
public:
    /**
     * @brief 
     * 
     * @param fName 
     * @return JTree 
     */
    static JTree loadFromFile(const std::string &fName);

    /**
     * @brief 
     * 
     * @param fName 
     * @return JTree 
     */
    static JTree loadFromFile(const std::wstring &fName);

    /**
     * @brief 
     * 
     * @param str 
     * @return JTree 
     */
    static JTree loadFromString(const std::string &str);

private:
    static JTree parse(auto &is);
    static bool parseTrue(auto &is, JValue &value);
    static bool parseFalse(auto &is, JValue &value);
    static bool parseNull(auto &is, JValue &value);
    static bool parseNumber(auto &is, char c, JValue &value);
    static char nextChar(auto &is);
    static bool nextChildAvailable(auto &is);
    static bool nextParentAvailable(auto &is);
    static int8_t convertUtf8Char(uint8_t c, uint8_t result[2]);
    static int8_t convertUtf8Char(uint16_t c, uint8_t result[3]);
    template <size_t SZ>
    static bool parseString(auto &is, auto &value);
    static bool parseValue(auto &is, JParent parent, JValue &value);
    static JParent getParent(JParent &parent);
    static bool parseObject(auto &is, JObject &obj, auto &stack);
    static bool parseArray(auto &is, JArray &arr, auto &stack);
    static void putNextObject(auto &is, JParent object, auto &stack);

private:
    static const char TRUE_STR[4];
    static const char FALSE_STR[5];
    static const char NULL_STR[4];
    static const char FLOAT_STR[5];
    static const size_t NUMBER_MAX_LEN;
    static const size_t KEY_MAX_LEN;
    static const size_t STR_MAX_LEN;
};

} // namespace psi::json