#pragma once

#include "JArray.h"
#include "JObject.h"

#include <optional>

namespace psi::json {

/**
 * @brief 
 * 
 */
class JTree
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
     * @param head 
     */
    JTree(const JValue &head);

    /**
     * @brief Destroy the JTree object
     * 
     */
    virtual ~JTree() = default;

    /**
     * @brief 
     * 
     * @return std::optional<JObject *> 
     */
    std::optional<JObject *> asObject();

    /**
     * @brief 
     * 
     * @return std::optional<JArray *> 
     */
    std::optional<JArray *> asArray();

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string toString() const;

private:
    JValue m_head = JObject();
};

} // namespace psi::json