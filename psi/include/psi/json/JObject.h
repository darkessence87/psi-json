#pragma once

#include "JTypes.h"

#include <map>
#include <optional>
#include <vector>

namespace psi::json {

/**
 * @brief 
 * 
 */
class JObject final
{
    using DataType = std::multimap<JKey, JValue>;

public:
    JObject() = default;
    JObject(JParent parent);
    ~JObject() = default;

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
     * @return const JValue*
     */
    const JValue *at(const JKey &key) const;

    /**
     * @brief 
     * 
     * @param key 
     * @return std::optional<const JValue*>
     */
    std::optional<const JValue *> get(const JKey &key) const;

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
    std::vector<JKey> getKeys() const;

    /**
     * @brief Get the Array object
     * 
     * @param key 
     * @return std::optional<const JArray *>
     */
    std::optional<const JArray *> getArray(const JKey &key) const;

    /**
     * @brief Get the Object object
     * 
     * @param key 
     * @return std::optional<JObject *>
     */
    std::optional<const JObject *> getObject(const JKey &key) const;

    /**
     * @brief Get the Array Object object
     * 
     * @param key 
     * @return std::vector<std::optional<JObject *>>
     */
    std::vector<std::optional<const JObject *>> getArrayObject(const JKey &key) const;

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
     * @return std::vector<int64_t>
     */
    std::vector<int64_t> getArrayNumberInt64(const JKey &key) const;

    /**
     * @brief Get the Array String object
     * 
     * @param key 
     * @return std::vector<std::string>
     */
    std::vector<std::string> getArrayString(const JKey &key) const;

    /**
     * @brief Get the Array String W object
     * 
     * @param key 
     * @return std::vector<std::wstring>
     */
    std::vector<std::wstring> getArrayStringW(const JKey &key) const;

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

private:
    template <typename T>
    T getNumber(const JKey &key) const;

    template <typename T>
    T getString(const JKey &key) const;

    template <typename T, typename V>
    std::vector<T> getArrayOf(const JKey &key) const;

    template <typename R, typename V>
    R getAs(const JValue &value) const;

    JObject(const JObject &) = delete;
    JObject operator=(const JObject &) = delete;
    JObject(JObject &&) = default;
    JObject &operator=(JObject &&) = default;

private:
    DataType m_data;
    std::vector<DataType::const_iterator> m_data_indices;
    JParent m_parent;
};

} // namespace psi::json