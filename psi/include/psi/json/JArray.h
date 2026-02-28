#pragma once

#include "JTypes.h"

#include <optional>
#include <vector>

namespace psi::json {

class JsonParser;

/**
 * @brief JArray represents json array in memory
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
     * @param parent parent
     */
    JArray(JParent parent);

    /**
     * @brief Construct a new JArray object
     * 
     * @param value value
     */
    void add(JValue &&value);

    /**
     * @brief Gives access to data as vector of pointers
     * 
     * @return std::optional<std::vector<const JValue *>>
     */
    std::optional<std::vector<const JValue *>> data() const;

    /**
     * @brief Gives access to parent
     * 
     * @return JParent 
     */
    JParent parent() const;

    /**
     * @brief Logger operator
     * 
     * @param os os
     * @return std::ostream& 
     */
    std::ostream &operator<<(std::ostream &os) const;

    /**
     * @brief Represents as string
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
