#include "psi/json/JObject.h"
#include "psi/json/JArray.h"

#include "psi/tools/Tools.h"

#include <sstream>

namespace psi::json {

JObject::JObject(JParent parent)
    : m_parent(parent)
{
}

void JObject::insert(JKey &&key, JValue &&value)
{
    m_data.emplace_back(std::make_pair(std::move(key), std::move(value)));
}

// void JObject::insert(const JKey &key, JValue &&value)
// {
//     m_data.emplace_back(std::make_pair(std::move(key), value));
// }

JValue *JObject::at(const JKey &key)
{
    for (auto &p : m_data) {
        if (p.first == key) {
            return &p.second;
        }
    }
    return nullptr;
}

std::optional<JValue> JObject::get(const JKey &key) const
{
    for (auto &p : m_data) {
        if (p.first == key) {
            return std::make_optional(p.second);
        }
    }
    return std::nullopt;
}

std::list<JKey> JObject::getKeys() const
{
    std::list<JKey> result;
    for (auto it : m_data) {
        result.emplace_back(it.first);
    }
    return result;
}

const JObject::DataType &JObject::data() const
{
    return m_data;
}

JParent JObject::parent() const
{
    return m_parent;
}

std::string JObject::toString() const
{
    std::ostringstream os;

    os << "{";
    for (auto it : m_data) {
        os << "\"" << it.first << "\":";
        std::visit(
            [&os](auto &&v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, JObject>) {
                    os << v.toString();
                } else if constexpr (std::is_same_v<T, JArray>) {
                    os << v.toString();
                } else if constexpr (std::is_same_v<T, bool>) {
                    os << std::boolalpha << v;
                } else {
                    os << v;
                }
            },
            it.second);
        os << ",";
    }
    if (!m_data.empty()) {
        os.seekp(-1, std::ios_base::cur);
    }
    os << "}";

    return os.str();
}

std::ostream &JObject::operator<<(std::ostream &os) const
{
    os << toString();
    return os;
}

std::optional<JArray *> JObject::getArray(const JKey &key)
{
    auto arr = std::get_if<JArray>(this->at(key));
    return arr ? std::make_optional(arr) : std::nullopt;
}

std::optional<JObject *> JObject::getObject(const JKey &key)
{
    auto optValue = this->at(key);
    if (!optValue) {
        return std::nullopt;
    }

    auto obj = std::get_if<JObject>(optValue);
    return obj ? std::make_optional(obj) : std::nullopt;
}

std::list<std::optional<JObject *>> JObject::getArrayObject(const JKey &key)
{
    auto optValue = this->at(key);
    if (!optValue) {
        return {};
    }

    auto arr = std::get_if<JArray>(optValue);
    if (!arr) {
        return {};
    }

    const auto &data = arr->data();
    if (!data.has_value()) {
        return {};
    }

    std::list<std::optional<JObject *>> result;
    for (auto v : data.value()) {
        if (auto o = std::get_if<JObject>(v)) {
            result.emplace_back(o);
        } else {
            break;
        }
    }
    return result;
}

int64_t JObject::getNumberInt64(const JKey &key) const
{
    return getNumber<int64_t>(key);
}

int32_t JObject::getNumberInt32(const JKey &key) const
{
    return getNumber<int32_t>(key);
}

int16_t JObject::getNumberInt16(const JKey &key) const
{
    return getNumber<int16_t>(key);
}

uint64_t JObject::getNumberUInt64(const JKey &key) const
{
    return getNumber<int64_t>(key);
}

uint32_t JObject::getNumberUInt32(const JKey &key) const
{
    return getNumber<int32_t>(key);
}

uint16_t JObject::getNumberUInt16(const JKey &key) const
{
    return getNumber<int16_t>(key);
}

double_t JObject::getNumberDouble(const JKey &key) const
{
    return getNumber<double_t>(key);
}

std::string JObject::getString(const JKey &key) const
{
    return getString<std::string>(key);
}

std::wstring JObject::getStringW(const JKey &key) const
{
    return getString<std::wstring>(key);
}

bool JObject::getBool(const JKey &key) const
{
    auto optValue = this->get(key);
    if (!optValue.has_value()) {
        return {};
    }

    if (!std::holds_alternative<bool>(optValue.value())) {
        return {};
    }

    return std::get<bool>(optValue.value());
}

std::list<int64_t> JObject::getArrayNumberInt64(const JKey &key) const
{
    return getArrayOf<int64_t, JNumber>(key);
}

std::list<std::string> JObject::getArrayString(const JKey &key) const
{
    return getArrayOf<std::string, JString>(key);
}

std::list<std::wstring> JObject::getArrayStringW(const JKey &key) const
{
    return getArrayOf<std::wstring, JString>(key);
}

template <typename T>
T JObject::getNumber(const JKey &key) const
{
    auto optValue = this->get(key);
    if (!optValue.has_value()) {
        return {};
    }

    if (!std::holds_alternative<JNumber>(optValue.value())) {
        return {};
    }

    auto v = std::get<JNumber>(optValue.value());
    T result = getAs<T, JNumber>(v);
    return result;
}

template <typename T>
T JObject::getString(const JKey &key) const
{
    auto optValue = this->get(key);
    if (!optValue.has_value()) {
        return {};
    }

    if (!std::holds_alternative<JString>(optValue.value())) {
        return {};
    }

    auto v = std::get<JString>(optValue.value());
    T result = getAs<T, JString>(v);
    return result;
}

template <typename T, typename V>
std::list<T> JObject::getArrayOf(const JKey &key) const
{
    auto optValue = this->get(key);
    if (!optValue.has_value()) {
        return {};
    }

    if (!std::holds_alternative<JArray>(optValue.value())) {
        return {};
    }

    auto &arr = std::get<JArray>(optValue.value());
    const auto &data = arr.data();
    if (!data.has_value()) {
        return {};
    }

    std::list<T> result;
    for (const auto &v : data.value()) {
        result.emplace_back(getAs<T, V>(*v));
    }
    return result;
}

template <typename R, typename V>
R JObject::getAs(const JValue &value) const
{
    R result = R();
    std::visit(
        [&result](auto &&v) {
            using TypeValue = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, TypeValue>) {
                if constexpr (std::is_same_v<V, JNull>) {
                    // not implemented yet
                } else if constexpr (std::is_same_v<V, JNumber>) {
                    std::visit([&result](auto &&v) { result = static_cast<R>(v); }, v);
                } else if constexpr (std::is_same_v<V, JString>) {
                    std::visit(
                        [&result](auto &&v) {
                            using TypeStringValue = std::decay_t<decltype(v)>;
                            if constexpr (std::is_same_v<R, std::string> && std::is_same_v<std::string, TypeStringValue>) {
                                result = v;
                            } else if constexpr (std::is_same_v<R, std::wstring> && std::is_same_v<std::string, TypeStringValue>) {
                                result = tools::utf8_to_wstring(v);
                            } else if constexpr (std::is_same_v<std::string, TypeStringValue>) {
                                // @todo: remove std::wstring from JString variant
                            } else if constexpr (std::is_same_v<std::wstring, TypeStringValue>) {
                                // @todo: remove std::wstring from JString variant
                            } else {
                                static_assert(always_false_v<TypeStringValue>, "not implemented TypeStringValue handler");
                            }
                        },
                        v);
                } else if constexpr (std::is_same_v<V, JObject>) {
                    result = R(v);
                } else {
                    static_assert(always_false_v<V>, "not implemented V handler");
                }
            }
        },
        value);
    return result;
}

} // namespace psi::json