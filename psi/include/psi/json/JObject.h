#pragma once

#include "JTypes.h"

#include <list>
#include <optional>

namespace psi::json {

/**
 * @brief 
 * 
 */
class JObject
{
    using DataType = std::list<std::pair<JKey, JValue>>;

public:
    JObject() = default;
    JObject(JParent parent);

    /**
     * @brief 
     * 
     * @param key 
     * @param value 
     */
    void insert(JKey &&key, JValue &&value);

    /**
     * @brief 
     * 
     * @param key 
     * @return JValue* 
     */
    JValue *at(const JKey &key);

    /**
     * @brief 
     * 
     * @param key 
     * @return std::optional<JValue> 
     */
    std::optional<JValue> get(const JKey &key) const;

    /**
     * @brief 
     * 
     * @return const DataType& 
     */
    const DataType &data() const;

    /**
     * @brief 
     * 
     * @return JParent 
     */
    JParent parent() const;

    /**
     * @brief Get the Keys object
     * 
     * @return std::list<JKey> 
     */
    std::list<JKey> getKeys() const;

    /**
     * @brief Get the Array object
     * 
     * @param key 
     * @return std::optional<JArray *> 
     */
    std::optional<JArray *> getArray(const JKey &key);

    /**
     * @brief Get the Object object
     * 
     * @param key 
     * @return std::optional<JObject *> 
     */
    std::optional<JObject *> getObject(const JKey &key);

    /**
     * @brief Get the Array Object object
     * 
     * @param key 
     * @return std::list<std::optional<JObject *>> 
     */
    std::list<std::optional<JObject *>> getArrayObject(const JKey &key);

    /**
     * @brief Get the Number Int64 object
     * 
     * @param key 
     * @return int64_t 
     */
    int64_t getNumberInt64(const JKey &key) const;

    /**
     * @brief Get the Number Int32 object
     * 
     * @param key 
     * @return int32_t 
     */
    int32_t getNumberInt32(const JKey &key) const;

    /**
     * @brief Get the Number Int16 object
     * 
     * @param key 
     * @return int16_t 
     */
    int16_t getNumberInt16(const JKey &key) const;

    /**
     * @brief Get the Number U Int64 object
     * 
     * @param key 
     * @return uint64_t 
     */
    uint64_t getNumberUInt64(const JKey &key) const;

    /**
     * @brief Get the Number U Int32 object
     * 
     * @param key 
     * @return uint32_t 
     */
    uint32_t getNumberUInt32(const JKey &key) const;

    /**
     * @brief Get the Number U Int16 object
     * 
     * @param key 
     * @return uint16_t 
     */
    uint16_t getNumberUInt16(const JKey &key) const;

    /**
     * @brief Get the Number Double object
     * 
     * @param key 
     * @return double_t 
     */
    double_t getNumberDouble(const JKey &key) const;

    /**
     * @brief Get the String object
     * 
     * @param key 
     * @return std::string 
     */
    std::string getString(const JKey &key) const;

    /**
     * @brief Get the String W object
     * 
     * @param key 
     * @return std::wstring 
     */
    std::wstring getStringW(const JKey &key) const;

    /**
     * @brief Get the Bool object
     * 
     * @param key 
     * @return true 
     * @return false 
     */
    bool getBool(const JKey &key) const;

    /**
     * @brief Get the Array Number Int64 object
     * 
     * @param key 
     * @return std::list<int64_t> 
     */
    std::list<int64_t> getArrayNumberInt64(const JKey &key) const;

    /**
     * @brief Get the Array String object
     * 
     * @param key 
     * @return std::list<std::string> 
     */
    std::list<std::string> getArrayString(const JKey &key) const;

    /**
     * @brief Get the Array String W object
     * 
     * @param key 
     * @return std::list<std::wstring> 
     */
    std::list<std::wstring> getArrayStringW(const JKey &key) const;

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

protected:
    template <typename T>
    T getNumber(const JKey &key) const;

    template <typename T>
    T getString(const JKey &key) const;

    template <typename T, typename V>
    std::list<T> getArrayOf(const JKey &key) const;

    template <typename R, typename V>
    R getAs(const JValue &value) const;

private:
    DataType m_data;
    JParent m_parent;
};

} // namespace psi::json