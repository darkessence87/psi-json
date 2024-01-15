
#include "psi/json/JTree.h"

#include "psi/tools/Tools.h"

namespace psi::json {

JTree::JTree(const JValue &head)
    : m_head(head)
{
}

std::optional<JObject *> JTree::asObject()
{
    return std::make_optional(std::get_if<JObject>(&m_head));
}

std::optional<JArray *> JTree::asArray()
{
    return std::make_optional(std::get_if<JArray>(&m_head));
}

std::string JTree::toString() const
{
    if (std::holds_alternative<JObject>(m_head)) {
        return std::get<JObject>(m_head).toString();
    }

    if (std::holds_alternative<JArray>(m_head)) {
        return std::get<JArray>(m_head).toString();
    }

    return "";
}

} // namespace psi::json