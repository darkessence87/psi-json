#pragma once

#include "JTypes.h"

#include <list>
#include <optional>

namespace psi::json {

class JsonParser;

/**
 * @brief 
 * 
 */
class JArray
{
    using DataType = std::list<JValue>;

public:
    /**
     * @brief Construct a new JArray object
     * 
     */
    JArray() = default;

    /**
     * @brief Construct a new JArray object
     * 
     * @param parent 
     */
    JArray(JParent parent);

    /**
     * @brief Construct a new JArray object
     * 
     * @param list 
     */
    JArray(std::initializer_list<JValue> &&list);

    /**
     * @brief 
     * 
     * @param value 
     */
    void add(JValue &&value);

    /**
     * @brief 
     * 
     * @return std::optional<std::list<JValue *>> 
     */
    std::optional<std::list<JValue *>> data();

    /**
     * @brief 
     * 
     * @return JParent 
     */
    JParent parent() const;

    /**
     * @brief 
     * 
     * @param os 
     * @return std::ostream& 
     */
    std::ostream &operator<<(std::ostream &os) const;

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string toString() const;

private:
    DataType m_data;
    JParent m_parent;

    friend class JsonParser;
};

} // namespace psi::json