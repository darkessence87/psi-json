
#include "psi/json/JTypes.h"
#include "psi/tools/Tools.h"

#include <iomanip>
#include <type_traits>

namespace psi::json {

std::ostream &operator<<(std::ostream &os, const JNull &)
{
    os << "null";
    return os;
}

std::ostream &operator<<(std::ostream &os, const JNumber &value)
{
    std::visit(
        [&os](auto &&v) {
            if constexpr (std::is_same_v<double_t, std::decay_t<decltype(v)>>) {
                os << std::scientific << std::setprecision(15) << v;
            } else if constexpr (std::is_same_v<float_t, std::decay_t<decltype(v)>>) {
                os << std::scientific << std::setprecision(6) << v;
            } else {
                os << std::to_string(v);
            }
        },
        value);

    return os;
}

std::ostream &operator<<(std::ostream &os, const JString &value)
{
    std::visit(
        [&os](auto &&v) {
            using TypeStringValue = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<std::string, TypeStringValue>) {
                os << "\"" << v << "\"";
            } else if constexpr (std::is_same_v<std::wstring, TypeStringValue>) {
                os << "\"" << tools::wstring_to_utf8(v) << "\"";
            } else {
                static_assert(always_false_v<TypeStringValue>, "not implemented TypeStringValue handler");
            }
        },
        value);
    return os;
}

} // namespace psi::json