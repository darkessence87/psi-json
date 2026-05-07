# Description

A streaming, UTF-8 JSON parser. Types are resolved mostly at compile time. The parser implements an online algorithm, making it suitable for streamed data.

- [*JsonParser*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JsonParser.h) — Entry point. Parses a JSON byte stream or file into a `JTree`.
- [*JTree*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTree.h) — Owns the parsed document; provides `asObject()` / `asArray()` access to the root and a `toString()` serialiser.
- [*JObject*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JObject.h) — Represents a JSON object `{ "key": value, … }`. Backed by `std::multimap<JKey, JValue>`. Key access methods: `at`, `get`, `getKeys`, `getArray`, `getObject`, `getString`, `getStringW`.
- [*JArray*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JArray.h) — Represents a JSON array `[ value, … ]`. Backed by `std::vector<JValue>`. Provides `data()` and `toString()`.
- [*JKey*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTypes.h) — `std::string` key used for object members.
- [*JValue*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTypes.h) — `std::variant<JNull, JNumber, JString, bool, JObject*, JArray*>` representing any JSON value.
- [*JNumber*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTypes.h) — `std::variant` over all integer and floating-point types (`int16_t`…`uint64_t`, `float`, `double`).
- [*JString*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTypes.h) — `std::variant<std::wstring, std::string>`.
- [*JParent*](https://github.com/darkessence87/psi-json/blob/master/psi/include/psi/json/JTypes.h) — `std::variant<JObject *, JArray *>` back-pointer used internally to track the owning container.

# Usage examples
* [1 Json examples](https://github.com/darkessence87/psi-json/blob/master/psi/examples/1_JsonExamples.cpp)
