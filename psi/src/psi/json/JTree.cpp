
#include "psi/json/JTree.h"

#include "psi/tools/Tools.h"

namespace psi::json {

JTree::JTree(JValue &&head)
    : m_head(std::move(head))
{
}

std::optional<const JObject *> JTree::asObject()
{
    auto ptr = std::get_if<std::unique_ptr<JObject>>(&m_head);
    return ptr ? std::make_optional(ptr->get()) : std::nullopt;
}

std::optional<const JArray *> JTree::asArray()
{
    auto ptr = std::get_if<std::unique_ptr<JArray>>(&m_head);
    return ptr ? std::make_optional(ptr->get()) : std::nullopt;
}

std::string JTree::toString() const
{
    if (std::holds_alternative<std::unique_ptr<JObject>>(m_head)) {
        return std::get<std::unique_ptr<JObject>>(m_head)->toString();
    }

    if (std::holds_alternative<std::unique_ptr<JArray>>(m_head)) {
        return std::get<std::unique_ptr<JArray>>(m_head)->toString();
    }

    return "";
}

} // namespace psi::json