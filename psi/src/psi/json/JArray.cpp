#include "psi/json/JArray.h"
#include "psi/json/JObject.h"

#include <sstream>

namespace psi::json {

JArray::JArray(JParent parent)
    : m_parent(parent)
{
}

void JArray::add(JValue &&value)
{
    m_data.emplace_back(std::forward<JValue>(value));
}

std::optional<std::vector<const JValue *>> JArray::data() const
{
    std::vector<const JValue *> result;
    result.reserve(m_data.size());
    for (const auto &v : m_data) {
        result.emplace_back(&v);
    }
    return m_data.empty() ? std::nullopt : std::make_optional(result);
}

JParent JArray::parent() const
{
    return m_parent;
}

std::ostream &JArray::operator<<(std::ostream &os) const
{
    os << toString();
    return os;
}

std::string JArray::toString() const
{
    std::ostringstream os;

    os << "[";
    bool first = true;
    for (const auto &value : m_data) {
        if (!first) {
            os << ",";
        }
        first = false;
        std::visit(
            [&os](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::unique_ptr<JObject>>) {
                    os << v->toString();
                } else if constexpr (std::is_same_v<T, std::unique_ptr<JArray>>) {
                    os << v->toString();
                } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
                    os << "null";
                } else {
                    os << v;
                }
            },
            value);
    }
    os << "]";
    return os.str();
}

} // namespace psi::json
