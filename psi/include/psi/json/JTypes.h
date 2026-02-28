#pragma once

#include <memory>
#include <string>
#include <variant>

namespace psi::json {

/**
 * @brief JNull
 * 
 */
using JNull = std::nullptr_t;

/**
 * @brief JNumber
 * 
 */
using JNumber = std::variant<int64_t, int32_t, int16_t, uint64_t, uint32_t, uint16_t, double_t, float_t>;

/**
 * @brief JString
 * 
 */
using JString = std::variant<std::wstring, std::string>;

class JObject;
class JArray;

/**
 * @brief JKey
 * 
 */
using JKey = std::string;

/**
 * @brief JValue
 * 
 */
using JValue = std::variant<JNull, JNumber, JString, bool, std::unique_ptr<JObject>, std::unique_ptr<JArray>>;

/**
 * @brief JParent
 * 
 */
using JParent = std::variant<JObject *, JArray *>;

/**
 * @brief operator<<
 * 
 * @param os os
 * @param value value
 * @return std::ostream& 
 */
std::ostream &operator<<(std::ostream &os, const JNull &value);

/**
 * @brief operator<<
 * 
 * @param os os
 * @param value value
 * @return std::ostream& 
 */
std::ostream &operator<<(std::ostream &os, const JNumber &value);

/**
 * @brief operator<<
 * 
 * @param os os
 * @param value value
 * @return std::ostream& 
 */
std::ostream &operator<<(std::ostream &os, const JString &value);

/**
 * @brief operator<<
 * 
 */
template <class>
inline constexpr bool always_false_v = false;

} // namespace psi::json
