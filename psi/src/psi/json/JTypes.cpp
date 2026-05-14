
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
            auto writeEscaped = [&os](const std::string &s) {
                os << '"';
                for (char ch : s) {
                    if (ch == '"') {
                        os << "\\\"";
                    } else if (ch == '\\') {
                        os << "\\\\";
                    } else if (ch == '\n') {
                        os << "\\n";
                    } else if (ch == '\r') {
                        os << "\\r";
                    } else if (ch == '\t') {
                        os << "\\t";
                    } else if (ch == '\b') {
                        os << "\\b";
                    } else if (ch == '\f') {
                        os << "\\f";
                    } else {
                        os << ch;
                    }
                }
                os << '"';
            };
            if constexpr (std::is_same_v<std::string, TypeStringValue>) {
                writeEscaped(v);
            } else if constexpr (std::is_same_v<std::wstring, TypeStringValue>) {
                writeEscaped(tools::wstring_to_utf8(v));
            } else {
                static_assert(always_false_v<TypeStringValue>, "not implemented TypeStringValue handler");
            }
        },
        value);
    return os;
}

} // namespace psi::json
