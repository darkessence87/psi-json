include_directories(
    include
    src
)

set (SOURCES
    src/psi/json/JArray.cpp
    src/psi/json/JObject.cpp
    src/psi/json/JsonParser.cpp
    src/psi/json/JTree.cpp
    src/psi/json/JTypes.cpp
)

add_library(psi-json ${SOURCES})

set(TEST_SRC
    tests/JsonParserTests.cpp
    tests/JTypesTests.cpp
)
# psi_make_tests("Json" "${TEST_SRC}" "psi-json;psi-shared;psi-tools")
psi_make_tests("Json" "${TEST_SRC}" "psi-json;psi-tools")
add_custom_command(TARGET PSI_TEST_Json POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/tests/test_data ${BUILD_OUT})

# psi_make_examples("1_JsonExamples" "examples/1_JsonExamples.cpp" "psi-json;psi-logger;psi-shared")
psi_make_examples("1_JsonExamples" "examples/1_JsonExamples.cpp" "psi-json;psi-tools")