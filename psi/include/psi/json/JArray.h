#pragma once

#include "JTypes.h"

#include <optional>
#include <vector>

namespace psi::json {

class JsonParser;

/**
 * @brief 
 * 
 */
class JArray
{
    using DataType = std::vector<JValue>;

public:
    JArray() = default;
    ~JArray() = default;

    /**
     * @brief Construct a new JArray object
     * 
     * @param parent 
     */
    JArray(JParent parent);

    /**
     * @brief Construct a new JArray object
     * 
     * @param value 
     */
    void add(JValue &&value);

    /**
     * @brief 
     * 
     * @return std::optional<std::vector<const JValue *>>
     */
    std::optional<std::vector<const JValue *>> data() const;

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
    JArray(const JArray &) = delete;
    JArray operator=(const JArray &) = delete;
    JArray(JArray &&) = default;
    JArray &operator=(JArray &&) = default;

private:
    DataType m_data;
    JParent m_parent;

    friend class JsonParser;
};

} // namespace psi::json