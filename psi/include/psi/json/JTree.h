#pragma once

#include "JArray.h"
#include "JObject.h"

#include <optional>

namespace psi::json {

/**
 * @brief JTree
 * 
 */
class JTree final
{
public:
    /**
     * @brief Construct a new JTree object
     * 
     */
    JTree() = default;

    /**
     * @brief Construct a new JTree object
     * 
     * @param head head
     */
    JTree(JValue &&head);

    JTree(const JTree &) = delete;
    JTree operator=(const JTree &) = delete;
    JTree(JTree &&) = default;
    JTree &operator=(JTree &&) = default;

    /**
     * @brief asObject
     * 
     * @return std::optional<JObject *> 
     */
    std::optional<const JObject *> asObject();

    /**
     * @brief asArray
     * 
     * @return std::optional<JArray *> 
     */
    std::optional<const JArray *> asArray();

    /**
     * @brief toString
     * 
     * @return std::string 
     */
    std::string toString() const;

private:
    JValue m_head;
};

} // namespace psi::json
